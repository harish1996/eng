#ifndef _MISC_H
#define _MISC_H

#include "../refs/refs.h"
#include "../tree/tree.h"
#include "../commit/commit.h"

enum gct_errors{
	GCT_SUCCESS = 0,
	EGCT_COMMIT_FAIL
};

enum gcc_errors{
	GCC_SUCCESS = 0,
	EGCC_COMMIT_FAIL,
	EGCC_EMPTY_BRANCH
};

enum gt_errors{
	GT_SUCCESS = 0,
	EGT_COMMIT_FAIL	
};

int get_current_tree( TREE& tree );

int get_current_commit( commit& com );

int get_tree( TREE& tree, std::string& commit_hash );

#endif