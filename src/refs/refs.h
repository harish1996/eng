#ifndef _REFS_H
#define _REFS_H

#include<iostream>
#include<string>
#include<cstdlib>
#include<stdio.h>
#include<string.h>

std::string read_branch( const std::string& );

int write_branch( const std::string& , const std::string& );

enum gethead_returns{
	GETHEAD_SUCCESS,
	EGETHEAD_NOFILE
};

int getHEAD( char&, std::string& );

int writeHEAD( char , const std::string& );

#endif
