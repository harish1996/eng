#include "tree.h"

int first_dirname( const std::string path, std::string &dir )
{
	std::size_t found = path.find_first_of("/");
	if( found == std::string::npos){
		return -1;
	}
	else{
		dir = path.substr(0,found);
		return found;
	}
}

int filename( const std::string path, std::string &fname )
{
	std::size_t found = path.find_last_of("/");
	if( found == std::string::npos ){
		return -1;
	}
	else{
		fname = path.substr(found+1);
		return found;
	}
}

int full_dirpath( const std::string path, std::string &pathname )
{
	std::size_t found = path.find_last_of("/");
	if( found == std::string::npos ){
		return -1;
	}
	else{
		pathname = path.substr(0,found+1);
		return found;
	}	
}

/**
 * @brief TREE::_get_local_entry
 * Gets the local entry with name `name`
 *
 * @param name Name of the local entry
 * @param entry Variable to hold the value of the entry
 * @return GET_SUCCESS on success,
 *      -EGET_NO_ENTRY if no entry with `name` exists
 */
int TREE::_get_local_entry( std::string name, struct entry* &entry )
{
        auto ret = contents.find( name );
        if( ret == contents.end() )
                return -EGET_NO_ENTRY;
        else{
                entry = &ret->second;
                return GET_SUCCESS;
        }
}

/**
 * @func _get_child_tree
 * @brief Helper function for the class to get the child TREE object ptr
 *
 * @param name Name of the tree
 * @param tree Pointer to return the TREE object
 *
 * @return GET_SUCCESS on success 
 *	-EGET_NO_ENTRY if a valid entry is not present in the current tree
 *	-EGET_NO_TREE if the object with `name` is not a tree
 *	-EGET_NO_MEM if memory not available to allocate data structure for subtree
 */
int TREE::_get_child_tree( std::string name, TREE* &tree )
{
	struct entry *tmp;
	int ret = _get_local_entry( name, tmp );
	if( ret == -EGET_NO_ENTRY )
		return -EGET_NO_ENTRY;

	return _get_child_tree( tmp, tree );
}

/**
 * @func _get_child_tree
 * @brief Helper function for the class to get the child TREE object ptr
 *
 * @param entry Entry of the corresponding subtree
 * @param tree Pointer to return the TREE object
 *
 * @return GET_SUCCESS on success 
 *	-EGET_NO_ENTRY if a valid entry is not present in the current tree
 *	-EGET_NO_TREE if the object with `name` is not a tree
 *	-EGET_NO_MEM if memory not available to allocate data structure for subtree
 */
int TREE::_get_child_tree( struct entry *entry, TREE* &tree )
{
	if( !entry )
		return -EGET_NO_ENTRY;
	if( TYPE(entry->type) != TREE_OBJ )
		return -EGET_NO_TREE;

	// Allocate memory for subtree if and only when needed. This will inturn allocate memory
	// for TREE objects below in the heirarchy
	if( entry->s_tree == NULL ){
		entry->s_tree = new subtree( entry->hash );
		if( !entry->s_tree || !entry->s_tree->sub_tree )
			return -EGET_NO_MEM;
	}

	tree = entry->s_tree->sub_tree;
	return GET_SUCCESS;
}

/**
 * @brief _get_immediate_parent_tree
 * @func Gets the immediate parent TREE of the given path of a file/directory.
 *
 * @param name The entire path of the file/directory
 * @param parent Pointer to return the TREE object
 *
 * @return GET_SUCCESS on success
 *	-EGET_NOPARENT if path contains a direct file/directory without any parent
 *	Other errors mentioned for `_get_child_tree`
 */
