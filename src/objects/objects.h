#ifndef _OBJECTS_H
#define _OBJECTS_H

#include <libs/gzstream/gzstream.h>
#include <iostream>
#include <libs/sha1/sha1.h>
#include <string>
#include <cstdlib>
#include <sstream>

/* Codes for the first byte identifier */
#define BLOB_OBJECT 0
#define COMMIT_OBJECT 1
#define TREE_OBJECT 2

class OBJ : public SHA1
{
	// Variable to store the hash of the open object
	std::string hash;

	//
	// @func private copy_file
	// @brief Copies the content of the input stream to the output compressed
	// 	file
	//
	// @param ofile The ogzstream instance of the output object file
	// @param s The input stream 
	//
	// @return 0 on success , -1 if file not opened in the ogzstream instance
	//
	// @bug if a stream of text is written to the file and if cat_blob_object is
	// 	requested, objstream will be empty. So it will fail
	int copy_file( ogzstream &ofile, std::istream &s );

	// Type of the object
	char type;

	// The input stream for the object
	igzstream objstream;

public:
	// @func OBJ
	// @brief The default constructor
	OBJ(): type(-1){};

	// @func create_object
	// @brief Creates the object with an input stream and a given type
	//
	// @param s The istream instance which contains the contents to be written inside
	// 	the object ( Use isstream ( input string stream ) incase of strings that needs
	// 	to be written inside this.
	// @param type The type of the object to be written ( 0 for blob, 1 for commit, 2 for tree )
	//
	// @return 0 on success, -1 if invalid type, +2 if object already exists. 
	int create_object( std::istream &s, char type );

	// @func create_blob_object 
	// @brief Creates a blob object using a given file
	//
	// @param filename Filename of the file
	//
	// @return 0 on success, -2 if file specified doesn't exist 
	int create_blob_object( const std::string filename );

	// @func get_hash
	// @brief Gets the hash value of the open object
	//
	// @return string with the hash value ( Convert to a set of integers using strtoq )
	//
	// @todo return a integer vector or a custom HASH type
	std::string get_hash();

	// @func get_new_object
	// @brief Gets a new object specified by the hash, discarding the previous one.
	//
	// @param hash The hash value of the object
	//
	// @return 1 on success, -1 if error opening the specified object, -2 if not a valid
	// 	object
	//
	// @todo accept a integer vector or a custom HASH type
	int get_new_object( const std::string &hash );

	// @func discard_object
	// @brief Discards the open/created object.
	//
	// @return 1 on success
	int discard_object();

	// @func read_object
	// @brief Reads the contents of the object.
	//
	// @param buf The character buffer to which the contents are to be stored.
	// @param size Size of the request.
	//
	// @return no. of bytes read.
	//
	// @bug on multiple reads, if file becomes empty on the last read perfectly ( i.e.
	// the last read size was equal to the remaining size on the object ), the user wouldn't
	// know the object is empty and also the next read would fail, giving out a misleading
	// number in the next call, so going on an infinte loop.
	int read_object( char* buf, size_t size );

	// @func object_type
	// @brief Returns the type of the opened/created object currently held by
	// 	the instance
	//
	// @return the type of the object
	int object_type();

	// @func cat_blob_object
	// @brief Prints the blob object as such
	//
	// @return -3 if open object is not a blob, -1 if no valid object is open,
	// 	1 on success
	int cat_blob_object( );
};

#endif
