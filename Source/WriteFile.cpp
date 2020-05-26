#include "WriteFile.h"

// Author : Samuel Wesley Rasquinha

/*
https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
*/

/*
TODO: 
To be able to write unicode characters in a file! 
*/

/*CHUNKS in a bat file
FSIG : Bat file signature
FNSZ : File name size (in bytes)
FLAB : File name chunk whose size is equal to the value obtained by reading the FNSZ chunk
FSZ : File size chunk
PRO : File property chunk consists of 1's and 0's which specifies specific properties of a file 
FCON : Contents of the file itself
*/

//FSIG Chunk 
int file_limit = 65000; // The maximum number of files that can be stored in a .bat file
string hex_null = "&"; // The character to be replaced for an empty blank in the FSIG chunk (Amount of files) 
string bat_ver = "0.0.1";

//FNSZ Chunk
int max_labelsize = 1024;
//FSZ Chunk
static long long max_filesz = 1099511627776; // There is a copy of this in the View.h file


int WriteFile(bat_Operation bat_op)
{
	//TODO : COPY THE REQUIRED FILES FOR EACH OPERATION AND THINK OF AN ALGORITHM TO EDIT PREVIOUSLY WRITTEN FILES
	
	fstream archive(bat_op.archive) ; 
	
	string chunk; 
	string buff_1;

	vector <string> directory_files; 
	
	if (bat_op.operation == op_wof)
	{
		if (WriteSpaceCheck(bat_op.file,bat_op.archive,0) == true)
		{
			if (bat_op.o_write == true) 
			{
				_WriteFile(bat_op.archive, bat_op.file);
			}
				
			else
			{
				chunk = GetFSIG(); 
				archive.seekp(ios::beg, 0);
				archive.write(chunk.c_str(), chunk.size()); 
				archive.close(); 

				_WriteFile(bat_op.archive, bat_op.file);
			}
				
		}

		else
		{
			PostError("Not enough space on the drive");
		}
	}

	else if (bat_op.operation == op_wad)
	{
		if (WriteSpaceCheck(bat_op.dir, bat_op.archive, 1) == true)
		{
			if (bat_op.o_write == true)
			{
				GetFileListFromDir(bat_op.dir,directory_files);

				for (int i = 0; i < directory_files.size(); i++)
				{
					_WriteFile(bat_op.archive, directory_files[i]);
				}
			}
				
			else
			{
				chunk = GetFSIG();
				archive.seekp(ios::beg, 0);
				archive.write(chunk.c_str(), chunk.size());
				archive.close();
					
				GetFileListFromDir(bat_op.dir, directory_files);

				for (int i = 0; i < directory_files.size(); i++)
				{
					_WriteFile(bat_op.archive, directory_files[i]);
				}
			}
		}

		else
		{
			PostError("Not enough space on the drive");
		}
	}

	else if (bat_op.operation == op_wal)
	{
		if (WriteSpaceCheck(bat_op.file_list, bat_op.archive) == true)
		{
			if (bat_op.o_write == true)
			{
				for (int i = 0; i < (bat_op.file_list).size(); i++)
				{
					_WriteFile(bat_op.archive, bat_op.file_list[i]);
				}
			}

			else
			{
				chunk = GetFSIG();
				archive.seekp(ios::beg, 0);
				archive.write(chunk.c_str(), chunk.size());
				archive.close();

				for (int i = 0; i < (bat_op.file_list).size(); i++)
				{
					_WriteFile(bat_op.archive, bat_op.file_list[i]);
				}
			}
		}
			
		else
		{
			PostError("Not enough space on the drive");
		}
	}

	archive.close();
	//UpdateCRCInFile(bat_op.archive); 
	return 0; 
}


