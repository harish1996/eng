#ifndef _TREE_H
#define _TREE_H

#include "../objects/objects.h"
#include "../../libs/helper/hash.h"
#include <stdint.h>
#include <sstream>
#include <map>


class TREE : protected OBJ{

private:
	bool modified;
	std::string old_hash;
	std::map<std::string,struct entry> contents;
	std::map<std::string,struct entry> inv;


	int _add_local_object( std::string name, std::string hash, int type );
	int _get_local_object( std::string name, std::string& hash, int type );
	int _get_local_subtree( std::string dirname, std::string& hash );
	int _get_local_blob( std::string filename, std::string& hash );
	int _get_local_entry( std::string dirname, struct entry* &entry );	
        int _get_child_tree( std::string dirname, TREE* &tree );
        int _get_child_tree( struct entry *entry, TREE* &tree );
        int _get_next_dir(std::string name, struct entry* &parent, std::string &depth);
        int _modify_local_object( std::string name, std::string hash, int type );
        int _remove_local_object( std::string name, int type );
        std::map<std::string,struct entry>::const_iterator _cpos( std::string path, int position );
public:
	TREE(): modified(false) {}
	int open_tree( std::string hash );
	int create_tree();
	int _get_immediate_parent_tree( std::string name, TREE* &parent );
	int _get_object( std::string name, std::string& hash, int type );
	int get_subtree( std::string dirname, std::string& hash );
	int get_blob( std::string filename, std::string& hash );
	int _add_object( std::string name, std::string hash, int type );
	int add_subtree( std::string treename, std::string hash );
	int add_blob( std::string filename, std::string hash );
	int _modify_object( std::string filename, std::string hash, int type );
	int modify_subtree( std::string treename, std::string hash );
	int modify_blob( std::string filename, std::string hash );
	int _remove_object( std::string filename, int type );
	int remove_subtree( std::string treename );
	int remove_blob( std::string filename );
	// Creates the tree object recursively.
	int write_tree( std::string &hash );
	void destroy_tree();
        std::map<std::string,struct entry>::const_iterator cbegin( std::string path );
        std::map<std::string,struct entry>::const_iterator cend( std::string path );
        std::map<std::string,struct entry>::const_iterator cbegin();
        std::map<std::string,struct entry>::const_iterator cend();
        std::map<std::string,struct entry>::const_iterator cinv();
	void cat();
	~TREE(){
		destroy_tree();
	};
};

struct subtree {
	bool modified;
	TREE *sub_tree;
	subtree(): modified(false) {}
	subtree( std::string hash ){
		sub_tree = new TREE();
		int ret = sub_tree->open_tree( hash );
		if( ret != 0 )
			modified = false;
		else
			modified = true;
	}
	~subtree(){
		if( sub_tree != NULL ){
			delete sub_tree;
		}
	}
};


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
#define SET_TYPE(t,x)	t = ( ( t & ~TYPES_MASK ) | ( x << TYPES_SHIFT ) )

#define TREE_OBJ	0
#define BLOB_OBJ	1

	uint16_t type;
	std::string hash;
	struct subtree *s_tree;
        entry(){}
        entry( uint16_t type, std::string hash ):type(type),hash(hash){}
        ~entry(){
            if( s_tree )
                delete s_tree;
        }

};


#endif
