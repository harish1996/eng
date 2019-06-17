#ifndef _TREE_H
#define _TREE_H

#include "src/objects/objects.h"
#include <stdint.h>
#include <sstream>

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
	std::map<std::string,struct entry> contents;


	int _add_local_object( std::string name, std::string hash, int type );
	int _get_local_object( std::string name, std::string& hash, int type );
	int _get_local_subtree( std::string dirname, std::string& hash );
	int _get_local_blob( std::string filename, std::string& hash );
	int _get_local_entry( std::string dirname, struct entry& *entry );	
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

#endif
