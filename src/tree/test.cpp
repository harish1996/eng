#include <iostream>
#include <string>

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

int main()
{
	std::cout<<first_dirname("afdsfh/dsgsdrewr/sgsg/weret/dsfdsf")<<endl;
	cout<<first_dirname("afjksdhf")<<endl;
	cout<<first_dirname("a/c")<<endl;
	cout<<first_dirname("asfsf/fsf/sdfsdf/sdfds/dsfds/sdfs/dsdfsd/sfddsf/asd")<<endl;
}