#ifndef _BRANCH_H
#define _BRANCH_H

#include <iostream>
#include <string>
#include "../refs/refs.h"

#define ECREATE_BRANCH_FAILURE -1
#define CREATE_BRANCH_SUCCESS 0

int DEFAULT_CREATE_BRANCH( std::string );

int DEFAULT_LIST_BRANCHES();

#endif
