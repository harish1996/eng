#include "commit_cmd.h"
#include <iostream>
#include <vector>

using namespace std;

int main( int argc, char *argv[] )
{
	std::vector<std::string> paths;

	if( argc < 2 ){
		cout<<"Required argument add/commit"<<endl;
		return 0;
	}
	if( (string)argv[1] == "add" ){
		for( int i=2; i<argc; i++ )
			paths.push_back( argv[i] );

		cout<< DEFAULT_ADD( paths )<<endl;
	}
	else if( (string) argv[1] == "commit" ){
		string message;
		for( int i=2; i<argc; i++ )
			message += (string)argv[i] + (string)" ";

		cout<< DEFAULT_COMMIT( message )<<endl;
	}
}