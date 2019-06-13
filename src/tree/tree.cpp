#include "src/objects/objects.h"
#include <stdint.h>
#include <sstream>

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

struct entry {

#define OTHERS_MASK	0x0007
#define OTHERS_SHIFT	0
#define OTHERS_PERM(x) (( x & OTHERS_MASK ) >> OTHERS_SHIFT)

#define GROUPS_MASK	0x0038
#define GROUPS_SHIFT	3
#define GROUPS_PERM(x) (( x & GROUPS_MASK ) >> GROUPS_SHIFT)

#define USERS_MASK	0x01C0
#define USERS_SHIFT	6
#define USERS_PERM(x) (( x & USERS_MASK ) >> USERS_SHIFT)

#define TYPES_MASK	0xFE00
#define TYPES_SHIFT	9
#define TYPE(x)		(( x & TYPES_MASK ) >> TYPES_SHIFT)
#define SET_TYPE(t,x)	t = ( ( t & ~MASK ) | ( x << TYPES_SHIFT ) )

#define TREE	0
#define BLOB	1

	uint16_t type;
	//char hash[20];
	std::string hash;
	//std::string filename;
	struct subtree *s_tree;
}

struct subtree {
	bool modified;
	TREE *subtree;
	subtree(): modified(false) {}
	subtree( std::string hash ){
		subtree = new TREE();
		int ret = subtree->open_tree( hash );
		if( ret != 0 )
			modified = false;
		else
			modified = true;
	}
	~subtree(){
		if( subtree != NULL ){
			delete subtree;
		}
	}
};

class TREE : protected OBJ{

private:
	bool modified;
	std::string old_hash;
	//std::vector<struct entry> contents;
	std::map<std::string,struct entry> contents;
	int _add_local_object( std::string name, std::string hash, int type );
	int _get_local_object( std::string name, std::string& hash, int type );
	int _get_local_subtree( std::string dirname, std::string& hash );
	int _get_local_blob( std::string filename, std::string& hash );
	int _get_local_entry( std::string dirname, struct entry& *entry )	
	int _get_child_tree( std::string dirname, TREE& *tree );
public:
	TREE(): modified(false) {}
	int open_tree( std::string hash );
	int create_tree();
	int _get_immediate_parent_tree( std::string name, TREE& *parent );
	int _get_object( std::string name, std::string& hash, int type );
	int get_subtree( std::string dirname, std::string& hash );
	int get_blob( std::string filename, std::string& hash );
	int add_subtree( std::string treename, std::string hash );
	int add_blob( std::string filename, std::string hash );
	int modify_subtree( std::string treename, std::string hash );
	int modify_blob( std::string filename, std::string hash );
	int remove_subtree( std::string treename );
	int remove_blob( std::string filename );
	// Creates the tree object recursively.
	int write_tree( std::string &hash );
	int destroy_tree();
	std::map<std::string,struct entry>::const_iterator cbegin();
	std::map<std::string,struct entry>::const_iterator cend();
	void cat();
	~TREE();
};

/**
 * @func _add_local_object
 * @brief Helper function for adding a object entry in the current tree
 *
 * @param name Name of the object
 * @param hash Hash of the object
 * @param type Type of the object
 *
 * @return 0 on success
 */
