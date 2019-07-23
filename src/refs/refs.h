#ifndef _REFS_H
#define _REFS_H

#include<iostream>
#include<string>
#include<cstdlib>
#include<stdio.h>
#include<string.h>

std::string read_branch(std::string);

int write_branch(std::string , std::string);

enum gethead_returns{
	GETHEAD_SUCCESS,
	EGETHEAD_NOFILE
};

int getHEAD( std::string& );

int writeHEAD(std::string);

#endif
