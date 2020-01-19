/* Project Details
Author : Samuel Wesley Rasquinha
Email : samuelrasquinha@gmail.com
Project Name : Basic Archiving Tool (BAT)
Description : A Basic tool for archiving and extracting files 
Date Started : 24-6-2019
*/

/*
	https://www.convertunits.com/from/GB/to/byte

	FAT32
	4294967294

	NTFS
	17179869184

*/

/* Plan

*/

/*Command Prompt Format
	bat.exe *bat_file* *operation* *operation arguments*
*/

/* Todo
	- Write one file to an archive  ✓
	- Append a file to an archive   ✓
	- Write multiple files from a list  ✓
	- Write multiple files from a directory  ✓
	- View an archive  ✓
	- Delete a file from an archive  ✓
	- Extract one file from an archive ✓
	- Extract all files from an archive ✓
	- Add CRC Integrity check ✓
	- Find a more efficient way of generating CRC(s) ✓✓
	- Support unicode files (UTF8) (UTF16) (UTF32) ✓✓✓
	- CreateFileIfDoesNotExist() Should you ask the user if he/she wants to overwrite the file?!
	- Add support for ParseCMDArguments() and check   ✓
		Update? ✓
	- Multiple threads? -_-
	- Add error returns and a debug windows (IMPORTANT)  ✓
		- If needed save the debug data to a file. ✓
	- Make sure to only show necessary output on the CMD Window ✓
	- Add "Author and Email" details to projects ✓
	- Proper data should be displayed for the view function ✓
	- Proper testing ✓
	*-* After adding all of the above, Only then are you ready to upload this to github
*/

/* Bugs

*/

// BAT v01


#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <Windows.h>

#include "Structures.h"
#include "BatLogic.h"
#include "Execute.h"

using namespace std; 

int main(int argc, char** argv)
{
	/* Test Commands for debugging
 		 \WOF C:\Users\samue\Desktop\archive.batarc C:\Users\samue\Desktop\2.txt
		 \EOF C:\Users\samue\Desktop\archive.batarc 2.txt C:\Users\samue\Desktop
	*/

	cout << "Welcome to BAT V01 (Basic Archiving Tool Version 1.0.0) By Samuel Rasquinha (samuelrasquinha@gmail.com)"; 

	// Display argument information
	cout << 
		"\n\nTO VIEW A BAT FILE :\n\t\\V ARCHIVE" << "\n\nTO DELETE A PARTICULAR FILE FROM A BAT FILE:\n\t\\D ARCHIVE FILENAME" << 
		"\n\nWRITE FUNCTIONS :\n\tWrite a single file = \\WOF ARCHIVE FILE_PATH\n\tWrite all files from directory = \\WAD ARCHIVE DIRECTORY\n\tWrite all files from a list into a .bat file = \\WAL ARCHIVE FILE_1 FILE_2 ..." 
		<<"\n\nEXTRACT FUNCTIONS\n\tExtract a single file = \\EOF ARCHIVE FILENAME DIR\n\tExtract all files from an archive = \\EAF ARCHIVE DIR \n\tExtract specified files provided in a list = \\EAL ARCHIVE FILE_1 FILE_2 ..." ; 

	cout << "\n\nERRORS / MESSAGES :   \n\n"; 
		 
	bat_Operation op = ParseCMDArg(argc,argv) ;
	ExecuteCommands(op);

	cout << "\n\nAll the commands have been executed! Press enter to exit!\n";
	cin.get(); 
}
