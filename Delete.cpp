#include "Delete.h"

void DeleteFileFromArchive(string _batf, string file_name)
{
	int arr_ref = -1;
	
	string system_command; 
	fstream new_bfile;
	string tempfile_dir; 
	string t_filename = "\\tarchive.batarc";
	FILE* _tcreate = NULL;

	size_t appdata_sz; 
	char* appdata_pth = nullptr; 

	_dupenv_s(&appdata_pth, &appdata_sz, "appdata");

	if (appdata_pth == NULL)
	{
		 return; 
		 // return an error here
	}

	tempfile_dir = appdata_pth; 
	tempfile_dir = tempfile_dir + t_filename;


	vector <Bat_FileProperties> bat_files;

	GetBatFileInfo(_batf, bat_files);

	for (int i = 0; i < bat_files.size(); i++)
	{
		if (bat_files[i].filename == file_name)
		{
			arr_ref = i;
			break;
		}
	}

	if (arr_ref == -1)
	{
		PostError("File does not exist in the archive");

		return;

		// Post an error here
	}

	else
	{
		fopen_s(&_tcreate, tempfile_dir.c_str(), "w+");

		if (_tcreate != NULL)
		{
			fclose(_tcreate);
		}

		else
		{
			PostError("Error while creating the temporary file in the %APPDATA% folder"); 
		}

		new_bfile.open(tempfile_dir);

		if (new_bfile.good() == false)
		{
			PostError("Could not create the temporary file in the %APPDATA% folder");
			return;
		}

		CustomFileCopy(_batf, tempfile_dir, 0, bat_files[arr_ref].filebeg_pos, 1);

		if ((arr_ref + 1) == bat_files.size())
		{

		}

		else if (arr_ref + 1 < bat_files.size()) 
		{
			CustomFileCopy(_batf,tempfile_dir,bat_files[arr_ref + 1].filebeg_pos, _GetFileSize(_batf.c_str()),1); 
		}
	}

	new_bfile.close();

	// This moves the file using the system() function

	system("@ECHO OFF");
	system_command = "DEL /Q "; 
	system_command = system_command + _batf.c_str(); 
	cout << system_command;
	system(system_command.c_str());
	system_command = "MOVE /Y "; 
	system_command = system_command + tempfile_dir.c_str(); 
	system_command = system_command + " ";
	system_command = system_command + _batf.c_str();
	cout << endl << system_command;
	system(system_command.c_str());
}
