#ifndef _PUSH_PULL_H
#define _PUSH_PULL_H

#include <stdlib.h>
#include <fstream>
#include <string>

enum error_codes {
	SUCCESS = 0,
	ORIGIN_NOT_SET
};

int push_to_origin( const std::string &origin );
int pull_from_origin( const std::string &origin );

int update_origin( const std::string &origin );

int DEFAULT_PUSH();
int DEFAULT_PULL();

#endif