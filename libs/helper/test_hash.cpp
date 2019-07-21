#include "hash.h"
#include <iostream>
#include <stdio.h>
#include <iomanip>

int main()
{
	std::string hash("abababababba1234098765432109aefcdabdabae");
	unsigned char h_num[20];

	int ret;
	
	std::cout<< std::hex << std::setw(3)<< std::setfill('0');
	ret = hash_strtonum( hash, h_num );
	for( int i=0; i<20; i++ ){
		std::cout<< std::setw(2)<< std::setfill('0')<< (int)h_num[i] ;
	}
	std::cout<< std::endl;
	std::cout<< hash<< std::endl;
}


