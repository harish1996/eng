#ifndef _REVERT_H
#define _REVERT_H

#include "../refs/refs.h"
#include "../checkout/checkout.h"

enum dr_errors{
	REVERT_SUCCESS = 0,
	EDR_NO_FILE,
	EDR_INVALID_BRANCH,
	EDR_RESTORE_TREE_FAILURE,
	EDR_WRITE_BRANCH_FAILURE
};

int DEFAULT_REVERT(const std::string &commit_hash);

#endif