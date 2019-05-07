#include <gzstream.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#define BUF_SIZE 256

void usage( char *arg )
{
	std::cout<<" Usage: "<< arg << " <in-file> <out-file>\n";
}

int main( int argc, char*argv[]) {
	char buf[BUF_SIZE];

	if( argc != 3 ){
		usage( argv[0] );
		return EXIT_FAILURE;
	}

	ogzstream out( argv[2] );
	if( !out.good() ){
		usage( argv[0] );
		return EXIT_FAILURE;
	}

	std::ifstream infile( argv[1] );
	if( !infile.good() ){
		usage( argv[0] );
		return EXIT_FAILURE;
	}

	while( infile.good() ){
		infile.read( buf, BUF_SIZE );
		out.write( buf, infile.gcount() );
	}

	return 0;
}
	
