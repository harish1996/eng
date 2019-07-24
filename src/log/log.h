#ifndef _LOG_H
#define _LOG_H

#include <iostream>
#include <string>
#include "../commit/commit.h"
#include "../misc/misc.h"

enum plr_errors {
	PLR_SUCCESS = 0,
	EPLR_INVALID_COMMIT_HASH 
};

enum dpl_errors
{
	DPL_SUCCESS = 0,
	EDPL_NO_COMMIT 
};

int print_log_recurse(std::string &);
int DEFAULT_LOG();

#endif