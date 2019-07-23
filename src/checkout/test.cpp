#define NEW_T_OBJECT "c3b72622980a9e22cf15b9a71a4fce7e057ca3dd"

#include <iostream>
#include <string>
#include <stdio.h>
#include <map>
#include "checkout.h"
#include "../tree/tree.h"
#include "../objects/objects.h"

using namespace std;

int _main()
{
	TREE first;
	OBJ obj;
	string hash;
	vector<string> files;
	string path;

	cout<< obj.create_blob_object("test.cpp")<<endl;
	hash = obj.get_hash();

	cout<< first.create_tree()<<endl;
	cout<< first.recursive_add_blob("folder1/folder2/folder3/folder4/folder5/test",obj.get_hash())<<endl;
	cout<< obj.discard_object();
	cout<< obj.create_blob_object("Makefile")<<endl;
	cout<< first.recursive_add_blob("folder1/folder2/folder4/make",obj.get_hash())<<endl;
	cout<< first.recursive_add_blob("folder1/folder2/folder5/folder6/make",obj.get_hash())<<endl;
	first.rec_cat();

	cout<< first.write_tree( hash )<<endl;
	cout<< hash<<endl;

	path.clear();
	cout<< first.tree_to_fs( path );
}


int main()
{
	// _main();
	int ret = _restore_tree( NEW_T_OBJECT, "" );
	return 0;
}
