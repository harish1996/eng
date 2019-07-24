#include "commit_command/commit_cmd.h"
#include <iostream>
#include <vector>
#include "init/init.h"
#include "checkout/checkout.h"
#include "log/log.h"

using namespace std;

int main( int argc, char *argv[] )
{
	std::vector<std::string> paths;
	int ret;

	if( argc < 2 ){
		cout<<"Required argument add/commit"<<endl;
		return 0;
	}
	if( (string)argv[1] == "add" ){
		for( int i=2; i<argc; i++ )
			paths.push_back( argv[i] );

		ret = DEFAULT_ADD( paths );
		if( ret != 0 )
			cerr<<"Add failed\n";
	}
	else if( (string) argv[1] == "commit" ){
		string message;
		for( int i=2; i<argc; i++ )
			message += (string)argv[i] + (string)" ";

		ret = DEFAULT_COMMIT( message );
		if( ret != 0 )
			cerr<<"Commit failed\n";
	}
	else if( (string) argv[1] == "init" ){
		ret = init();
	}
	else  if( (string) argv[1] == "checkout" ){
		ret = DEFAULT_CHECKOUT( argv[2] );
		if( ret != 0 )
			cerr<<"Checkout failed\n";
	}
	else if( (string) argv[1] == "cat" ){
		ret = DEFAULT_CAT( argv[2] );
		if( ret != 0 )
			cerr<<"Cat failed\n";
	}
	else if( (string) argv[1] == "log" ){
		ret = DEFAULT_LOG( );
		if( ret != 0 )
			cerr<<"Log failed\n";
	}
	else{
		cerr<<"Command unknown\n";
	}
}