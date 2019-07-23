#include "add.h"
#include <iostream>
#include <stdio.h>
#include <vector>

// #define T_OBJECT "8f8e85330221ad054d67516331c5bfbd117aa3c9"

using namespace std;

// static int extract_info( std::string str, std::string& name, std::string& hash )
// {
// 	int tmp = str.find_first_of(1);
// 	name = str.substr(0,tmp-1);
// 	hash = str.substr(tmp+1);
// 	return 0;
// }

int main( int argc, char *argv[] )
{
	// TREE tr;
	// STAGE stager;


	std::vector<std::string> paths;

	for( int i=1; i<argc; i++ )
		paths.push_back( argv[i] );

	cout<< DEFAULT_ADD( paths );

	// cout<< tr.open_tree(T_OBJECT);
	// cout<< stager.try_add("second/", &tr)<<endl;
	// cout<< stager.try_add("second/first/error", &tr)<< endl;
	// cout<< stager.try_add("second/first/test", &tr)<< endl;
	// cout<< stager.try_add("second/third/file", &tr)<< endl;

	// cout<< stager.flush();
	// cout<< add_file_to_list( "second/first/error", &tr )<<endl;
	// cout<< add_file_to_list( "second/first/test", &tr )<<endl;
	// cout<< add_file_to_list( "second/third/file", &tr )<<endl;
}