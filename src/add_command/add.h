#ifndef _ADD_H
#define _ADD_H

#include "../tree/tree.h"
#include "../objects/objects.h"
#include <iostream>
#include <string>
#include <set>

enum add_file_to_list_returns{
	AFL_SUCCESS = 0,
	EAFL_GET_FAIL,
	EAFL_HASH_FAIL,
};

class STAGE{
private:
	std::set<std::string> staged;
	std::string staging_file;
public:
	STAGE( std::string staging_file = ".eng/TO_BE_STAGED" ){
		this->staging_file = staging_file; 
		populate();
	}

	// change_staging_file
	// @brief Changes the staging file, while keeping everything as such
	//
	// @param s_file The new staging file
	//
	// @return 0 on success
	int change_staging_file( std::string s_file );
	
	// populate
	// @brief Populates the data structures using the existing staging file
	void populate();

	// drop
	// @brief Drops the data structure and starts a fresh

	void drop();
	
	// flush
	// @brief Flushes the datastructure into the staging file
	//
	// @return 0 on success
	int flush();


	// update
	// @brief Updates the datastructure with the filepath
	// 
	// @param filepath Path to the file both in the filesystem and in the tree
	void update( std::string filepath);
	
	// dontupdate
	// @brief Placeholder for future purposes
	//
	// @param filepath Path to the file both in the filesystem and in the tree
	void dontupdate( std::string filepath);

	// try_add
	// @brief Adds a given filepath to the add list, if it is updated
	// 	with respect to `tree`
	//
	// @param filepath Path to the file both in the filesystem and in the tree
	// @param tree The current tree, which is to be used as reference
	//
	// @return
	int try_add( std::string filepath, TREE* tree );

	~STAGE(){
		flush();
		staged.clear();
		staging_file.clear();
	}
};

#endif