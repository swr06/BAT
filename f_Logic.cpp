#include "f_Logic.h"

using namespace std; 

int Copy(string src, string dest)
{
	int copy_sz = 10; // Amount of bytes to copy 
	char* copy_buff = new char[copy_sz];
	long int source_sz;
	long int run_times;
	long int lastrun_sz;
	fstream source_chk;
	fstream dest_chk;
	fstream source_f;
	fstream dest_f;

	source_chk.open(src);
	dest_chk.open(dest);

	if (!source_chk.good() || !dest_chk.good())
	{
		source_chk.close();
		dest_chk.close();
		return -1;
	}

	source_chk.close();
	dest_chk.close();
	source_f.open(src);
	dest_f.open(dest);

	source_f.seekg(ios::beg, 0);
	dest_f.seekp(ios::beg, 0);

	source_sz = 60; // TO FIX!!
	run_times = source_sz / copy_sz;
	lastrun_sz = source_sz % copy_sz;

	while (run_times != 0)
	{
		source_f.read((char*)copy_buff, copy_sz);
		dest_f.write((char*)copy_buff, copy_sz);
		run_times--;
	}


	if (lastrun_sz != 0)
	{
		source_f.read((char*)copy_buff, lastrun_sz);
		dest_f.write((char*)copy_buff, lastrun_sz);
	}

	source_f.close();
	dest_f.close();
}

int GetFileSize()
{
	//To implement probably by using stat.h
}