int _WriteFile(string file_a, string file_f) // file_a is the archive and file_f is the file to copy
{
	ofstream arc;
	ifstream file;
	string buff; 

	file.open(file_f);
	arc.open(file_a, ios::binary | ios::app | ios::out);
	arc.seekp(0, ios::end);

	if (arc.good() && file.good())
	{
		file.close();

		buff = GetFNSZ(file_f);
		arc.write(buff.c_str(), buff.size());
		buff = GetFLAB(file_f);
		arc.write(buff.c_str(), buff.size());
		buff = GetFSZ(file_f);
		arc.write(buff.c_str(), buff.size());
		buff = GetPRO(file_f);
		arc.write(buff.c_str(), buff.size());
		auto start_crc = std::chrono::system_clock::now();
		buff = GenerateCRC(file_f);
		buff = ExtendString(buff, 10, "&");
		arc.write(buff.c_str(), buff.size());
		auto end_crc = std::chrono::system_clock::now(); // Measure the time taken for crc
		auto elapsed_crc = std::chrono::duration_cast<std::chrono::milliseconds>(end_crc - start_crc);
		arc.close();

		auto start_write = std::chrono::system_clock::now(); // Measure the time taken
		WriteFCON(file_f, file_a, 1);
		auto end_write = std::chrono::system_clock::now(); // Measure the time taken
		auto elapsed_write = std::chrono::duration_cast<std::chrono::milliseconds>(end_write - start_write);
		cout << "\nWriting " << file_f << " to the .bat file took " << elapsed_write.count()
			<< " milliseconds and " << elapsed_crc.count() << " milliseconds to generate the CRC Code";



		return 1; 
	}
	
	else
	{
		PostError("Could not write to the bat file.");
		return -1;
	}

}

string GetFSIG() // Get File Signature
{
	/*string bat_filesig = "%bat%"; // Set this to whatever
	string ret_val; 
	string hex_amt; 
	string stemp_1; 
	string stemp_2; 
	int file_limitc;
	int itemp_1; 
	int itemp_2; 

	if (file_amt > file_limit)
	{
		return '\0';
	}

	hex_amt = GetHexString(file_amt);
	stemp_1 = GetHexString(file_limit); 
	hex_amt = ExtendString(hex_amt, stemp_1.size(), hex_null); 

	bat_filesig = bat_filesig + bat_ver ;
	ret_val = bat_filesig + hex_amt; 

	return ret_val;*/

	return "%BAT%"; 
}

string GetFNSZ(string file_name)
{
	string hex_fsz; // file name size
	string hex_mfsz; // max file name size
	std::filesystem::path pth = std::filesystem::path(file_name.c_str()).filename();
	string ret_val = pth.string();

	if (file_name.size() > max_labelsize)
	{
		// report error
		PostError("The file name is too big (Max file size is 1024 bytes)"); 
	}

	else
	{
		hex_fsz = GetHexString(ret_val.size());
		hex_mfsz = GetHexString(max_labelsize); 
		hex_fsz = ExtendString(hex_fsz, hex_mfsz.size(), hex_null);
		
		return hex_fsz; 
	}
	
}

string GetFLAB(string file_name) // Get File Label 
{
	// VERIFIED

	std::filesystem::path pth = std::filesystem::path(file_name.c_str()).filename(); 
	string ret_val = pth.string(); 

	return ret_val;
}

string GetFSZ(string file_path)
{
	long long int_fz = _GetFileSize(file_path);
	string hex_fz; // hex value of file size
	string hex_mfz; // hex value of max file size

/*	if (int_fz > max_filesz)
	{
		PostError("File is too big"); 
		
	}*/

	hex_fz = GetHexString(int_fz); 
	hex_mfz = GetHexString(max_filesz);
	hex_fz = ExtendString(hex_fz, hex_mfz.size(), hex_null); 

	return hex_fz; 
}

string GetPRO(string file_path)
{
	DWORD file_attributes = GetFileAttributesA(file_path.c_str()); 
	char pro_val[9];
	string ret_val; 

	pro_val[0] = '0' ; 
	pro_val[1] = '0';
	pro_val[2] = '0';
	pro_val[3] = '0';
	pro_val[4] = '0';
	pro_val[5] = '0';
	pro_val[6] = '0';
	pro_val[7] = '0';
	pro_val[8] = '\0'; 

	if (file_attributes & FILE_ATTRIBUTE_DEVICE)
	{
		pro_val[0] = '1'; 
	}

	if (file_attributes & FILE_ATTRIBUTE_SYSTEM)
	{
		pro_val[1] = '1';
	}

	if (file_attributes & FILE_ATTRIBUTE_VIRTUAL)
	{
		pro_val[2] = '1';
	}

	if (file_attributes & FILE_ATTRIBUTE_TEMPORARY)
	{
		pro_val[3] = '1';
	}

	if (file_attributes & FILE_ATTRIBUTE_HIDDEN)
	{
		pro_val[4] = '1';
	}

	if (file_attributes & FILE_ATTRIBUTE_ENCRYPTED)
	{
		pro_val[5] = '1';
	}

	if (file_attributes & FILE_ATTRIBUTE_READONLY)
	{
		pro_val[6] = '1';
	}

	if (file_attributes & FILE_ATTRIBUTE_NORMAL)
	{
		pro_val[7] = '1';
	}

	ret_val = pro_val; 
	return ret_val; 
}

