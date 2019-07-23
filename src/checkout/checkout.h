#ifndef _CHECKOUT_H
#define _CHECKOUT_H

#include <iostream>
#include "../tree/tree.h"
#include "../refs/refs.h"
#include "../misc/misc.h"

enum restore_returns{
	UPD_SUCCESS = 0,
	EUPD_BLOB_OBJMISSING,
	EUPD_BLOB_OBJCORRUPT,
	EUPD_BLOB_BUG,
	EUPD_BLOB_CANT_REMOVE,
	EUPD_BLOB_CANT_MKDIR,
	EUPD_TREE_ERR
};

#define CHECKOUT_FAILURE -1
#define CHECKOUT_SUCCESS 0

int _restore_tree( std::string hash, std::string prefix );

int _update_blob( TREE* tree, std::string filename, struct entry& node );

int _update_tree( TREE *tree, std::string name, struct entry& node );

int _try_addnew_blob( TREE *old_tree, TREE *new_tree, std::string filename, struct entry& node );

int _try_addnew_tree( TREE *old_tree, TREE *new_tree, std::string filename, struct entry& node );

int _restore_tree( TREE *new_tree, TREE *old_tree, std::string prefix );

int DEFAULT_CHECKOUT( std::string str );
#endif