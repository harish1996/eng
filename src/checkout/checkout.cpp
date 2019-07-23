#include "checkout.h"

/****************** TEMP FUNCTIONS
 *** functions that will be used when the actual functions are not ready
 */
#define T_OBJECT "78bffd68347d60004e9738c7c5955c0f9b0ab008"


static int get_current_tree( TREE* tree /*, Commit object */  )
{
	// int tmp = get_current_commit( /* Commit object */ );
	/* Get tree of the corresponding commit */
	tree->open_tree(T_OBJECT);
	return 0;
}

static int get_current_commit( /* Commit reference goes here */ )
{
	return 0;
}


/****************** END ************/


int _restore_tree( std::string hash, std::string prefix )
{
	TREE new_tree;
	TREE old_tree;
	get_current_tree( &old_tree );
	new_tree.open_tree( hash );
	return _restore_tree( &new_tree, &old_tree, prefix );
}

enum restore_returns{
	UPD_SUCCESS = 0,
	EUPD_BLOB_OBJMISSING,
	EUPD_BLOB_OBJCORRUPT,
	EUPD_BLOB_BUG,
	EUPD_BLOB_CANT_REMOVE,
	EUPD_BLOB_CANT_MKDIR
};

int _update_blob( TREE* tree, std::string filename, struct entry *node )
{
	int ret;
	std::string hash;

	// Try to get the same file from the other tree. 
	ret = tree->get_blob( filename, hash );
	switch(ret){
		// If the same file is present
		case GET_SUCCESS:
			// Leave it as such, if the hash is same as the old hash
			if( hash == node->hash )
				return UPD_SUCCESS;
			// Replace the old file with the new file ( Generate the new file )
			else{
				OBJ obj;
				ret = obj.create_file_from_blob( hash, filename );
				switch(ret){
					case CFB_SUCCESS:
						return UPD_SUCCESS;
					case -ECFB_NOOBJ:
						return -EUPD_BLOB_OBJMISSING;
					case -ECFB_INVOBJ:
						return -EUPD_BLOB_OBJCORRUPT;
					case -ECFB_NOTBLOB:
					case -ECFB_BUG:
						return -EUPD_BLOB_BUG;
				}
			}
			break;
		// If there is no such file with the same name in the new tree, delete
		// the file from the current folder.
		case -EGET_NO_OBJECT:
			ret = unlink( filename.c_str() );
			if( ret )
				return -EUPD_BLOB_CANT_REMOVE;
			break;

		// If there is no such file with same name in the new tree, but there is a
		// folder with the same name. Remove the entire tree.
		case -EGET_TYPE:
			system(((std::string)(" rm -r ")+filename).c_str());
			return UPD_SUCCESS;
			break;
	}
	return UPD_SUCCESS;
}

int _update_tree( TREE *old_tree, TREE *new_tree, std::string name, struct entry *node )
{
	int ret;
	std::string hash;
	// Get the same subtree from the new tree
	ret = new_tree->get_subtree( name, hash );
	switch(ret){
		// If the subtree is present, restore the tree. ( Recurse ).
		case GET_SUCCESS:
			ret = _restore_tree( old_tree, new_tree, name );
			return ret;

		// If there is no tree with the same name, remove entire tree.
		case -EGET_NO_OBJECT:
			system(((std::string)(" rm -r ")+name).c_str());
			return UPD_SUCCESS;
			break;

		// If there is no such folder with same name in the new tree, but there is a
		// file with the same name. Remove the entire tree.
		case -EGET_TYPE:
			ret = unlink( name.c_str() );
			if( ret )
				return -EUPD_BLOB_CANT_REMOVE;
			return UPD_SUCCESS;
	}
	return UPD_SUCCESS;
}

int _try_addnew_blob( TREE *old_tree, TREE *new_tree, std::string filename, struct entry *node )
{
	std::string hash;
	int ret = old_tree->get_blob( filename, hash );
	switch(ret){
		case GET_SUCCESS:
			return UPD_SUCCESS;
		case -EGET_TYPE:
			return UPD_SUCCESS;
		case -EGET_NO_OBJECT:
			OBJ obj;
			ret = obj.create_file_from_blob( node->hash, filename );
			switch(ret){
				case CFB_SUCCESS:
					return UPD_SUCCESS;
				case -ECFB_NOOBJ:
					return -EUPD_BLOB_OBJMISSING;
				case -ECFB_INVOBJ:
					return -EUPD_BLOB_OBJCORRUPT;
				case -ECFB_NOTBLOB:
				case -ECFB_BUG:
					return -EUPD_BLOB_BUG;
			}
	}
	return UPD_SUCCESS;
}

int _try_addnew_tree( TREE *old_tree, TREE *new_tree, std::string filename, struct entry *node )
{
	std::string hash;
	int ret = old_tree->get_subtree( filename, hash );
	switch(ret){
		case GET_SUCCESS:
			return UPD_SUCCESS;
		case -EGET_NO_OBJECT:
			ret = new_tree->tree_to_fs( filename );
			switch(ret){
				case TREE_TO_FS_SUCCESS:
					return UPD_SUCCESS;
				case -ETREE_TO_FS_BUG:
					return -EUPD_BLOB_BUG;
				case -ETREE_TO_FS_CORRUPT:
					return -EUPD_BLOB_OBJCORRUPT;
				case -ETREE_TO_FS_MKDIR_FAIL:
					return -EUPD_BLOB_CANT_MKDIR;
			}
		case -EGET_TYPE:
			return UPD_SUCCESS;

	}
	return UPD_SUCCESS;
}


int _restore_tree( TREE *new_tree, TREE *old_tree, std::string prefix )
{
	// TREE old_tree;
	// tmp = get_current_tree( &old_tree );
	std::string hash;
	int ret;

	std::map<std::string,struct entry>::const_iterator old_begin,old_end,new_begin,new_end;

	if( prefix.empty() ){
		old_begin = old_tree->cbegin();
		old_end = old_tree->cend();
		new_begin = new_tree->cbegin();
		new_end = new_tree->cend();
		prefix = "";
	}
	else{
		old_begin = old_tree->cbegin( prefix );
		old_end = old_tree->cend( prefix );
		new_begin = new_tree->cbegin( prefix );
		new_end = new_tree->cend( prefix );
		prefix += "/";
	}

	// Loop through all files in the old tree	
	for( ; old_begin != old_end; old_begin++ ){

		struct entry *node = ( struct entry *)&old_begin->second;
		std::string name = prefix + old_begin->first;
		
		// If the entry is a regular file
		if( TYPE(node->type) == BLOB_OBJ )
			ret = _update_blob( new_tree, name, node);
		
		// If the entry is a tree
		else
			ret = _update_tree( old_tree, new_tree, name, node );

		switch(ret){
			case UPD_SUCCESS:
				continue;
			default:
				return ret;
		}
	}

	// Loop through all the files in the new tree to create any new files
	for( ; new_begin != new_end; new_begin++ ){

		struct entry *node = ( struct entry *)&new_begin->second;
		std::string name = prefix + new_begin->first;
		
		// If the entry is a regular file
		if( TYPE(node->type) == BLOB_OBJ )
			ret = _try_addnew_blob( old_tree, new_tree, name, node);
		
		// If the entry is a tree
		else
			ret = _try_addnew_tree( old_tree, new_tree, name, node );

		switch(ret){
			case UPD_SUCCESS:
				continue;
			default:
				return ret;
		}
	}	

	return UPD_SUCCESS;
}