int TREE::__get_immediate_parent_tree( std::string dirname, TREE* &parent )
{
	std::string h,subtree,depth;
	int ret;

	int tmp = first_dirname( dirname, subtree );
	if( tmp >= 0 ){
		// If the `name` itself is in the current tree
		if( tmp == (int)dirname.size() - 1 )
			return _get_child_tree( subtree, parent );
		else{
			//struct entry *ptr;
			TREE *ptr;
			// Extract the remaining of the pathname from the fullpath `name`
			depth = dirname.substr( tmp+1, dirname.size() - tmp - 1 );
			ret = _get_child_tree( subtree, ptr );
		       	if( ret != GET_SUCCESS )
				return ret;
			return ptr->_get_immediate_parent_tree( depth, parent );
		}
	}
	else
		return -EGET_NO_PARENT;
}

int TREE::_get_immediate_parent_tree( std::string dirname, TREE* &parent )
{
	std::string dirpath;
	int tmp = full_dirpath( dirname, dirpath );

	if( tmp >= 0 ){
		return __get_immediate_parent_tree( dirpath, parent );
	}
	else
		return -EGET_NO_PARENT;
}

/**
 * @func _get_next_dir
 * @brief Gets the entry of the next directory in line, and also returns the
 * remaining path with it.
 *
 * @param name Full path from the current tree as root
 * @param parent The placeholder for the entry that is going to be returned
 * @param depth The placeholder for the returning the remaining path
 *
 * @return GET_SUCCESS on successful retrieval
 * 	-EGET_NOENTRY if the subdirectory mentioned in the path doesn't exist
 * 	-EGET_NOPARENT if the path doesn't contain a subdirectory
 */
int TREE::_get_next_dir( std::string dirname, struct entry* &parent, std::string &depth )
{
	std::string subtree;

	int tmp = first_dirname( dirname, subtree );
	if( tmp >= 0 ){
		if( tmp == (int)dirname.size() - 1 ){
			depth.clear();
			return _get_local_entry( subtree, parent );
		}
		else{
			depth = dirname.substr( tmp+1, dirname.size() - tmp - 1 );
			return _get_local_entry( subtree, parent );
		}
	}
	else
		return -EGET_NO_PARENT;
}
			
/**
 * @func _add_local_object
 * @brief Helper function for adding a object entry in the current tree
 *
 * @param name Name of the object
 * @param hash Hash of the object
 * @param type Type of the object
 *
 * @return ADD_SUCCESS on success
 * 	-EADD_EXIST if object with `name` exists
 */
int TREE::_add_local_object( std::string name, std::string hash, int type )
{
        struct entry tmp;

        // tmp.type = 0;
        SET_TYPE(tmp.type,type);
        tmp.hash = hash;
        tmp.s_tree = NULL;

        modified = true;

        auto ret = contents.insert( std::pair<std::string, struct entry>(name,tmp) );
        if( ret.second == false )
                return -EADD_EXIST;

        return ADD_SUCCESS;
}

/**
 * @func _add_empty_first_dir
 * @brief Adds a empty dummy directory
 *
 * @param name Entire path
 *
 * @return ADD_SUCCESS on success
 * 	-EADD_EXIST if the tree with first dirname exists
 * 	-EADD_NO_PARENT if the path has no parent directories
 */
int TREE::_add_empty_first_dir( std::string name )
{
	int tmp;
	std::string subtree;

	tmp = first_dirname( name, subtree );
	if( tmp >= 0 ){
		return _add_local_object( subtree, "0", TREE_OBJ );
	}
	else
		return -EADD_NO_PARENT;
}

/**
 * @func _add_object
 * @brief Adds an object to the current tree
 *
 * @param name Name of the object
 * @param hash Hash value of the object
 * @param type Type of the object
 *
 * @return ADD_SUCCESS on success,
 * 	-EADD_EXIST if the object with `name` already exists
 * 	-EADD_EXISTN if object with `name` exists, but of different type
 * 	-EADD_INVNAME if `name` is invalid
 *	-EADD_NOSUBD if the path has some invalid directory
 *	-EADD_INVHASH if the hash is invalid
 *	-EADD_NO_MEM if error regarding unavailability of memory.
 */
