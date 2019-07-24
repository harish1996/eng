#ifndef _PUSH_PULL_H
#define _PUSH_PULL_H

#include <stdlib.h>
#include <fstream>
#include <string>

enum error_codes {
	SUCCESS = 0,
	ORIGIN_NOT_SET
};

static int push_to_origin( const std::string &origin );
static int pull_from_origin( const std::string &origin );

static int update_origin( const std::string &origin );

static int DEFAULT_PUSH();
static int DEFAULT_PULL();

#endif