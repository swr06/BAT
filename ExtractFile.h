#pragma once
#define _HAS_STD_BYTE 0

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <iomanip>
#include <fstream>
#include <filesystem>

#include <cstdlib> 

#include "Functions.h"
#include "Structures.h"
#include "BatLogic.h"
#include "Macros.h"

using namespace std;

void ExtractFile(string _filename, string _bat_file, string output_dir); 
void ExtractAllFiles(string bat_file, string _output_dir); 
void ExtractAllFilesFromList(string bat_file, string output_dir, vector <string> f_list); 