int TREE::_add_object( std::string name, std::string hash, int type, bool recursive )
{
	if( name.length() <= 0 )
		return -EADD_INVNAME;
	if( hash.length() != 40 )
		return -EADD_INVHASH;
	
	std::string h,subtree,depth;
	int ret,tmp;
	struct entry *ptr;


	
	// Check for duplicates
	ret = _get_object( name, h, type );
	if( ret == GET_SUCCESS )
		return -EADD_EXIST;
	else if( ret == -EGET_TYPE )
		return -EADD_EXISTN;
        else if( ret == -EGET_INVNAME )
		return -EADD_INVNAME;
	else if( !recursive && ( ret == -EGET_NO_SUBDIR || ret == -EGET_NO_ENTRY ) )
		return -EADD_NOSUBD;


add_block:
	// Get the directory which is next inorder in the given path
	ret = _get_next_dir( name, ptr, depth );
	switch(ret){
                case -EGET_NO_ENTRY:
                	if( recursive ){
                		tmp = _add_empty_first_dir( name );
                		goto add_block;
                	}
                	else
				return -EADD_NOSUBD;
		// If the path contains no subdirectories, just add the
		// element directly in this tree, if no such entry is
		// already present
                case -EGET_NO_PARENT:{
			tmp = _get_local_entry( name, ptr );
			if( tmp == -EGET_NO_ENTRY )
				return _add_local_object( name, hash, type );
			else
				return -EADD_EXIST;
		}
		// If the path has a subdirectory and the subdirectory 
		// exists, pass on the remaining path to the 
		// _add_object of that tree, and then mark the
		// modified flag
		case GET_SUCCESS:{
			TREE *tree = NULL;
			tmp = _get_child_tree( ptr, tree );
			if( tmp != GET_SUCCESS || tree == NULL  ){
				if( tmp == -EGET_NO_MEM )
					return -EADD_NO_MEM;
				else{
					if( recursive ){
						tmp = _add_empty_first_dir( name );
						goto add_block;	
					}
					else
						return -EADD_NOSUBD;
				}
			}
			else{
				tmp = tree->_add_object( depth, hash, type, recursive );
				if( tmp != ADD_SUCCESS )
					return tmp;
				else{
					ptr->s_tree->modified = true;
					return ADD_SUCCESS;
				}
			}
		}
	}

	return ADD_SUCCESS;
}

int TREE::add_subtree( std::string treename, std::string hash )
{
	return _add_object( treename, hash, TREE_OBJ, false );
}

int TREE::add_blob( std::string filename, std::string hash )
{
	return _add_object( filename, hash, BLOB_OBJ, false );
}

int TREE::recursive_add_subtree( std::string treename, std::string hash )
{
	return _add_object( treename, hash, TREE_OBJ, true );	
}

int TREE::recursive_add_blob( std::string filename, std::string hash )
{
	return _add_object( filename, hash, BLOB_OBJ, true );
}

/**
 * @brief TREE::_modify_local_object
 * Modifies the current tree for the given name to the target hash. This function
 * cannot modify a local object which is of a different type. To do that, the old
 * object must be deleted and then must be replaced by the new object of the new type.
 *
 * @param name Name of the target object, which is being modified
 * @param hash The new hash of the target object
 * @param type Type of the new object
 * @return MOD_SUCCESS on successful modification,
 *      -EMOD_NO_ENTRY if the local object doesn't exist
 *      -EMOD_TYPE if the type of the local object is different from the target type
 *      -EMOD_CORRUPTED if the data structure is corrupted
 */
int TREE::_modify_local_object( std::string name, std::string hash, int type )
{
	struct entry *tmp;
	std::string t;

	int ret = _get_local_entry( name, tmp );
	if( ret == -EGET_NO_ENTRY )
		return -EMOD_NO_ENTRY;
	if( TYPE(tmp->type) != type )
		return -EMOD_TYPE;

	tmp->hash = hash;
	if( TYPE(type) == TREE_OBJ ){
		if( tmp->s_tree != NULL ){
			if( tmp->s_tree->sub_tree != NULL ){
				// TODO: Write to reflog
				ret = tmp->s_tree->sub_tree->write_tree( t );
                                if( ret != WRITE_SUCCESS )
                                    return -EMOD_CORRUPTED;
                                tmp->s_tree->sub_tree->destroy_tree();
				delete tmp->s_tree;
				tmp->s_tree = NULL;
			}
		}
	}

	return MOD_SUCCESS;
}

