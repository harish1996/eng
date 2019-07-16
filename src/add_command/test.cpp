#include "add.h"
#include <iostream>
#include <stdio.h>

#define T_OBJECT "8f8e85330221ad054d67516331c5bfbd117aa3c9"

using namespace std;

int main()
{
	TREE tr;
	STAGE stager;

	cout<< tr.open_tree(T_OBJECT);
	cout<< stager.try_add("second/first/error", &tr)<< endl;
	cout<< stager.try_add("second/first/test", &tr)<< endl;
	cout<< stager.try_add("second/third/file", &tr)<< endl;
	cout<< stager.flush();
	// cout<< add_file_to_list( "second/first/error", &tr )<<endl;
	// cout<< add_file_to_list( "second/first/test", &tr )<<endl;
	// cout<< add_file_to_list( "second/third/file", &tr )<<endl;
}