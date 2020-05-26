#include "View.h"

static long long max_filesz = 1099511627776;

int ViewBatFile(string bat_file)
{
	vector <Bat_FileProperties> files;

	bool res = GetBatFileInfo(bat_file, files); 
	
	if (res)
	{
		for (int i = 0; i < files.size(); i++)
		{
			cout << "\n" << files[i].filename << "  " << files[i].file_size << " BYTES " << "CRC : " << files[i].crc;
		}
	}
	
	else
	{
		PostError("The bat file is possibly empty or an unexpected error has occured!", true); 
	}

	return 1; 
}