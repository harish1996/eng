#ifndef _COMMIT_H
#define _COMMIT_H

#include <src/objects/objects.h>
#include <libs/helper/hash.h>
#include <sstream>
#include <time.h>

class commit: public OBJ {
	std::string _parents[2], _tree_id, _author, _commit_message;
	int _commit_type = -1;
	time_t _timestamp;
	bool is_open;
public:
	commit( const std::string &commit_id ) {
		int ret = open_commit( commit_id );
		if(ret == -1)
			is_open=false;
		else
			is_open=true;
	}
	commit() {
	}
	int open_commit( const std::string &commit_id );
	int get_commit_type();
	int get_parents( std::string *parents );
	int get_tree( std::string &tree_id );
	int get_author( std::string &author );
	int get_message( std::string &commit_message );
	int get_timestamp( std::string &timestamp);

	int write_parents( int num, const std::string *parents );
	int write_tree( const std::string &tree_id );
	int write_author( const std::string &author );
	int write_message( const std::string &commit_message );
	int create_commit( std::string &hash );

	bool is_opened(){ return is_open; }
	void cat();
};

#endif