/**
 * @brief TREE::_modify_object
 *
 * Modifies a given object with path `name` to the given hash. Modifying across
 * types is not supported
 *
 * @param name The fullpath of the object in question
 * @param hash The target hash
 * @param type The target/actual type of the object in question
 * @return MOD_SUCCESS on successful modification
 *      -EMOD_INVNAME if the given path is invalid
 *      -EMOD_INVHASH if the given hash is invalid
 *      -EMOD_NO_OBJECT if the specified object with `name` doesn't exist
 *      -EMOD_NO_SUBDIR if the subdirectory in the path doesn't exist
 *      -EMOD_NO_TREE if any of the subdirectory specified in path, is not a tree
 *      -EMOD_TYPE if the specified type doesn't match with the object type
 *      -EMOD_NO_ENTRY if the specified object doesn't have an entry
 *      -EMOD_NO_MEM if the memory creation during modification failed
 *      -EMOD_CORRUPTED if the data structure is corrupted
 */
int TREE::_modify_object( std::string name, std::string hash, int type )
{
        struct entry *ptr;
	std::string depth;
        std::string tmp_hash;
        int tmp;

	if( name.length() <= 0 )
		return -EMOD_INVNAME;
	if( hash.length() != 40 )
		return -EMOD_INVHASH;

        int ret = _get_object( name, tmp_hash, type );
	switch(ret){
		case -EGET_NO_OBJECT:
			return -EMOD_NO_OBJECT;
		case -EGET_NO_SUBDIR:
			return -EMOD_NO_SUBDIR;
		case -EGET_NO_TREE:
			return -EMOD_NO_TREE;
		case -EGET_TYPE:
			return -EMOD_TYPE;
		case -EGET_INVNAME:
			return -EMOD_INVNAME;
	}

	// Get the directory which is next inorder in the given path
	ret = _get_next_dir( name, ptr, depth );
	switch(ret){
                case -EGET_NO_ENTRY:
			return -EMOD_NO_SUBDIR;
		// If the path contains no subdirectories, search for
		// the element and modify that if it exists. Otherwise
		// return an error.
                case -EGET_NO_PARENT:{
			tmp = _get_local_entry( name, ptr );
			if( tmp == -EGET_NO_ENTRY )
				return -EMOD_NO_ENTRY;
			else
				return _modify_local_object( name, hash, type );
		}
		// If the path has a subdirectory and the subdirectory 
		// exists, pass on the remaining path to the 
		// _modify_object of that tree, and then mark the
		// modified flag
		case GET_SUCCESS:{
			TREE *tree = NULL;
			tmp = _get_child_tree( ptr, tree );
			if( tmp != GET_SUCCESS || tree == NULL  ){
				if( tmp == -EGET_NO_MEM )
					return -EMOD_NO_MEM;
				else
					return -EMOD_NO_SUBDIR;
			}
			else{
				tmp = tree->_modify_object( depth, hash, type );
				if( tmp != MOD_SUCCESS )
					return tmp;
				else{
					ptr->s_tree->modified = true;
					return MOD_SUCCESS;
				}
			}
		}
	}
	return MOD_SUCCESS;
}

int TREE::modify_subtree( std::string treename, std::string hash )
{
	return _modify_object( treename, hash, TREE_OBJ );
}

int TREE::modify_blob( std::string filename, std::string hash )
{
	return _modify_object( filename, hash, BLOB_OBJ );
}


/**
 * @brief TREE::_remove_local_object
 * Removes a local object with `name`
 *
 * @param name Name of the target object
 * @param type Type of the object
 * @return REM_SUCCESS on successful removal
 *      -EREM_NO_ENTRY, if there is no such entry with `name`
 *      -EREM_TYPE, if there is a type mismatch
 *      -EREM_CORRUPTED, if the data structure is corrupted( modified
 *          flag is ON, but the subtree is NULL )
 *      -EREM_FAILURE, if the erase operation was unable to be completed.
 */
