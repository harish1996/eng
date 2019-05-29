#include "src/objects/objects.h"
#include <stdint.h>
#include <sstream>

struct entry {

#define OTHERS_MASK	0x0007
#define OTHERS_SHIFT	0
#define OTHERS_PERM(x) (( x | OTHERS_MASK ) >> OTHERS_SHIFT)

#define GROUPS_MASK	0x0038
#define GROUPS_SHIFT	3
#define GROUPS_PERM(x) (( x | GROUPS_MASK ) >> GROUPS_SHIFT)

#define USERS_MASK	0x01C0
#define USERS_SHIFT	6
#define USERS_PERM(x) (( x | USERS_MASK ) >> USERS_SHIFT)

#define TYPES_MASK	0xFE00
#define TYPES_SHIFT	9
#define TYPE(x)		(( x | TYPES_MASK ) >> TYPES_SHIFT)

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
};

class TREE : protected OBJ{

private:
	bool modified;
	std::string old_hash;
	//std::vector<struct entry> contents;
	std::map<std::string,struct entry> contents;

public:
	TREE(): modified(false) {}
	int open_tree( std::string hash );
	int create_tree();
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

		// If the object is a tree, allocate the s_tree object
		if( TYPE(tmp.type) == TREE ){
			tmp.s_tree = new struct subtree();
			tmp.s_tree->modified = false;
			tmp.s_tree->subtree = NULL;
		}

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

//int TREE::get_subtree

