# BAT (Basic Archiving Tool) 

![BAT V01 Logo](https://github.com/swr06/Basic_Archiving_Tool/blob/master/branding/Logo.png)

This is a basic archiving tool that can combine multiple files into a single file. Which makes transferring the archived file from one computer to another very easy. It has been tested and works fine though there might be some unseen bugs.

# Features

BAT works with all file formats and supports ANSI, UTF8, UTF16 and UTF32. 
It has CRC file integrity checks and has support for various file attributes. It is kind of similar to TAR

# Building and Libraries used

BAT is programmed in C++17 and compiled using Visual Studio 2019.
I used basic MFC and a CRC library called CRC++ by Daniel Bahr

# Command Line arguments

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
    
# About the programmer

My name is Samuel Rasquinha and I am a 13 year old programmer from India! :)

# Reporting bugs 

You can email me at samuelrasquinha@gmail.com

