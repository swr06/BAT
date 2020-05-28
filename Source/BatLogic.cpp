#include "BatLogic.h"

bat_Operation ParseCMDArg(int cargc, char** cargv)
{
	/* CARGV
	argv[0] EXECUTABLE DIR
	argv[1] OPERATION EX "\d" or "\v"
	argv[2] BAT ARCHIVE path
	argv[3] ARGUMENT 1
	*/

	bool flag = true;
	signed int ret_val = -1;
	bool fileres; // file result
	string op;
	string archive;
	fstream farchive;

	string buff_1;
	string buff_2;
	vector <string> buff_list;
	bat_Operation bat_op;

	if (cargc < 3)
	{
		PostError("Invalid number of arguments!", true); 
	}

	op = cargv[1];
	archive = cargv[2];
	farchive.open(archive.c_str());
	fileres = farchive.good();

	bat_op.archive = archive;

	if (fileres == 0) //If the file does not exist
	{
		bat_op.can_extract = false; // Set the extracting capability to false
		bat_op.o_write = false;

		// I M P O R T A N T ! ASK THE USER WHETHER THEY CAN OVERWRITE THE BAT FILE

		CreateBATFile(archive); // Create the file
		farchive.open(archive); // Open it and check if is done correctly

		if (farchive.good() == true)
		{
			bat_op.o_write = false;
		}

		else
		{
			//at_op = 0;
			flag = false;
			PostError("The BAT File does not exist. Tried to create but creation failed",true); 	// POST HUGE ERROR HERE !! 
		}

	}

	else if (fileres == 1) // if it exists, check if it is a bat file
	{
		if (IsBatFile(archive)) // Checks if the archive is a bat file
		{
			bat_op.can_extract = true; // Should I have a better IsBatFile() function 
			bat_op.o_write = true; 
		}

		else
		{
			cout << "\nFile exists but is not a BAT file." ; 
		}
	}

	else
	{
		bat_op.o_write = false;
	}

	farchive.close(); 

	if (op == "\v" || op == "\view" || op == "\\V" || op == "\\v" || op == "\\view")
	{
		ret_val = op_v;
	}

	else if (op == "\d" || op == "\delete" || op == "\\D" || op == "\\delete" || op == "\\d")
	{
		bat_op.file = cargv[3];
		ret_val = op_d;
	}

	else if (op == "\WOF" || op == "\\WOF" || op == "WOF")
	{
		if (ChkFile(cargv[3]) == 1)
		{
			bat_op.file = cargv[3];
		}
		
		ret_val = op_wof;
	}

	else if (op == "\WAD" || op == "\\WAD" || op == "WAD")
	{
		if (IsDirectory(cargv[3]))
		{
			bat_op.dir = cargv[3];
		}

		ret_val = op_wad; 
	}

	else if (op == "\WAL" || op == "\\WAL" || op == "WAL")
	{
		for (int i = 3; i < cargc; i++)
		{
			if (ChkFile(cargv[i]) == 1)
			{
				bat_op.file_list.push_back(cargv[i]);
			}
			
			else
			{
				cout << "\n" << cargv[i] << "  Is An Invalid File." << "\n";
				// POST ERROR
			}
		}
		
		ret_val = op_wal; 
	}

	// Extract Functions 
	else if (op == "\EOF" || op == "\\EOF" || op == "EOF")
	{
		bat_op.file = cargv[3]; 
		bat_op.dir = cargv[4]; 

		ret_val = op_eof;
	}
	
	else if (op == "\EAF" || op == "\\EAF" || op == "EAF")
	{
		bat_op.dir = cargv[3]; 

		ret_val = op_eaf;
	}

	else if (op == "\EAL" || op == "\\EAL" || op == "EAL")
	{
		bat_op.dir = cargv[3];
		
		for (int i = 4; i < cargc; i++)
		{
			// TODO : Add an if statement that checks if the file exists in the archive which is passed to the program

			bat_op.file_list.push_back(cargv[i]); 
		}

		ret_val = op_eal;
	}

	if (flag == false)
	{
		bat_op.operation = ret_val;
	}
 
	bat_op.operation = ret_val; 

	return bat_op; 
}