//Write file contents from a file to another file.
//The variable pos indicates the position to write if pos == 1 then write to the end else write to the beginning 
bool WriteFCON(string src, string dest, bool pos) 
{
	/*
	TODO : 
	Check if the copying has an effect on the file character location pointer
	*/
	
	long copy_sz = CPY_SPEED ; // Amount of bytes to copy CURRENTLY IT IS SET TO AROUND 10 MB
	char* copy_buff = new char[copy_sz+1];
	long int source_sz;
	long int run_times;
	long int lastrun_sz;

	ifstream source_f;
	ofstream dest_f;

	source_f.open(src, ios::binary);
	dest_f.open(dest, ios::binary | ios::app);

	if (!source_f.good() || !dest_f.good() || !source_f.is_open() || !dest_f.is_open())
	{
		dest_f.close();
		source_f.close();
		PostError("The source or destination file is bad", true); 

		return -1;
	}

	if (pos == 0)
	{
		dest_f.seekp(0, ios::beg) ;
	}

	else if (pos == 1)
	{
		dest_f.seekp(0, ios::end); 
	}

	source_sz = _GetFileSize(src) ;

	if (source_sz >= copy_sz)
	{
		run_times = source_sz / copy_sz;
		lastrun_sz = source_sz % copy_sz; // Amount of bytes to write for the last run

		while (run_times != 0)
		{
			source_f.read(copy_buff, copy_sz);
			copy_buff[copy_sz] = '\0';
			dest_f.write(copy_buff, copy_sz);
			run_times--;
		}


		if (lastrun_sz != 0)
		{
			source_f.read(copy_buff, lastrun_sz);
			copy_buff[lastrun_sz] = '\0';
			dest_f.write(copy_buff, lastrun_sz);
		}
	}

	else if (source_sz < copy_sz)
	{
		source_f.read(copy_buff, source_sz);
		copy_buff[source_sz] = '\0'; 
		dest_f.write(copy_buff, source_sz);
	}

	delete[] copy_buff; 
	source_f.close();
	dest_f.close();
}

void GetFileListFromDir(string path, vector <string> &vec)
{
	filesystem::path pth;

	for (std::filesystem::directory_entry entry : filesystem::directory_iterator(path))
	{
		pth = entry.path();
		vec.push_back(pth.string());
	}

	return; 
}

long long GetFreeSpaceOnVolume(string dir)
{
	long long retval;
	wstring drive(dir.begin(), dir.end());
	DWORD lpSectorsPerCluster;
	DWORD lpBytesPerSector;
	DWORD lpNumberOfFreeClusters;
	DWORD lpTotalNumberOfClusters;

	GetDiskFreeSpace(drive.c_str(), &lpSectorsPerCluster, &lpBytesPerSector, &lpNumberOfFreeClusters, &lpTotalNumberOfClusters);
	retval = (long long)lpSectorsPerCluster * lpBytesPerSector * lpNumberOfFreeClusters;

	return retval;
}

int GetFileSystemDrive(string _drive)
{
	_drive.push_back('\\');
	_drive.push_back('\\');
	
	TCHAR* szFileSystemName = new TCHAR[6];
	wstring drive(_drive.begin(), _drive.end());
	GetVolumeInformation(drive.c_str(), NULL, 0, NULL, NULL, NULL, szFileSystemName, 255);

	if (wcscmp(szFileSystemName, L"NTFS") == 0)
	{
		return fs_NTFS;
	}

	else if (wcscmp(szFileSystemName, L"FAT32") == 0)
	{
		return fs_FAT32;
	}

	else if (wcscmp(szFileSystemName, L"EXFAT") == 0)
	{
		return fs_EXFAT; 
	}

	else
	{
		PostError("Unsupported file system"); 
		return -1; 
	}
}