int TREE::_remove_local_object( std::string name, int type )
{
    struct entry *tmp;
    std::string t;

    int ret = _get_local_entry( name, tmp );
    if( ret == -EGET_NO_ENTRY )
            return -EREM_NO_ENTRY;
    if( TYPE(tmp->type) != type )
            return -EREM_TYPE;

    if( TYPE(type) == TREE_OBJ ){
            if( tmp->s_tree != NULL ){
                    if( tmp->s_tree->sub_tree != NULL ){
                            // TODO: Write to reflog
                            ret = tmp->s_tree->sub_tree->write_tree( t );
                            if( ret != WRITE_SUCCESS )
                                return -EREM_CORRUPTED;
                            tmp->s_tree->sub_tree->destroy_tree();

                            // The subtree is cleared in the destructor of the struct
                            // entry. So no need to clear that part of the memory here
                            delete tmp->s_tree;
                            tmp->s_tree = NULL;
                    }
            }
    }

    ret = contents.erase( name );
    if( ret == 0 )
        return EREM_FAILURE;

    return REM_SUCCESS;
}

/**
 * @brief TREE::_remove_object
 * Removes the object with `name` with `type`
 *
 * @param name Name of the target object
 * @param type Type of the target object
 * @return REM_SUCCESS on success,
 *      -EREM_INVNAME, if the given `name` is invalid
 *      -EREM_NO_SUBDIR, if the path contains some invalid subdirectory
 *      -EREM_NO_TREE, if the path contains some file which is not a directory
 *      -EREM_TYPE, if there is a type mismatch
 *      -EREM_NO_ENTRY. if there is no object with `name`
 *      -EREM_NO_MEM, for memory related issues during remove
 */
int TREE::_remove_object( std::string name, int type )
{
	struct entry *ptr;
	std::string tmp_hash,depth;
	int tmp;

	if( name.length() <= 0 )
		return -EREM_INVNAME;

	int ret = _get_object( name, tmp_hash, type );
	switch(ret){
		case -EGET_NO_OBJECT:
		return -EREM_NO_OBJECT;
		case -EGET_NO_SUBDIR:
		return -EREM_NO_SUBDIR;
		case -EGET_NO_TREE:
		return -EREM_NO_TREE;
		case -EGET_TYPE:
		return -EREM_TYPE;
		case -EGET_INVNAME:
		return -EREM_INVNAME;
	}

    // Get the directory which is next inorder in the given path
	ret = _get_next_dir( name, ptr, depth );
	switch(ret){
		case -EGET_NO_ENTRY:
		return -EREM_NO_SUBDIR;
            // If the path contains no subdirectories, search for
            // the element and modify that if it exists. Otherwise
            // return an error.
		case -EGET_NO_PARENT:{
			tmp = _get_local_entry( name, ptr );
			if( tmp == -EGET_NO_ENTRY )
				return -EREM_NO_ENTRY;
			else
				return _remove_local_object( name, type );
		}
            // If the path has a subdirectory and the subdirectory
            // exists, pass on the remaining path to the
            // _modify_object of that tree, and then mark the
            // modified flag
		case GET_SUCCESS:{
			TREE *tree = NULL;
			tmp = _get_child_tree( ptr, tree );
			if( tmp != GET_SUCCESS || tree == NULL  ){
				if( tmp == -EGET_NO_MEM )
					return -EREM_NO_MEM;
				else
					return -EREM_NO_SUBDIR;
			}
			else{
				tmp = tree->_remove_object( depth, type );
				if( tmp != REM_SUCCESS )
					return tmp;
				else{
					ptr->s_tree->modified = true;
					return REM_SUCCESS;
				}
			}
		}
	}
	return REM_SUCCESS;
}

int TREE::remove_blob( std::string filename )
{
    return _remove_object( filename, BLOB_OBJ );
}

int TREE::remove_subtree( std::string dirname )
{
    return _remove_object( dirname, TREE_OBJ );
}