bool ChkFile(string file_name)
{
	ifstream file; 

	file.open(file_name.c_str());
	
	if (file.good() == true)
	{
		return true; 
	}

	else
	{
		return false;
	}

}

bool IsDirectory(string file)
{
	DWORD f_attributes; 

	f_attributes = GetFileAttributesA(file.c_str()); 

	if (f_attributes == FILE_ATTRIBUTE_DIRECTORY)
	{
		return true; 
	}

	else
	{
		return false; 
	}
}

void CreateBATFile(string path)
{
	FILE* f_ptr = NULL;

	fopen_s(&f_ptr, path.c_str(), "a");

	if (f_ptr != NULL)
	{
		fclose(f_ptr);
	}
}

bool IsBatFile(string path)
{
	int path_sz = path.size();
	char* read_bytes = new char[6];
	ifstream bat_f;

	read_bytes[0] = '\0';
	read_bytes[1] = '\0';
	read_bytes[2] = '\0';
	read_bytes[3] = '\0';
	read_bytes[4] = '\0';
	read_bytes[5] = '\0';

	//batarc
	if (path[path_sz - 1] == 'c' && path[path_sz - 2] == 'r' && path[path_sz - 3] == 'a' && path[path_sz - 4] == 't' &&
		path[path_sz - 5] == 'a' && path[path_sz - 6] == 'b' && path[path_sz - 7] == '.')
	{
		// check one complete 
		bat_f.open(path);

		if (bat_f.good() == true)
		{
			bat_f.close(); 
			return true;
		}

		else
		{
			bat_f.close();
			return false;
		}
	}

	else
	{
		bat_f.close();
		return false;
	}
}

bool GetBatFileInfo(string _file, vector <Bat_FileProperties>& bat_files)
{
	// these are for reading the contents 
	int chunk_size = 5; // SIZE OF THE SIGNATURE CHUNK!! NEEDS TO BE UPDATED!!!
	int buff_sz = 256; // Currently set to 256 bytes
	char* buff = new char[buff_sz];
	char tempchar;
	int file_namesz = 0;
	int cur_ref = 0;
	bool flag = true;
	bool pro_val; // Don't touch
	bool return_val = 0;
	short damn_temp = 0;

	ifstream file;
	ifstream::pos_type file_size = _GetFileSize(_file) ; 

	Bat_FileProperties holder;
	string file_properties;

	if (IsBatFile(_file))
	{
		if (file_size < 8)
		{
			PostError("The BAT file is empty!"); 
			return_val = 0;
			return return_val;
		}

		else
		{
			return_val = 1;
			file.open(_file.c_str());
			file.seekg(ios::beg, 0);
			file.read(buff, chunk_size); // size of signature chunk
			buff[5] = '\0';

			if (!strcmp(buff, (const char*)"%BAT%"))
			{
				while (flag == true)
				{
					holder.filebeg_pos = file.tellg();

					// FNSZ chunk
					file.read(buff, 3);
					buff[3] = '\0';
					RemoveHexTempChar_(buff);
					file_namesz = HexToDec(buff);

					// FN chunk
					file.read(buff, file_namesz);
					buff[file_namesz] = '\0';
					holder.filename = buff;
					//file_name = buff;

					//FSZ chunk
					file.read(buff, 11);
					buff[11] = '\0';
					RemoveHexTempChar_(buff);
					holder.file_size = HexToDec(buff);
					//file_size = HexToDec(buff);

					// PRO chunk
					file.read(buff, 8);
					buff[8] = '\0';
					file_properties = buff;

					holder.dev = file_properties[0] - '0';
					holder.sys = file_properties[1] - '0';
					holder.vir = file_properties[2] - '0';
					holder.temp = file_properties[3] - '0';
					holder.hid = file_properties[4] - '0';
					holder.encr = file_properties[5] - '0';
					holder.ron = file_properties[6] - '0';
					holder.nor = file_properties[7] - '0';

					// CRC32 (int32_t -> std::hex)
					file.read(buff, 10);
					buff[10] = '\0';
					RemoveHexTempChar_(buff);
					holder.crc = buff;

					holder.file_pos = file.tellg();
					file.seekg((int)holder.file_size, ios::cur);
					holder.fileend_pos = file.tellg();
					bat_files.push_back(holder);
					//file.seekg((int)holder.file_size, ios::cur);

					//tempchar = file.get();

					if (file.tellg() > file_size || file.tellg() == file_size)
					{
						//file.seekg(-2, ios::cur);
						flag = false;
						file.close();
					}

					else
					{
						//cout << file.tellg() << endl;
						cur_ref++;
					}

					damn_temp++;
				}
			}

			else
			{
				return_val = 0; 
				return return_val;
			}
		}

		file.close();
	}

	else
	{
		PostError("Not a valid BAT file",true); 
		file.close();	
	}

	return return_val;
}


