#include "misc.h"

int get_current_tree( TREE& tree )
{
	std::string branch,hash;
	int ret;
	commit cobj;

	ret = get_current_commit( cobj );
	if( ret == -EGCC_COMMIT_FAIL )
		return -EGCT_COMMIT_FAIL;
	else if( ret == -EGCC_EMPTY_BRANCH )
		return GCT_SUCCESS;

	ret = cobj.get_tree( hash );
	tree.open_tree( hash );
	return GCT_SUCCESS;
}

int get_current_commit( commit& com )
{
	std::string branch,hash;
	int ret;

	branch = getHEAD();
	if( ! branch.empty() ){

		if( branch.size() == 40 ){
			hash = branch;
		}
		else{
			hash = read_branch( branch );		
		}
		ret = com.open_commit(hash);
		com.cat();
		if( com.is_opened() == false ){
			return -EGCC_COMMIT_FAIL;
		}
		return GCC_SUCCESS;
	}
	return -EGCC_EMPTY_BRANCH;
}

int get_tree( TREE& tree, std::string& commit_hash )
{
	std::string branch,hash;
	int ret;
	commit com;

	ret = com.open_commit(commit_hash);
	// com.cat();
	if( com.is_opened() == false ){
		return -EGT_COMMIT_FAIL;
	}

	ret = com.get_tree( hash );
	tree.open_tree( hash );
	return GT_SUCCESS;

}