#pragma once
#define _HAS_STD_BYTE 0
#define CPY_SPEED 100000000 // The amount of ram it allocates while copying or generating a CRC code

#include <Windows.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <string>

#include "CRC.h"
#include "Structures.h"
#include "Macros.h"
#include "Functions.h"

using namespace std;

// This function takes the arguments passed to the command prompt as arguments 
// cargc = command prompt argc, cargv = command prompt argv
bat_Operation ParseCMDArg(int cargc, char** cargv);
bool ChkFile(string file_name);
bool IsDirectory(string file);
void CreateBATFile(string path);
bool IsBatFile(string path);
bool GetBatFileInfo(string _file, vector <Bat_FileProperties>& bat_files); 
void RemoveHexTempChar_(char* str_a); // overload 1 
void RemoveHexTempChar_(string& str); // overload 2 
void CustomFileCopy(string _filesrc, string _filedest, ifstream::pos_type start, ifstream::pos_type end, bool offset_dest); 
void CreateFileIfDoesNotExist(string filename); 
string GenerateCRC(string);
string ExtendString(string str, int ex_amt, string ex_c); 
void PostError(string text, bool fatal = false); 
string GenerateCRC_Custom(string _file, ifstream::pos_type start, ifstream::pos_type end);