#ifndef _TREE_H
#define _TREE_H

#include "../objects/objects.h"
#include "../../libs/helper/hash.h"
#include <stdint.h>
#include <sstream>
#include <map>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

enum get_errors{
	GET_SUCCESS = 0,
	EGET_NO_OBJECT,
	EGET_TYPE,
	EGET_NO_ENTRY,
	EGET_INVNAME,
	EGET_NO_SUBDIR,
        EGET_NO_TREE,
        EGET_NO_PARENT,
        EGET_NO_MEM
};


enum write_returns{
    WRITE_SUCCESS = 0,
    EWRITE_NULLMOD_SUBTREE,
    EWRITE_INV_HASH,
    EWRITE_UNABLE_FILE
};

enum open_errors{
	OPEN_SUCCESS = 0,
	EOPEN_GET_FAIL,
	EOPEN_NO_TREE
};


enum add_errors{
	ADD_SUCCESS = 0,
	EADD_EXIST,
	EADD_EXISTN,
	EADD_INVNAME,
	EADD_NOSUBD,
	EADD_INVHASH,
	EADD_NO_MEM,
	EADD_NO_PARENT
};


enum mod_errors{
    MOD_SUCCESS=0,
    EMOD_NO_ENTRY,
    EMOD_TYPE,
    EMOD_INVNAME,
    EMOD_INVHASH,
    EMOD_NO_OBJECT,
    EMOD_NO_SUBDIR,
    EMOD_NO_TREE,
    EMOD_NO_MEM,
    EMOD_CORRUPTED
};

enum rem_returns{
    REM_SUCCESS = 0,
    EREM_NO_ENTRY,
    EREM_TYPE,
    EREM_CORRUPTED,
    EREM_FAILURE,
    EREM_NO_OBJECT,
    EREM_NO_SUBDIR,
    EREM_NO_TREE,
    EREM_INVNAME,
    EREM_NO_MEM
};

enum tree_to_fs_returns{
	TREE_TO_FS_SUCCESS = 0,
	ETREE_TO_FS_BUG,
	ETREE_TO_FS_CORRUPT,
	ETREE_TO_FS_MKDIR_FAIL
};

class TREE : public OBJ{

private:
	bool modified;
	std::string old_hash;
	std::map<std::string,struct entry> contents;
	std::map<std::string,struct entry> inv;


	int _add_local_object( std::string name, std::string hash, int type );
	int _add_empty_first_dir( std::string path );
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
        int __get_immediate_parent_tree( std::string name, TREE* &parent );

public:
	TREE(): modified(false) {}
	int open_tree( std::string hash );
	int create_tree();
	int _get_immediate_parent_tree( std::string name, TREE* &parent );
	int _get_object( std::string name, std::string& hash, int type );
	int get_subtree( std::string dirname, std::string& hash );
	int get_blob( std::string filename, std::string& hash );
	int _add_object( std::string name, std::string hash, int type, bool recursive );
	int add_subtree( std::string treename, std::string hash );
	int add_blob( std::string filename, std::string hash );
	int recursive_add_subtree( std::string treename, std::string hash );
	int recursive_add_blob( std::string filename, std::string hash );
	int _modify_object( std::string filename, std::string hash, int type );
	int modify_subtree( std::string treename, std::string hash );
	int modify_blob( std::string filename, std::string hash );
	int _remove_object( std::string filename, int type );
	int remove_subtree( std::string treename );
	int remove_blob( std::string filename );
	int _write_tree( std::string &hash, bool is_simulate );
	// Creates the tree object recursively.
	int write_tree( std::string &hash );
	int simulate_write_tree( std::string &hash );
	void destroy_tree();
        std::map<std::string,struct entry>::const_iterator cbegin( std::string path );
        std::map<std::string,struct entry>::const_iterator cend( std::string path );
        std::map<std::string,struct entry>::const_iterator cbegin();
        std::map<std::string,struct entry>::const_iterator cend();
        std::map<std::string,struct entry>::const_iterator cinv();
	/**
	 * @brief Writes the tree structure in the filesystem i.e. creates the folder 
	 * 	structure as in the tree to the filesystem.
	 * 
	 * @param path Path of the tree to write. Send empty path to write entire tree.
	 *
	 * @return TREE_TO_FS_SUCCESS on success,
	 *	-ETREE_TO_FS_BUG if there is a bug in the code
	 * 	-ETREE_TO_FS_CORRUPT if the objects folder is corrupt
	 *	-ETREE_TO_FS_MKDIR_FAIL if creation of folders failed
	 */
	int tree_to_fs( std::string path );
	
	void cat();
	void _rec_cat( int indent );
	void rec_cat();
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
		if( hash != "0" ){
			int ret = sub_tree->open_tree( hash );
			if( ret != 0 )
				modified = false;
			else
				modified = true;
		}
		else{
			sub_tree->create_tree();
			modified = true;
		}
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
        entry():type(0),s_tree(0){}
        entry( uint16_t type, std::string hash ):type(type),hash(hash){}
        ~entry(){
            if( s_tree )
                delete s_tree;
        }

};

int DEFAULT_RM_TREE( std::vector<std::string> filelist );
#endif