void RemoveHexTempChar_(char* str_a) // This overload is only to be used by the ExtractFile() header and implementation
{
	string str = str_a;
	int temp1 = str.size();

	for (int i = 0; i < temp1; i++)
	{
		str.erase(std::find(str.begin(), str.end(), '&'));
	}

	for (int i = 0; i < str.size(); i++)
	{
		str_a[i] = str[i];
	}

	str_a[str.size()] = '\0';
}

void RemoveHexTempChar_(string& str)
{
	for (int i = 0; i < str.size(); i++)
	{
		str.erase(std::find(str.begin(), str.end(), '&'));
	}
}

void CustomFileCopy(string _filesrc, string _filedest, ifstream::pos_type start, ifstream::pos_type end, bool offset_dest)
{
	// offset 1 = end, 0 = beginning

	long copy_sz = CPY_SPEED; // amount of bytes to copy at once
	int loop_times = 0;
	int last_bytes = 0;
	char* buff = new char[copy_sz];
	ifstream::pos_type total_sz = (end - start);
	ifstream f_src;
	ofstream f_dest;

	f_src.open(_filesrc, ios::binary | ios::in);
	f_dest.open(_filedest, ios::binary | ios::app | ios::out);

	if (f_src.good() == true && f_dest.good() == true && f_src.is_open() == true && f_dest.is_open())
	{
		f_src.seekg(start);

		if (offset_dest == 1)
		{
			f_dest.seekp(0, ios::end);
		}

		else
		{
			f_dest.seekp(0, ios::beg);
		}

		if (total_sz <= copy_sz)
		{
			f_src.read(buff, total_sz);
			buff[total_sz] = '\0';
			f_dest.write(buff, total_sz);
		}

		else
		{
			loop_times = total_sz / copy_sz;
			last_bytes = total_sz % copy_sz;

			for (int i = 0; i < loop_times; i++)
			{
				f_src.read(buff, copy_sz);
				f_dest.write(buff, copy_sz);
			}

			if (last_bytes > 0)
			{
				f_src.read(buff, last_bytes);
				f_dest.write(buff, last_bytes);
			}
		}
	}

	else
	{
		delete[] buff;
		f_src.close();
		f_dest.close();

		return ;
		// post an error here 
	}

	delete[] buff;
	f_src.close();
	f_dest.close(); 
}

void CreateFileIfDoesNotExist(string filename)
{
	// TODO : Should you ask the user whether he/she wants to overwrite the file?

	FILE* _tcreate;
	string err_txt = "Could not create ";

	fopen_s(&_tcreate, filename.c_str(), "w+");

	if (_tcreate != NULL)
	{
		fclose(_tcreate);
	}

	else
	{
		err_txt = err_txt + filename; 
		PostError(err_txt); 
	}

}

/* // This version takes up too much ram (as it allocates ram as large as the file size)
string GenerateCRC(string _bat_file)
{
	ifstream bat_file;
	streampos sz = _GetFileSize(_bat_file);
	uint32_t crc = 0;
	string return_val;
	int seekpos = 0;
	char* contents = new char[sz];

	bat_file.open(_bat_file.c_str(), ios::binary | ios::in);

	bat_file.seekg(seekpos, ios::beg);
	bat_file.read(contents, sz);
	crc = CRC::Calculate(contents, sz, CRC::CRC_32());
	delete[] contents;

	return_val = GetHexString(crc);
	bat_file.close();
	return return_val;
}*/

