#pragma once
#define _HAS_STD_BYTE 0

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std; 

std::string GetHexString(long long integer); 
std::ifstream::pos_type _GetFileSize(string filename);
long HexToDec(string hex); 