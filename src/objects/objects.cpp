#include <libs/gzstream/gzstream.h>
#include <iostream>
#include <libs/sha1/sha1.h>
#include <string>
#include <cstdlib>

class OBJ : public SHA1
{
	std::string hash;
	int copy_file( std::fstream &ofile, std::istream &s );
public:
	OBJ(){};
	int create_object( std::istream &s );
	int create_object( const std::string filename );
	std::string get_hash();
	int get_new_object( std::string &hash );
};

#define BUF_SIZE 256
#define FILE_NOT_OPENED -1
#define SUCCESS 0

int OBJ::copy_file( std::fstream &ofile, std::istream &s )
{
	char buf[BUF_SIZE];
	size_t size;
	
	if( !ofile.is_open() )
		return FILE_NOT_OPENED;
	s.seekg( 0, s.end );
	size = s.tellg();
	s.seekg( 0, s.beg );

	while( size >= BUF_SIZE ){
		s.read( buf, BUF_SIZE );
		ofile.write( buf, BUF_SIZE );
		size -= BUF_SIZE;
	}
	if( size >= 0 ){
		s.read( buf, size );
		ofile.write( buf, size );
	}

	return SUCCESS;
}

#define OBJ_EXISTS 2
int OBJ::create_object( std::istream &s )
{
	std::fstream file;
	std::cout<<s.tellg()<<"\n";
	update( s );
	s.clear();
	s.seekg(0, s.beg);
	std::cout<<s.tellg()<<"\n";
	hash = final();
	file.open( hash.c_str(), std::ifstream::in );
       	if( file.is_open() ){
		file.close();
		std::cout<<"Hash already exists\n";
		return OBJ_EXISTS;
	}
	else{
		file.open( hash.c_str(), std::ofstream::out | std::ofstream::trunc );
		copy_file( file, s );
		file.close();
		return SUCCESS;
	}
}

#define FILENOT_EXISTS -1
int OBJ::create_object( const std::string filename )
{
	std::ifstream file( filename.c_str() );
	if( file.is_open() )
		return create_object( file );	
	else
		return FILENOT_EXISTS;
}

std::string OBJ::get_hash()
{
	return hash;
}

int OBJ::get_new_object( std::string &hash )
{
	this->hash = hash;
	return 1;
}

int usage( char *arg )
{
	std::cout<<arg<<" <infile>\n";
}

int main( int argc, char* argv[] )
{
	int ret;
	if( argc != 2 ){
		usage(argv[0]);
		exit(-1);
	}

	std::string filename( argv[1] );
	OBJ obj;
	ret = obj.create_object( filename );
	if( ret == SUCCESS ){
		std::cout<<" object with name "<<obj.get_hash()<<" created\n";
	}
	else{
		std::cout<<" Failed\n";
	}

	return 0;
}