string GenerateCRC_Custom(string _file, ifstream::pos_type start, ifstream::pos_type end) // A much more efficient version. Define the "CPY_SPEED" macro to modify ram allocation size (BatLogic.h)
{
	ifstream file;
	uint32_t crc;
	string retval;

	if (end < start)
	{
		PostError("File start position and end position in file possibly corrupted !"); 
	}

	long long size = abs(end - start) ;
	long speed = CPY_SPEED;
	int loop_times;
	int last_bytes;
	char* buff;

	file.open(_file, ios::binary | ios::in);

	if (file.good() && file.is_open())
	{
		file.seekg(start); 

		if (size <= speed)
		{
			buff = new char[size + 64];
			file.read(buff, size);
			buff[size] = '\0';
			crc = CRC::Calculate(buff, size, CRC::CRC_32());
			retval = GetHexString(crc);

			return retval;
		}

		else
		{
			buff = new char[speed + 1];
			loop_times = size / speed;
			loop_times = loop_times - 1;
			last_bytes = size % speed;

			file.read(buff, speed);
			crc = CRC::Calculate(buff, speed, CRC::CRC_32());

			for (int i = 0; i < loop_times; i++)
			{
				file.read(buff, speed);
				crc = CRC::Calculate(buff, speed, CRC::CRC_32(), crc);
			}

			file.read(buff, last_bytes);
			crc = CRC::Calculate(buff, last_bytes, CRC::CRC_32(), crc);
			retval = GetHexString(crc);
		}

		delete[] buff;
	}

	return retval;
}

string GenerateCRC(string _file) // A much more efficient version. Define the "CPY_SPEED" macro to modify ram allocation size (BatLogic.h)
{
	ifstream file;
	uint32_t crc;
	string retval;
	long long file_sz = _GetFileSize(_file); 
	long speed = CPY_SPEED;
	int loop_times;
	int last_bytes;
	char* buff;


	file.open(_file, ios::binary | ios::in);

	if (file.good() && file.is_open())
	{
		if (file_sz <= speed)
		{
			buff = new char[file_sz + 64];
			file.read(buff, file_sz);
			buff[file_sz] = '\0';
			crc = CRC::Calculate(buff, file_sz, CRC::CRC_32());
			retval = GetHexString(crc);

			return retval;
		}

		else
		{
			buff = new char[speed + 1];
			loop_times = file_sz / speed;
			loop_times = loop_times - 1;
			last_bytes = file_sz % speed;

			file.read(buff, speed);
			crc = CRC::Calculate(buff, speed, CRC::CRC_32());

			for (int i = 0; i < loop_times; i++)
			{
				file.read(buff, speed);
				crc = CRC::Calculate(buff, speed, CRC::CRC_32(), crc);
			}

			file.read(buff, last_bytes);
			crc = CRC::Calculate(buff, last_bytes, CRC::CRC_32(), crc);
			retval = GetHexString(crc);
		}

		delete[] buff;
	}

	return retval;
}


string ExtendString(string str, int ex_amt, string ex_c)
{
	/*
	ExtendString() is used to extend a stirng to a particular size by adding a particular character to the beginning of a string
	Example : if ex_amt = 4, ex_c = '0', str = "9"
	then this function will return "0009"
	*/

	int temp = ex_amt - str.size();
	//ex_amt = abs(ex_amt - str.size()); 
	ex_amt = abs(temp);

	for (int i = 0; i < ex_amt; i++)
	{
		str.insert(0, ex_c);
	}

	return str;
}

void PostError(string text, bool fatal)
{
	if (fatal == true)
	{
		cout << "\nFATAL ERROR! : " << text << "\nEXITING!"; 
		cout << " PRESS ENTER TO CONTINUE TO EXIT!"; 
		cin.get();
		exit(0); 
 	}
	
	cout << "\nERROR : " << text; 

}

// END