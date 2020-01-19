#include "Execute.h"

void ExecuteCommands(bat_Operation bat_op)
{
	if (bat_op.operation == op_wof || bat_op.operation == op_wad || bat_op.operation == op_wal) // Write operations
	{
		WriteFile(bat_op); // I made this function earlier. That is why I am not directly calling the sub functions or whatever 
	}

	else if (bat_op.operation == op_eof)
	{
		ExtractFile(bat_op.file, bat_op.archive, bat_op.dir);
	}

	else if (bat_op.operation == op_eal)
	{
		ExtractAllFilesFromList(bat_op.archive, bat_op.dir, bat_op.file_list);
	}

	else if (bat_op.operation == op_eaf)
	{
		ExtractAllFiles(bat_op.archive, bat_op.dir);
	}

	else if (bat_op.operation == op_v)
	{
		ViewBatFile(bat_op.archive);
	}

	else if (bat_op.operation == op_d)
	{
		DeleteFileFromArchive(bat_op.archive, bat_op.file); 
		//UpdateCRCInFile(bat_op.archive); // Update the CRC in the BAT File
	}

	else
	{
		PostError("The operation specified was not found"); 
	}

}