int TREE::_add_local_object( std::string name, std::string hash, int type )
{
	struct entry tmp;

	SET_TYPE(tmp.type,type);
	tmp.hash = hash;
	tmp.s_tree = NULL;

	modified = true;

	contents.insert( std::pair<std::string, struct entry>(name,tmp) );

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
	if( TYPE(tmp->type) != TREE )
		return -EGET_NO_TREE;

	// Allocate memory for subtree if and only when needed. This will inturn allocate memory
	// for TREE objects below in the heirarchy
	if( tmp->s_tree == NULL ){
		tmp->s_tree = new subtree( tmp->hash );
		if( !tmp->s_tree || !tmp->s_tree->subtree )
			return -EGET_NO_MEM;
	}

	tree = tmp->s_tree->subtree;
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
			
enum add_errors{
	ADD_SUCCESS = 0,
	EADD_EXIST,
	EADD_EXISTN,
	EADD_INVNAME,
	EADD_NOSUBD,
	EADD_INVHASH
}

int TREE::_add_object( std::string name, std::string hash, int type )
{
	if( treename.len() <= 0 )
		return -EADD_INVNAME;
	if( hash.len() != 40 )
		return -EADD_INVHASH;
	
	std::string h,subtree,depth;
	int ret;
	TREE *ptr;

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
	
	// Add one object in the immediate parent
	ret = _get_immediate_parent_tree( name, ptr );
	if( ret == -EGET_NOPARENT )
		return _add_local_object( name, hash, type );
	else if( ret == GET_SUCCESS ){
		// Extracts filename from fullpath `name`
		ret = filename( name, depth );
		return ptr->_add_object( depth, hash, type );
	}
	else
		return ret;
}

int TREE::add_subtree( std::string treename, std::string hash )
{
	return _add_object( treename, hash, TREE );
}

int TREE::add_blob( std::string filename, std::string hash )
{
	return _add_object( filename, hash, BLOB );
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
			subtree = it->second->s_tree->subtree;
			if( it->second->s_tree->modified == true ){
				if( subtree == NULL ){
					return -3;
				}
				ret = subtree->write_tree( &it->hash );
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
	SUCCESS = 0,
	ENO_OBJECT,
	E_TYPE,
	ENO_ENTRY,
	EINV_NAME,
	ENO_SUBDIR,
	ENO_TREE
};
	
int TREE::_get_local_object( std::string name, std::string& hash, int type )
{
	auto ret = contents.find( dirname );
	if( ret == contents.end())
		return -ENO_OBJECT;
	else{
		if( TYPE(ret->second.type) == type ){
			hash = ret->second.hash;
			return SUCCESS;
		}
		else{
			return -E_TYPE;
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
		return -EINV_NAME;
	// subtree is for the immediate subtree in this tree node
	// depth is for the remaining path
	std::string subtree,depth;

	// Extracts the first dirname to be searched under this tree node
	int tmp = first_dirname( dirname, subtree );
	
	// If there is a subfolder/file character
	if( tmp >= 0 ){

		// Subfolder/file turns out to be empty ( eg. abc/ is a valid directory name )
		if( tmp == dirname.size() - 1 )
			return _get_local_subtree( dirname, hash );

		// Subfolder/file is present
		else{
			struct entry *ptr;

			// Remaining path
			depth = dirname.substr(found+1,dirname.size() - found - 1);
			
			// Get the entry of the corresponding subtree hash 
			int ret = _get_local_entry( subtree, ptr );

			// Return failure if there are no entries with the subtree name
			if( ret == -ENO_ENTRY )
				return -ENO_ENTRY;

			// If there is a valid entry
			else{
				// If the entry is a valid tree object
				if( TYPE(ptr->type) == TREE ){

					// If the tree is already not open
					if( ptr->s_tree == NULL ){

						// Allocate memory for a subtree and call subTREE's get_subtree function
						ptr->s_tree = new subtree( ptr->hash );
						return ptr->s_tree->subtree->_get_object( depth, hash, type );
					}
					else
						return ptr->s_tree->subtree->_get_object( depth, hash, type );
				}
				// If the entry is not a tree object return -2
				else{
					return -ENO_SUBDIR;
				}

			}
		}
	}
	// If there is no / character => Search for the directory name directly in this folder
	else
		return _get_local_object( dirname, hash, type );
	
}
int TREE::get_subtree( std::string dirname, std::string& hash )
{
	return _get_object( dirname, hash, TREE );
}

int TREE::get_blob( std::string filename, std::string &hash )
{
	return _get_object( filename, hash, BLOB );
}