/**
 * @brief TREE::open_tree
 * Opens a tree with `hash` and populates all the required data structures
 *
 * @param hash Hash of the target tree
 * @return OPEN_SUCCESS on success
 *      -EOPEN_GET_FAIL if the object with `hash` couldn't be opened
 *      -EOPEN_NO_TREE if the object with `hash` is not a tree
 */
int TREE::open_tree( std::string hash )
{
	int ret;
	std::stringstream stream;
	char buf[256];
	bool empty = false;
	struct entry tmp;
	std::string name;
	char hash_arr[20];

	// Clear contents to ensure old values arent sticking around
	contents.clear();

	ret = get_new_object( hash );
	if( ret != 1 )
                return -EOPEN_GET_FAIL;
	
	// Throw error if the hash doesnt correspond to tree object
	ret = object_type();
	if( ret != TREE_OBJECT )
                return -EOPEN_NO_TREE;
	
	// Read the entire object
	do {
		ret = read_object( buf, 256 );
		if( ret > 0 )
			stream.write( buf, ret );
	}while( ret == 256 );
	
	while( !empty ){
		int written;

		name.clear();
		stream.read( (char *)&tmp.type, 2 );
		if( stream.gcount() != 2 ){
			empty = true;
			break;
		}
		stream.read( hash_arr, 20 );
		if( stream.gcount() != 20 ){
			empty = true;
			break;
		}
		ret = hash_numtostr( tmp.hash, (unsigned char *)hash_arr );
		// Precautionary measure if the name is too large
		do{
			stream.getline( (char *)&buf, 256, (char)0 );
			written = stream.gcount();
			if( written > 0 )
				name = name + buf;
			if( written != 256 )
				break;
		}while(1);

		tmp.s_tree = NULL;

		// Finally add the element to the map
		contents.insert( std::pair<std::string, struct entry>(name,tmp) );
	}

	old_hash = hash;
	modified = false;

        return OPEN_SUCCESS;

}

/**
 * @brief TREE::_write_tree
 * Writes the tree in memory to disk i.e. create an actual object with the data
 * structure present in the memory
 *
 * @param hash Reference to a variable which will be stored with the value of the
 *              hash
 * @param is_simulate Whether to simulate writing or not.
 *
 * @return WRITE_SUCCESS on success,
 *      -EWRITE_NULLMOD_SUBTREE if a modified subtree is NULL
 *      -EWRITE_INV_HASH if the hash present in the data structure is corrupted
 */
int TREE::_write_tree( std::string& hash, bool is_simulate )
{
	std::stringstream stream;
	auto it = contents.begin();
	auto end = contents.end();
	int ret;
	TREE *subtree;

        // Loop through the entire map and write all the objects
	for( ; it != end; it++ ){
		unsigned char hashnum[20];

                // If the type of the subobject is TREE, try to write the subtree
                // recursively if it was modified. Otherwise write the hash directly
		if( TYPE(it->second.type) == TREE_OBJ ){
			if( it->second.s_tree != NULL ){
				subtree = it->second.s_tree->sub_tree;
				if( it->second.s_tree->modified == true ){
					if( subtree == NULL ){
	                                        return -EWRITE_NULLMOD_SUBTREE;
					}
					ret = subtree->_write_tree( it->second.hash, is_simulate );
					if( ret != WRITE_SUCCESS ){
	                                        return ret;
					}
				}
			}
		}
		stream.write( (const char *)&it->second.type, 2 );
		ret = hash_strtonum( it->second.hash, hashnum );
		if( ret != SUCCESS ){
                       return -EWRITE_INV_HASH;
		}	       
		stream.write( (char *)hashnum, 20 );
		stream.write( it->first.c_str(), it->first.length()+1 );
	}

	
	// Dont create the object, just simulate the hashing.
	if( is_simulate )
		ret = hash_contents( stream, TREE_OBJECT );
	// Actually create the object and get the hash to return it.
	else
		ret = create_object( stream, TREE_OBJECT );

	if ( ret == -1  )
		return -EWRITE_UNABLE_FILE;
	
	hash = get_hash();
	old_hash = hash;

	return WRITE_SUCCESS;	

}

