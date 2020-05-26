#pragma once
#define _HAS_STD_BYTE 0


#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <filesystem>


#include <cstdlib> 

#include "CRC.h"
#include "BatLogic.h"
#include "Functions.h"
#include "Structures.h"

using namespace std;

int WriteFile(bat_Operation bat_op);
int _WriteFile(string file_a, string file_f);
string GetFSIG();
string GetFNSZ(string file_name);
string GetFLAB(string file_name);
string GetFSZ(string file_path);
string GetPRO(string file_path);
bool WriteFCON(string src, string dest, bool pos);
void GetFileListFromDir(string path, vector <string> &vec);
long long GetFreeSpaceOnVolume(string dir);
int GetFileSystemDrive(string _drive);
bool WriteSpaceCheck(vector <string> files, string _archive);
bool WriteSpaceCheck(string file, string _archive, bool dir);
