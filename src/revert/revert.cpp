#include "revert.h"

int DEFAULT_REVERT(const std::string &commit_hash)
{
	std::string branch;
	char head_type;
	int ret;

	ret = getHEAD(head_type, branch);
	if(ret < 0)
		return -EDR_NO_FILE;
	if(0 != head_type || branch.empty())
		return -EDR_INVALID_BRANCH;

	ret = _restore_tree( commit_hash, "" );
	if( ret != UPD_SUCCESS )
		return -EDR_RESTORE_TREE_FAILURE;
	ret = write_branch( commit_hash, branch );
	if( ret != 0 ) {
		DEFAULT_CHECKOUT(branch);
		return -EDR_WRITE_BRANCH_FAILURE;
	}
	return REVERT_SUCCESS;
}