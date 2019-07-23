#ifndef _COMMIT_CMD_H
#define _COMMIT_CMD_H

#include "../add_command/add.h"
#include "../refs/refs.h"
#include "../commit/commit.h"
#include "../misc/misc.h"

enum add_staged_files_returns{
	ASF_SUCCESS = 0,
	EASF_BUG,
	EASF_FAIL,
	EASF_STAGECORR,
	EASF_NOTHING
};

int add_staged_files( TREE *tree );

enum default_commit_returns{
	COMMIT_FAILURE = -1,
	COMMIT_SUCCESS
};

int DEFAULT_COMMIT( std::string message );

#endif