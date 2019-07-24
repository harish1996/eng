#include "misc.h"

int get_current_tree( TREE& tree )
{
	std::string branch,hash;
	int ret;
	commit cobj;

	ret = get_current_commit( cobj );
	if( ret == -EGCC_COMMIT_FAIL || ret == -EGCC_GETHEAD_FAIL )
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
	char head_type;
	int ret;

	ret = getHEAD( head_type, branch );
	if( ret != GETHEAD_SUCCESS ) {
		return -EGCC_GETHEAD_FAIL;
	}
	if( ! branch.empty() ) {

		if( 1 == head_type )
			hash = branch;
		else
			hash = read_branch( branch );

		ret = com.open_commit(hash);
		if( ret != 0 )
			return -EGCC_COMMIT_FAIL;
		// com.cat();
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
	if( com.is_opened() == false ){
		return -EGT_COMMIT_FAIL;
	}

	ret = com.get_tree( hash );
	tree.open_tree( hash );
	return GT_SUCCESS;

}


int DEFAULT_CAT( std::string hash )
{
	OBJ obj;
	obj.get_new_object( hash );
	int type = obj.object_type();
	switch(type){
		case BLOB_OBJECT:{
			obj.cat_blob_object();
			return CAT_SUCCESS;
			break;
		}
		case TREE_OBJECT:{
			obj.discard_object();
			TREE tree;
			tree.open_tree( hash );
			tree.cat();
			return CAT_SUCCESS;
			break;
		}
		case COMMIT_OBJECT:{
			obj.discard_object();
			commit com( hash );
			com.cat();
			return CAT_SUCCESS;
			break;
		}
		default:{
			return CAT_FAILURE;
		}
	}
}
