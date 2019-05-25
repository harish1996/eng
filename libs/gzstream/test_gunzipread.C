#include <gzstream.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#define BUF_SIZE 30

using namespace std;

void usage( char *arg )
{
	std::cout<<" Usage: "<< arg << " <in-file> <out-file>\n";
}

int main( int argc, char*argv[]) {
	char buf[BUF_SIZE+1];

	if( argc != 2 ){
		usage( argv[0] );
		return EXIT_FAILURE;
	}

	igzstream out( argv[1] );
	if( !out.good() ){
		usage( argv[0] );
		return EXIT_FAILURE;
	}

	/*
	std::ifstream infile( argv[1] );
	if( !infile.good() ){
		usage( argv[0] );
		return EXIT_FAILURE;
	}
	*/

	/*
	while( infile.good() ){
		infile.read( buf, BUF_SIZE );
		out.write( buf, infile.gcount() );
	}
	*/

	out.read( buf, BUF_SIZE );
	buf[BUF_SIZE] = 0;
	cout<< buf;

	return 0;
}
	
