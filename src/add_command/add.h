#ifndef _ADD_H
#define _ADD_H

#include "../tree/tree.h"
#include "../objects/objects.h"
#include <iostream>
#include <string>
#include <set>
#include <ftw.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <vector>

enum try_add_returns{
	TA_SUCCESS = 0,
	ETA_GET_FAIL,
	ETA_HASH_FAIL,
};

enum try_add_tree_returns{
	TAT_SUCCESS = 0,
	ETAT_GET_FAIL
};

enum add_all_returns{
	ADDALL_SUCCESS,
	EADDALL_NOTSUPP
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

	int tree_walk(std::string filepath, TREE* tree );

	int update_wrapper( const char *fpath, const struct stat *sb, int typeflag, struct FTW *buf );

	int add_all_files( std::string filepath, TREE *tree );

	int _try_add_tree( std::string filepath, TREE* tree );

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

int DEFAULT_ADD( std::vector<std::string> );

#endif