#pragma once
#define _HAS_STD_BYTE 0


#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Macros.h"

using namespace std; 

/* Syntax for various operations
	VIEW 
	\v ARCHIVE

	DELETE
	\d ARCHIVE FILE_NAME

	WRITE FUNCTIONS 
		WOF (Write one file)  = \WOF ARCHIVE FILE_DIR
		WAD (Write all files from directory) = \WAD ARCHIVE DIRECTORY
		WAL (Write all files from a list) = \WAL ARCHIVE FILE_LIST

	EXTRACT FUNCTIONS
		
		EOF (Extract one file)  = \EOF ARCHIVE FILE_NAME DIR
		EAF (Extract all files from an archive) = \EAF ARCHIVE DIR
		EAL (Extract all files from a list) = \EAL ARCHIVE FILE_LIST
*/


// All the text in caps signifies that that word indicates a letter in the operation 
// eg : the operation d stands for delete. You may observe that 'delete' in the comment next to it is in caps. That means that the operation d 
// stands for delete

enum  // bat operations
{
	// Misc Functions
	op_v, // VIEW
	op_d, // DELETE a particular file from an archive


	// Write Functions 
	op_wof, op_wad, op_wal, // WRITE ONE FILE to the archive, WRITE ALL FILES from dir, WRITE ALL files from LIST
	//wofe, wade, wale // WRITE ONE FILE to the archive & ENCRYPT it, WRITE ALL FILES from dir & ENCRYPT, WRITE ALL files from LIST & ENCRYPT

	// Extract Functions
	op_eof, op_eaf, op_eal // EXTRACT ONE FILE from the archive, EXTRACT ALL the FILES from an archive, EXTRACT ALL files from a list 
};

// Operation structures
// Misc Operations

enum
{
	fs_NTFS, 
	fs_FAT32, 
	fs_EXFAT
};

class bat_Operation
{
	public : 
	
	bool can_extract = false ; // ? 
	bool o_write = false; // Needs to add files to an already existing bat archive (i.e : The First Chunk exists) 
	bool write_chk = false; // ? 
	bool extract_chk = false; // ? 
	string archive ; // The archive which is being referred to  
	string file ; // For the operation that writes a single file to the archive
	string dir ; // For the operation that writes all the files from a directory
	vector <string> file_list ; // For the operation that writes all the files listed in the arguments passed to cmd itself
	int operation = -1; 
} ;

struct Bat_FileProperties // File Properties    
{
	/*Links : 
		https://docs.microsoft.com/en-us/windows/win32/fileio/file-attribute-constants
		https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfileattributesa
		https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-setfileattributesa
	*/
	
	bool dev;	 // FILE_ATTRIBUTE_DEVICE
	bool sys;	 // FILE_ATTRIBUTE_SYSTEM
	bool vir;    // FILE_ATTRIBUTE_VIRTUAL
	bool temp;	 // FILE_ATTRIBUTE_TEMPORARY
	bool hid;    // FILE_ATTRIBUTE_HIDDEN
	bool encr;   // FILE_ATTRIBUTE_ENCRYPTED
	bool ron;    // FILE_ATTRIBUTE_READONLY
	bool nor;    // FILE_ATTRIBUTE_NORMAL

	//bool dir;    // FILE_ATTRIBUTE_DIRECTORY

	string filename; 
	string crc; 
	long long file_size;
	ifstream::pos_type file_pos; // the beginning of the file contents
	ifstream::pos_type filebeg_pos; // the beginning of the file chunks (fsz, pro) 
	ifstream::pos_type fileend_pos; // The end of the file contents 
};


