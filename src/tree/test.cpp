#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

std::string first_dirname( const std::string path )//, std::string &dir )
{
	string dir;
	std::size_t found = path.find_first_of("/");
	if( found == std::string::npos){
		//return -1;
		return "-1";
	}
	else{
		dir = path.substr(0,found);
		return dir;
	}
}

#define SHIFT 6
#define MASK 0x07C0

int main()
{
	std::cout<<first_dirname("afdsfh/dsgsdrewr/sgsg/weret/dsfdsf")<<endl;
	cout<<first_dirname("afjksdhf")<<endl;
	cout<<first_dirname("a/c")<<endl;
	cout<<first_dirname("asfsf/fsf/sdfsdf/sdfds/dsfds/sdfs/dsdfsd/sfddsf/asd")<<endl;
	int a = 0xffff;
	int t = 4;
	printf("%4x\n",( a & ~MASK ) | ( t << SHIFT ) );
}