// Overloaded function WriteSpaceCheck(vector<string>,string) && WriteSpaceCheck(string,string,bool) 
bool WriteSpaceCheck(vector <string> files, string _archive)
{
	int drivefs; 
	bool passed = false; // To verify whether the first check went through
	long long file_size_totaler = 0;
	long long holder_var = 0;
	long long archive_size = 0;
	string temp1; 
	ifstream temp;
	std::filesystem::path archive(_archive) ; 
	string error_txt; 

	for (int i = 0; i < files.size(); i++)
	{
		temp.open(files[i]); 
		
		if (temp.good() == true)
		{
			holder_var = _GetFileSize(files[i]);
			temp.close(); 
		}

		else
		{
			temp.close();
			return false; 
		}

		// Arithmetic that deals with 2 longs, to check
		file_size_totaler = file_size_totaler + holder_var;
	}

	archive_size = _GetFileSize(_archive);
	file_size_totaler = file_size_totaler + archive_size; // Make Up for the additional space taken by the archive if the user actually wants the files to be archived

	temp1 = (archive.root_name()).string() ;//(archive.root_name).string();
	drivefs = GetFileSystemDrive(temp1) ; 

	if (drivefs == fs_NTFS)
	{
		if (file_size_totaler < MAX_NTFSFS)
		{
			passed = true;
		}
	}
	
	else if (drivefs == fs_FAT32)
	{
		if (file_size_totaler < MAX_FAT32FS)
		{
			passed = true; 
		}
	}

	else if (drivefs == fs_EXFAT)
	{
		if (file_size_totaler < MAX_EXFATFS)
		{
			passed = true; 
		}
	}

	if (passed == true)
	{
		if (file_size_totaler < GetFreeSpaceOnVolume((archive.root_name()).string()))
		{
			return true; 
		}
	}

	else
	{
		error_txt = "There is not enough space on drive ";
		error_txt = error_txt + (archive.root_name()).string();
		PostError(error_txt);
		return false; 
	}
}

bool WriteSpaceCheck(string files, string _archive, bool dir)
{
	/* EXPLANATION FOR THIS OVERLOADED FUNCTION
	WriteCheck(string,string,bool) 
	There are 2 ways of getting the file(s) to archive using a single variable argument 
		- Archive all the files from a particular directory 
		- Archive a single file
	*/
	
	int temp;
	int drivefs = 0 ;
	bool passed = false;
	long long archive_sz = 0 ; 
	long long file_size_totaler = 0 ; 
	long long holder = 0 ;
	vector <string> dfiles; 
	filesystem::path archive(_archive);
	string error_txt; 

	archive_sz = _GetFileSize(_archive); 

	if (dir == false)
	{
		holder = _GetFileSize(files);
		file_size_totaler = archive_sz + holder; 
	}

	else if (dir == true)
	{
		GetFileListFromDir(files,dfiles);
		temp = dfiles.size(); 
		for (int i = 0; i < temp; i++)
		{
			holder = _GetFileSize(dfiles[i]);
			file_size_totaler = file_size_totaler + holder; 
		}

		file_size_totaler = file_size_totaler + archive_sz; 

	}

	drivefs = GetFileSystemDrive((archive.root_name()).string());

	if (drivefs == fs_NTFS)
	{
		if (file_size_totaler < MAX_NTFSFS)
		{
			passed = true;
		}
	}

	else if (drivefs == fs_FAT32)
	{
		if (file_size_totaler < MAX_FAT32FS)
		{
			passed = true;
		}
	}

	else if (drivefs == fs_EXFAT)
	{
		if (file_size_totaler < MAX_EXFATFS)
		{
			passed = true;
		}
	}

	if (passed == true)
	{
		if (file_size_totaler < GetFreeSpaceOnVolume((archive.root_name()).string()))
		{
			return true;
		}
	}

	else
	{
		error_txt = "There is not enough space on drive " ; 
		error_txt = error_txt + (archive.root_name()).string(); 
		PostError(error_txt); 
		return false;
	}
}

