#include <iostream>
#include <string>
#include <stdio.h>
#include <map>
#include "tree.h"
#include "../objects/objects.h"

using namespace std;

// std::string first_dirname( const std::string path )//, std::string &dir )
// {
// 	string dir;
// 	std::size_t found = path.find_first_of("/");
// 	if( found == std::string::npos){
// 		//return -1;
// 		return "-1";
// 	}
// 	else{
// 		dir = path.substr(0,found);
// 		return dir;
// 	}
// }

// #define SHIFT 6
// #define MASK 0x07C0
// void dummy( std::map<char,int>& mymap, int* &p )
// {
// 	std::map<char,int>::iterator it = mymap.find('b');
// 	p = &it->second;
// }

// int main ()
// {
// 	std::map<char,int> mymap;
// 	std::map<char,int>::iterator it;

// 	int *a;

// 	mymap['a']=50;
// 	mymap['b']=100;
// 	mymap['c']=150;
// 	mymap['d']=200;

// 	dummy( mymap, a );
// 	*a=1111;	
// 	// print content:
// 	std::cout << "elements in mymap:" << '\n';
// 	std::cout << "a => " << mymap.find('a')->second << '\n';
// 	std::cout << "b => " << mymap.find('b')->second << '\n';
// 	std::cout << "c => " << mymap.find('c')->second << '\n';
// 	std::cout << "d => " << mymap.find('d')->second << '\n';

// 	return 0;
// }

int main()
{
	TREE first;
	OBJ obj;
	string hash;

	cout<< obj.create_blob_object("test.cpp")<<endl;
	cout<< first.create_tree()<<endl;
	cout<< first.recursive_add_blob("folder1/folder2/folder3/folder4/folder5/test",obj.get_hash())<<endl;
	cout<< obj.discard_object();
	cout<< obj.create_blob_object("Makefile")<<endl;
	cout<< first.recursive_add_blob("folder1/folder2/folder4/make",obj.get_hash())<<endl;
	first.rec_cat();
	cout<< first.simulate_write_tree( hash )<<endl;
	cout<< hash << endl;
	first.rec_cat();
	cout<<"\n";

}
int __main()
{
	TREE first;
	TREE second;
	// int ret;
	string filename = "err";
	OBJ obj;
	string hash;

	// cout<< first.create_tree();
	// cout<< second.create_tree();

	// cout<< second.add_blob( "error", obj.get_hash() );
	// cout<< first.add_subtree("first/")
	// cout<< first.add_blob( "error", obj.get_hash() );
	// cout<< obj.discard_object();

	// cout<< first.write_tree( hash );
	// cout<< hash;
	// cout<< second.create_tree();
	cout<< obj.create_blob_object("test.cpp");
	// cout<< second.add_subtree("first", hash);
	// cout<< second.add_blob("first/test", obj.get_hash() );
	// cout<< "\n"<<obj.get_hash()<<endl;
	// cout<< second.write_tree( hash );
	// cout<< hash;
	cout<<"\n";

	// second.cat();
	// cout<< second.get_subtree("first", hash );
	first.destroy_tree();
	cout<< first.create_tree();
	cout<< "\n\n";
	cout<< first.recursive_add_blob("second/first/test", obj.get_hash() );

	cout<< obj.discard_object();
	cout<< obj.create_blob_object("err");	
	cout<< first.recursive_add_blob("second/first/error", obj.get_hash() );
	// cout<< first.add_subtree("second",hash);
	cout<< first.get_blob("second/first/test", hash);
	// cout<< "\n"<< hash<<"\n";
	cout<< "\n";
	first.rec_cat();

	cout<< obj.discard_object();
	cout<< obj.create_blob_object("Makefile");
	cout<< first.modify_blob("second/first/error",obj.get_hash() ) << "\n";

	// first.rec_cat();

	cout<< first.simulate_write_tree( hash );
	// cout<< first.write_tree( hash );
	cout<<endl;
	first.rec_cat();
	cout<< hash;
	// first.cat();
	return 0;
}

/*
int main()
{
	std::cout<<first_dirname("afdsfh/dsgsdrewr/sgsg/weret/dsfdsf")<<endl;
	cout<<first_dirname("afjksdhf")<<endl;
	cout<<first_dirname("a/c")<<endl;
	cout<<first_dirname("asfsf/fsf/sdfsdf/sdfds/dsfds/sdfs/dsdfsd/sfddsf/asd")<<endl;
	int a = 0xffff;
	int t = 4;
	printf("%4x\n",( a & ~MASK ) | ( t << SHIFT ) );
}*/
