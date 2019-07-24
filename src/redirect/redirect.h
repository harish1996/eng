#ifndef _REDIRECT_H
#define _REDIRECT_H

#include<stdio.h>
#include<string.h>
#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include "../init/init.h"
#include "../checkout/checkout.h"
#include "../log/log.h"
#include "../add_command/add.h"
#include "../commit_command/commit_cmd.h"


struct commands {
	std::string name;
	int (*redirect_function)(int argc, char *argv[]);
};

int redirect( int argc, char *argv[] );

#endif