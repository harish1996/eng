#include <iostream>
#include <string>

using namespace std;

static int extract_info( std::string str, std::string& name, std::string& hash )
{
	int tmp = str.find_first_of(1);
	name = str.substr(0,tmp);
	hash = str.substr(tmp+1);
	return 0;
}

int main()
{
	std::string a,b,c;
	a = (string)"str"+(char)1+"hello";
	extract_info( a,b,c );
	cout<< a << b <<" "<< c; 
}