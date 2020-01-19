#include "Functions.h"

using namespace std; 

string GetHexString(long long integer)
{
	stringstream stream;
	stream << std::hex << integer;
	string result(stream.str());
	stream.clear(); 
	return result; 
}

long HexToDec(string hex)
{
	char* p;
	long retval = strtol(hex.c_str(), &p, 16);

	return retval; 	
}

std::ifstream::pos_type _GetFileSize (string filename)
{
	std::ifstream in(filename.c_str(), std::ifstream::ate | std::ifstream::binary);
	std::ifstream::pos_type ret = in.tellg(); 
	in.close();
	return ret;
}