#include "commit_cmd.h"

int add_staged_files( TREE *tree, std::string& hash )
{
	STAGE stager;
	OBJ obj;
	auto begin = stager.cbegin();
	auto end = stager.cend();
	int tmp;
	std::string name;

	if ( begin == end )
		return -EASF_NOTHING;

	// tree->rec_cat();
	for( ; begin != end; begin++ ){
		name = begin->first;
		hash = begin->second;
		tmp = tree->recursive_add_blob( name, hash );
		// tree->rec_cat();
		switch(tmp){
			case ADD_SUCCESS:
				continue;
				break;
			case -EADD_EXIST:
				tmp = tree->modify_blob( name, hash );
				switch(tmp){
					case MOD_SUCCESS:
						continue;
						break;
					case -EMOD_INVNAME:
					case -EMOD_INVHASH:
						return -EASF_STAGECORR;
					case -EMOD_NO_OBJECT:
					case -EMOD_NO_SUBDIR:
					case -EMOD_NO_TREE:
					case -EMOD_TYPE:
					case -EMOD_NO_ENTRY:
						return -EASF_BUG;
					case -EMOD_NO_MEM:
					case -EMOD_CORRUPTED:
						return -EASF_FAIL;
				}
				break;
			case -EADD_EXISTN:
			case -EADD_INVNAME:
			case -EADD_INVHASH:
				return -EASF_STAGECORR;
				break;
			case -EADD_NO_MEM:
				return -EASF_FAIL;
				break;
		}
	}

	stager.drop();
	stager.flush();
	tree->write_tree( hash );
	return ASF_SUCCESS;
}

int DEFAULT_COMMIT( std::string message )
{
	TREE tree;
	std::string hash, old_hash, branch;
	int ret, commit_id, tmp;
	char head_type;
	commit new_cobj;

	if( message.empty() ) {
		std::cerr<<"Commit message compulsary\n";
		return -COMMIT_FAILURE;
	}

	ret = getHEAD( head_type, branch );
	if( ret != GETHEAD_SUCCESS ){
		return -COMMIT_FAILURE;
	}

	if( ! branch.empty() ) {
		if(0 == head_type)
			hash = read_branch( branch );
		else
			hash = branch;

		commit cobj(hash);
		if( cobj.is_opened() == false ){
			return COMMIT_FAILURE;
		}
		old_hash = hash;
		commit_id = 1;
		ret = cobj.get_tree( hash );
		tree.open_tree( hash );
	}
	else {
		head_type = 0;
		commit_id = 0;
		branch = "master";
	}
	
	// int tmp = get_current_commit( /* Commit obj */ );
	// tmp = get_current_tree( &tree );	
	// tree.rec_cat();
	tmp = add_staged_files( &tree, hash );
	switch(tmp) {
		case ASF_SUCCESS:
			break;
		case -EASF_STAGECORR:
			std::cerr<<"Stage corrupted"<<std::endl;
			return COMMIT_FAILURE;
		case -EASF_BUG:
			std::cerr<<"Bug in add_staged_files. Report an issue to the repository"<<std::endl;
			return COMMIT_FAILURE;
		case -EASF_FAIL:
			std::cerr<<"Commit failed.."<<std::endl;
			return COMMIT_FAILURE;
		case -EASF_NOTHING:
			std::cerr<<"Add something to commit"<<std::endl;
			return COMMIT_SUCCESS;
	}
	/* Update new commit object with old commit as parent and the new tree hash */
	/* Write the message */
	/* Create the object */

	new_cobj.write_parents( commit_id, &old_hash );
	new_cobj.write_tree( hash );
	hash = "GOD";
	new_cobj.write_author( hash );
	new_cobj.write_message( message );
	ret = new_cobj.create_commit( hash );
	if( ret == -1 ) {
		return COMMIT_FAILURE;
	}
	// std::cout<<hash;
	ret = writeHEAD(head_type, branch);
	ret = write_branch(hash, branch);

	// tree.rec_cat();

	return COMMIT_SUCCESS;
}