int TREE::simulate_write_tree( std::string& hash )
{
	return _write_tree( hash, true );
}

int TREE::write_tree( std::string& hash )
{
	return _write_tree( hash, false );
}

int TREE::create_tree()
{
	modified = true;
	return 0;
}

	
/**
 * @brief TREE::_get_local_object
 * Gets a local object from the given tree.
 *
 * @param name Name of the target object
 * @param hash Variable to hold hash of the required object
 * @param type Type of the target object
 * @return GET_SUCCESS on success,
 *      -EGET_NO_OBJECT if no such object with `name` exists
 *      -EGET_TYPE if the type of the object doesn't match
 */
int TREE::_get_local_object( std::string name, std::string& hash, int type )
{
        auto ret = contents.find( name );
	if( ret == contents.end())
		return -EGET_NO_OBJECT;
	else{
		if( TYPE(ret->second.type) == type ){
			hash = ret->second.hash;
			return GET_SUCCESS;
		}
		else{
			return -EGET_TYPE;
		}
	}
}

int TREE::_get_local_subtree( std::string dirname, std::string& hash )
{
	return _get_local_object( dirname, hash, TREE_OBJ );
}

int TREE::_get_local_blob( std::string filename, std::string& hash )
{
	return _get_local_object( filename, hash, BLOB_OBJ );
}

int TREE::_get_object( std::string name, std::string& hash, int type )
{

	if( name[name.size() - 1] == '/' && type == BLOB_OBJ )
		return -EGET_INVNAME;
	
	// depth is for the remaining path
	std::string depth;
	TREE *parent;


	int ret = _get_immediate_parent_tree( name, parent );
        if( ret == -EGET_NO_PARENT )
		return _get_local_object( name, hash, type );
	else if( ret == GET_SUCCESS ){
		ret = filename( name, depth );
		return parent->_get_object( depth, hash, type );
	}
	else
		return ret;
}

int TREE::get_subtree( std::string dirname, std::string& hash )
{
	return _get_object( dirname, hash, TREE_OBJ );
}

int TREE::get_blob( std::string filename, std::string &hash )
{
	return _get_object( filename, hash, BLOB_OBJ );
}

/**
 * @brief TREE::destroy_tree
 *
 * Destroys the entire tree and all its predecessors.
 */
void TREE::destroy_tree()
{
    contents.clear();
    old_hash.clear();
}

#define BEGIN 0
#define END 1
std::map<std::string,struct entry>::const_iterator TREE::_cpos( std::string path, int position )
{
	struct entry *ptr;
	std::string depth;
	int ret = _get_next_dir( path, ptr, depth );
	TREE *tree;
	switch(ret){
		case GET_SUCCESS:
			ret = _get_child_tree( ptr, tree );
			if( ret != GET_SUCCESS )
				return inv.cend();
			else
				return tree->_cpos(depth,position);
		case -EGET_NO_PARENT:
			ret = _get_child_tree( path, tree );
			if( ret != GET_SUCCESS )
				return inv.cend();
			else
				return position == BEGIN ? tree->cbegin() : tree->cend();
		default:
			return inv.cend();
	}
}

std::map<std::string,struct entry>::const_iterator TREE::cbegin( std::string path )
{
	return _cpos( path, BEGIN );
}

std::map<std::string,struct entry>::const_iterator TREE::cbegin()
{
	return contents.cbegin();
}

std::map<std::string,struct entry>::const_iterator TREE::cend( std::string path )
{
	return _cpos( path, END );
}

std::map<std::string,struct entry>::const_iterator TREE::cend()
{
	return contents.cend();
}

std::map<std::string,struct entry>::const_iterator TREE::cinv()
{
	return inv.cend();
}

