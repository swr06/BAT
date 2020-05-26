#include "ExtractFile.h"

void ExtractFile(string _filename, string _bat_file, string output_dir)
{
	vector <Bat_FileProperties> bat_files;
	fstream bat_file;
	fstream temp;
	string err_txt;
	string generated_crc;
	int arr_ref = -1;

	stringstream err; 

	temp.open(output_dir.c_str());

	if (temp.good() == true) {
		temp.close();

		return;
	}

	GetBatFileInfo(_bat_file, bat_files);

	for (int i = 0; i < bat_files.size(); i++)
	{
		if (bat_files[i].filename == _filename)
		{
			arr_ref = i;
			break;
		}
	}

	if (arr_ref == -1)
	{
		err_txt = "The file ";
		err_txt = err_txt + _filename;
		err_txt = err_txt + " does not exist in the archive!";
		PostError(err_txt);

		return;
	}

	else 
	{
		auto start_crc = std::chrono::system_clock::now();
		generated_crc = GenerateCRC_Custom(_bat_file, bat_files[arr_ref].file_pos, bat_files[arr_ref].fileend_pos); 
		auto end_crc = std::chrono::system_clock::now();
		auto elapsed_crc = std::chrono::duration_cast<std::chrono::milliseconds>(end_crc - start_crc);

		if (bat_files[arr_ref].crc == generated_crc)
		{
			cout << "CRC Check for " << _filename << " took " << elapsed_crc.count() << " milliseconds  " ;
			auto start_ex = std::chrono::system_clock::now();
			output_dir = output_dir + "\\";
			output_dir = output_dir + bat_files[arr_ref].filename;
			CreateFileIfDoesNotExist(output_dir);
			CustomFileCopy(_bat_file, output_dir, bat_files[arr_ref].file_pos, bat_files[arr_ref].fileend_pos, 1);

			wstring woutput_dir(output_dir.begin(), output_dir.end()); ;

			if (bat_files[arr_ref].nor == true)
			{
				SetFileAttributes(woutput_dir.c_str(), FILE_ATTRIBUTE_NORMAL);
			}

			if (bat_files[arr_ref].dev == true)
			{
				SetFileAttributes(woutput_dir.c_str(), FILE_ATTRIBUTE_DEVICE);
			}

			if (bat_files[arr_ref].sys == true)
			{
				SetFileAttributes(woutput_dir.c_str(), FILE_ATTRIBUTE_SYSTEM);
			}

			if (bat_files[arr_ref].vir == true)
			{
				SetFileAttributes(woutput_dir.c_str(), FILE_ATTRIBUTE_VIRTUAL);
			}

			if (bat_files[arr_ref].temp == true)
			{
				SetFileAttributes(woutput_dir.c_str(), FILE_ATTRIBUTE_TEMPORARY);
			}

			if (bat_files[arr_ref].hid == true)
			{
				SetFileAttributes(woutput_dir.c_str(), FILE_ATTRIBUTE_HIDDEN);
			}

			if (bat_files[arr_ref].encr == true)
			{
				SetFileAttributes(woutput_dir.c_str(), FILE_ATTRIBUTE_ENCRYPTED);
			}

			if (bat_files[arr_ref].ron == true)
			{
				SetFileAttributes(woutput_dir.c_str(), FILE_ATTRIBUTE_READONLY);
			}

			auto end_ex = std::chrono::system_clock::now();
			auto elapsed_ex = std::chrono::duration_cast<std::chrono::milliseconds>(end_crc - start_crc);
			cout << "\nExtracting file " << _filename << " from " << _bat_file << " took " << elapsed_ex.count() << " milliseconds"; 
		}

		else
		{
			err << "The file " << _filename << " has been modified! (in the bat file) The CRC32 check has failed!";
			PostError(err.str(), true);
		}
	}

	return;
}

void ExtractAllFiles(string bat_file, string _output_dir)
{
	vector <Bat_FileProperties> bat_files;
	fstream temp;
	string output_dir; 

	temp.open(output_dir.c_str());

	if (temp.good() == true)
	{
		temp.close();
		PostError("The output dir has to be a folder and not a file!"); 

		return;
	}

	GetBatFileInfo(bat_file, bat_files);

	for (int i = 0; i < bat_files.size(); i++)
	{
		if (bat_files[i].crc == GenerateCRC_Custom(bat_file, bat_files[i].file_pos, bat_files[i].fileend_pos))
		{
			output_dir = _output_dir;
			output_dir = output_dir + "\\";
			output_dir = output_dir + bat_files[i].filename;
			CreateFileIfDoesNotExist(output_dir);
			CustomFileCopy(bat_file, output_dir, bat_files[i].file_pos, bat_files[i].fileend_pos, 1);

			wstring woutput_dir(output_dir.begin(), output_dir.end()); ;

			if (bat_files[i].nor == true)
			{
				SetFileAttributes(woutput_dir.c_str(), FILE_ATTRIBUTE_NORMAL);
			}

			if (bat_files[i].dev == true)
			{
				SetFileAttributes(woutput_dir.c_str(), FILE_ATTRIBUTE_DEVICE);
			}

			if (bat_files[i].sys == true)
			{
				SetFileAttributes(woutput_dir.c_str(), FILE_ATTRIBUTE_SYSTEM);
			}

			if (bat_files[i].vir == true)
			{
				SetFileAttributes(woutput_dir.c_str(), FILE_ATTRIBUTE_VIRTUAL);
			}

			if (bat_files[i].temp == true)
			{
				SetFileAttributes(woutput_dir.c_str(), FILE_ATTRIBUTE_TEMPORARY);
			}

			if (bat_files[i].hid == true)
			{
				SetFileAttributes(woutput_dir.c_str(), FILE_ATTRIBUTE_HIDDEN);
			}

			if (bat_files[i].encr == true)
			{
				SetFileAttributes(woutput_dir.c_str(), FILE_ATTRIBUTE_ENCRYPTED);
			}

			if (bat_files[i].ron == true)
			{
				SetFileAttributes(woutput_dir.c_str(), FILE_ATTRIBUTE_READONLY);
			}
		}

		else
		{
			cout << "\nERROR : " << "The file " << bat_files[i].filename << " has been modified or is corrupted! Hence it won't be extracted!";
		}
	}
}

void ExtractAllFilesFromList(string bat_file, string output_dir, vector <string> f_list)
{
	for (int i = 0; i < f_list.size() ; i++)
	{
		ExtractFile(f_list[i], bat_file, output_dir); 
	}
}