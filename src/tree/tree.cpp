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

/**
 * @func _add_local_object
 * @brief Helper function for adding a object entry in the current tree
 *
 * @param name Name of the object
 * @param hash Hash of the object
 * @param type Type of the object
 *
 * @return 0 on success
 * 	-EADD_EXIST if object with `name` exists
 */
int TREE::_add_local_object( std::string name, std::string hash, int type )
{
	struct entry tmp;

	SET_TYPE(tmp.type,type);
	tmp.hash = hash;
	tmp.s_tree = NULL;

	modified = true;

	auto ret = contents.insert( std::pair<std::string, struct entry>(name,tmp) );
	if( ret.second == false )
		return -EADD_EXIST;

	return 0;
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
int TREE::_get_child_tree( std::string name, TREE& *tree )
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
	if( TYPE(entry->type) != TREE )
		return -EGET_NO_TREE;

	// Allocate memory for subtree if and only when needed. This will inturn allocate memory
	// for TREE objects below in the heirarchy
	if( entry->s_tree == NULL ){
		entry->s_tree = new subtree( entry->hash );
		if( !entry->s_tree || !entry->s_tree->subtree )
			return -EGET_NO_MEM;
	}

	tree = entry->s_tree->subtree;
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
int TREE::_get_immediate_parent_tree( std::string name, TREE& *parent )
{
	std::string h,subtree,depth;
	int ret;

	int tmp = first_dirname( dirname, subtree );
	if( tmp >= 0 ){
		// If the `name` itself is in the current tree
		if( tmp == dirname.size() - 1 )
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
		return -EGET_NOPARENT;
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
int TREE::_get_next_dir( std::string name, struct entry* &parent, std::string &depth )
{
	std::string subtree;
	int ret;

	int tmp = first_dirname( dirname, subtree );
	if( tmp >= 0 ){
		if( tmp == dirname.size() - 1 ){
			depth.clear();
			return _get_local_entry( subtree, parent );
		}
		else{
			depth = dirname.substr( tmp+1, dirname.size() - tmp - 1 );
			return _get_local_entry( subtree, parent );
		}
	}
	else
		return -EGET_NOPARENT;
}
			
enum add_errors{
	ADD_SUCCESS = 0,
	EADD_EXIST,
	EADD_EXISTN,
	EADD_INVNAME,
	EADD_NOSUBD,
	EADD_INVHASH,
	EADD_NO_MEM
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
int TREE::_add_object( std::string name, std::string hash, int type )
{
	if( treename.len() <= 0 )
		return -EADD_INVNAME;
	if( hash.len() != 40 )
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
	else if( ret == -EGET_INV_NAME )
		return -EADD_INVNAME;
	else if( ret == -EGET_NO_SUBDIR || ret == -EGET_NO_ENTRY )
		return -EADD_NOSUBD;

	// Get the directory which is next inorder in the given path
	ret = _get_next_dir( name, ptr, depth );
	switch(ret){
		case -EGET_NOENTRY:
			return -EADD_NOSUBD;
		// If the path contains no subdirectories, just add the
		// element directly in this tree, if no such entry is
		// already present
		case -EGET_NOPARENT:{
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
				else
					return -EADD_NOSUBD;
			}
			else{
				tmp = tree->_add_object( depth, hash, type );
				if( tmp != ADD_SUCCESS )
					return tmp;
				else{
					ptr->s_tree->modified = true;
					return ADD_SUCCESS;
				}
			}
		}
	}
}

int TREE::add_subtree( std::string treename, std::string hash )
{
	return _add_object( treename, hash, TREE );
}

int TREE::add_blob( std::string filename, std::string hash )
{
	return _add_object( filename, hash, BLOB );
}

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
	if( TYPE(type) == TREE ){
		if( tmp->s_tree != NULL ){
			if( tmp->s_tree->subtree != NULL ){
				// TODO: Write to reflog
				ret = tmp->s_tree->subtree->write_tree( t );
				ret = tmp->s_tree->subtree->destroy_tree();
				delete tmp->s_tree;
				tmp->s_tree = NULL;
			}
		}
	}

	return MOD_SUCCESS;
}

int TREE::_modify_object( std::string name, std::string hash, int type )
{
	TREE *ptr;
	std::string depth;

	if( name.len() <= 0 )
		return -EMOD_INVNAME;
	if( hash.len() != 40 )
		return -EMOD_INVHASH;

	int ret = _get_object( name, hash, type );
	switch(ret){
		case -EGET_NO_OBJECT:
			return -EMOD_NO_OBJECT;
		case -EGET_NO_SUBDIR:
			return -EMOD_NO_SUBDIR;
		case -EGET_NO_TREE:
			return -EMOD_NO_TREE;
		case -EGET_TYPE:
			return -EMOD_TYPE;
		case -EGET_INVNAME;
			return -EMOD_INVNAME;
	}

	// BUG: All the layers between the root and the actual layer is bypassed
	// here, and if the modified flag is any indication of actual modification
	// that will not get updated in the intermediate layers.
	ret = _get_immediate_parent_tree( name, ptr );
	if( ret == -EGET_NOPARENT )
		return _modify_local_object( name, hash, type );
	else if( ret == GET_SUCCESS ){
		ret = filename( name, depth );
		return ptr->_modify_local_object( depth, hash, type );
	}
}

int TREE::modify_subtree( std::string treename, std::string hash )
{
	return _modify_object( treename, hash, TREE );
}

int TREE::modify_blob( std::string filename, std::string hash )
{
	return _modify_object( filename, hash, BLOB );
}

int TREE::open_tree( std::string hash )
{
	int ret;
	std::stringstream stream;
	char buf[256];
	bool empty = false;
	struct entry tmp;
	std::string name;
	char hash[20];

	// Clear contents to ensure old values arent sticking around
	contents.clear();

	ret = get_new_object( hash );
	if( ret != 1 )
		return -1;
	
	// Throw error if the hash doesnt correspond to tree object
	ret = object_type();
	if( ret != TREE_OBJECT )
		return -2;
	
	// Read the entire object
	do {
		ret = read_object( buf, 256 );
		if( ret > 0 )
			stream.write( buf, ret );
	}while( ret == 256 );
	
	while( !empty ){
		int written;

		name.clear();
		stream.read( &tmp.type, 2 );
		if( stream.gcount() != 2 ){
			empty = true;
			break;
		}
		stream.read( hash, 20 );
		if( stream.gcount() != 20 ){
			empty = true;
			break;
		}
		ret = hash_numtostr( &tmp.hash, hash );
		// Precautionary measure if the name is too large
		do{
			stream.getline( &buf, 256, 0 );
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

	return 0;

}

#define WRITE_SUCCESS 0
int TREE::write_tree( std::string& hash )
{
	std::stringstream stream;
	auto it = contents.cbegin();
	auto end = contents.cend();
	int ret;
	struct subtree *st;
	TREE *subtree;

	for( ; it != end; it++ ){
		char hashnum[20];
		if( TYPE(it->second->type) == TREE ){

			// BUG: Check whether s_tree is NULL and remove the check
			// for modified. modified can be replaced by the NULL check,
			// as it is enough to say whether the tree has been actually
			// modified. i.e. subtree is opened only modified.
			//
			// BUG in BUG: subtree's can be opened just for reading only,
			// so modified flag is necessary.
			//
			subtree = it->second->s_tree->subtree;
			if( it->second->s_tree->modified == true ){
				if( subtree == NULL ){
					return -3;
				}
				ret = subtree->write_tree( it->second->hash );
				if( ret != WRITE_SUCCESS ){
					return -1;
				}
			}
		}
		stream.write( (const char *)&it->second->type, 2 );
		ret = hash_strtonum( it->second->hash, hashnum );
		if( ret != SUCCESS ){
		       return -2;
		}	       
		stream.write( hashnum, 20 );
		stream.write( it->first.c_str(), it->first.length()+1 );
	}

	ret = create_object( stream, TREE_OBJECT );
	hash = get_hash();
	old_hash = hash;

	return WRITE_SUCCESS;
}	

int TREE::create_tree()
{
	modified = true;
}
enum get_errors{
	GET_SUCCESS = 0,
	EGET_NO_OBJECT,
	EGET_TYPE,
	EGET_NO_ENTRY,
	EGET_INVNAME,
	EGET_NO_SUBDIR,
	EGET_NO_TREE
};
	
int TREE::_get_local_object( std::string name, std::string& hash, int type )
{
	auto ret = contents.find( dirname );
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
	return _get_local_object( dirname, hash, TREE );
}

int TREE::_get_local_blob( std::string filename, std::string& hash )
{
	return _get_local_object( filename, hash, BLOB );
}

int TREE::_get_local_entry( std::string dirname, struct entry& *entry )
{
	auto ret = contents.find( dirname );
	if( ret == contents.end() )
		return -ENO_ENTRY;
	else{
		entry = &ret->second;
		return SUCCESS;
	}
}

int TREE::_get_object( std::string name, std::string& hash, int type )
{

	if( name[name.size() - 1] == '/' && type == BLOB )
		return -EGET_INVNAME;
	
	// depth is for the remaining path
	std::string depth;
	TREE *parent;

	int ret = _get_immediate_parent_tree( name, parent );
	if( ret == -EGET_NOPARENT )
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
	return _get_object( dirname, hash, TREE );
}

int TREE::get_blob( std::string filename, std::string &hash )
{
	return _get_object( filename, hash, BLOB );
}