void TREE::cat()
{
	auto it = contents.begin();
	auto end = contents.end();

	// Loop through the entire map and prints all the objects in the tree
	for( ; it != end; it++ ){
		if( TYPE(it->second.type) == TREE_OBJ )
			std::cout<<"tree ";
		else
			std::cout<<"blob ";
		std::cout<< it->second.hash <<" ";
		std::cout<< it->first<<"\n";
	}

}

void TREE::_rec_cat( int indent )
{
	auto it = contents.begin();
	auto end = contents.end();

	// Loop through the entire map and prints all the objects in the tree
	for( ; it != end; it++ ){
		for( int i=0; i<indent; i++ ) std::cout<<"  ";
		if( TYPE(it->second.type) == TREE_OBJ )
			std::cout<<"tree ";
		else
			std::cout<<"blob ";
		std::cout<< it->second.hash <<" ";
		std::cout<< it->first<<"\n";
		if( TYPE(it->second.type) == TREE_OBJ ){
			TREE *tr;
			int ret = _get_child_tree( it->first, tr );
			tr->_rec_cat( indent + 1);
		}
	}
}

void TREE::rec_cat()
{
	_rec_cat( 0 );
}

static std::string new_path( std::string dirpath, std::string filename )
{
	if( dirpath[ dirpath.size() - 1 ] != '/' )
		dirpath += '/';
	return dirpath+filename;
}



int TREE::tree_to_fs( std::string path )
{
	std::map<std::string,struct entry>::const_iterator begin,end;
	std::string prefix;

	if( path.empty() ){
		begin = cbegin( );
		end = cend();
		prefix = "" ;
	}
	else{
		begin = cbegin( path );
		end = cend( path );
		int ret = mkdir( path.c_str(), 0777 );
		if( ret != 0 ) {
			if( errno != EEXIST )
				return -ETREE_TO_FS_MKDIR_FAIL; 
		}
		prefix = path+"/";
	}

	std::string name;

	for(; begin != end; begin++ ){
		std::string fname = begin->first;
		struct entry* node = ( struct entry *)&begin->second;

		name = prefix + fname;
		// name = new_path( path, fname );
		if( TYPE(node->type) == BLOB_OBJ ){
			OBJ obj;
			int ret = obj.create_file_from_blob( node->hash, name );
			switch(ret){
				case CFB_SUCCESS:
					continue;
				case -ECFB_BUG:
					return -ETREE_TO_FS_BUG;
				default:
					return -ETREE_TO_FS_CORRUPT;
			}
		}
		else{
			int ret = tree_to_fs( name );
			switch(ret){
				case TREE_TO_FS_SUCCESS:
					continue;
				default:
					return ret;
			}
		}
	}

	return TREE_TO_FS_SUCCESS;

}

/****************** TEMP FUNCTIONS
 *** functions that will be used when the actual functions are not ready
 */
#define T_OBJECT "8f8e85330221ad054d67516331c5bfbd117aa3c9"

static int get_current_tree( TREE* tree )
{
	tree->open_tree(T_OBJECT);
	return 0;
}

static int update_current_tree( TREE *tree )
{
	std::cout<< tree->get_hash();
	return 0;
}
/****************** END ************/

enum default_rm_errors{
	RM_SUCCESS = 0,
	ERM_TREE_BUG
};


int DEFAULT_RM_TREE( std::vector<std::string> filelist )
{
	TREE tree;
	// STAGE stager;
	int tmp;
	std::string hash;
	auto begin = filelist.cbegin();
	auto end=filelist.end();

	// tmp = get_current_tree( &tree );
	for( ; begin != end; begin++ ){
		tmp = tree.remove_blob( *begin );
		switch(tmp){
			case REM_SUCCESS:
				continue;
			case -EREM_TYPE:
				tmp = tree.remove_subtree( *begin );
				switch(tmp){
					case REM_SUCCESS:
						continue;
					default:
						return -ERM_TREE_BUG;
				}
				break;
			default:
				std::cerr<<tmp<<*begin<<" doesn't exist."<<std::endl;
				continue;
		}
	}

	tmp = tree.write_tree( hash );
	tmp = update_current_tree( &tree );
	// stager.flush();
	return RM_SUCCESS;
}