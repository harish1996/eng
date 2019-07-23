#include "objects.h"	

#define BUF_SIZE 256
#define FILE_NOT_OPENED -1
#define SUCCESS 0

int OBJ::copy_file( ogzstream &ofile, std::istream &s )
{
	char buf[BUF_SIZE];
	size_t size;
	gzstreambuf *ptr;

	ptr = ofile.rdbuf();	
	if( !ptr->is_open() )
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

#define OBJ_EXISTS -2
#define UNABLE_TO_CREAT_FILE -3

int OBJ::create_object( std::istream &s, char type )
{
	std::fstream file;
	std::stringstream t;
	std::string object_path; 

	if( type < BLOB_OBJECT && type > TREE_OBJECT )
		return -1;

	t.put( type );
	this->type = type; 
	update( t );
	update( s );
	s.clear();
	s.seekg(0, s.beg);
	hash = final();
	object_path = DEFAULT_OBJECTS_PATH+hash;
	file.open( object_path.c_str(), std::ifstream::in );
       	if( file.is_open() ){
		file.close();
		// std::cout<<"Hash already exists\n";
		return OBJ_EXISTS;
	}
	else{
		ogzstream ofile( object_path.c_str() );
		if( ! ofile.rdbuf()->is_open() )
			return UNABLE_TO_CREAT_FILE; 
		ofile<<type;
		copy_file( ofile, s );
		ofile.close();
		return SUCCESS;
	}
}

int OBJ::hash_contents( std::istream &s, char type )
{
	std::fstream file;
	std::stringstream t;
	std::string object_path; 

	// std::cout<< s;
	if( type < BLOB_OBJECT && type > TREE_OBJECT )
		return -1;

	t.put( type );
	this->type = type; 
	update( t );
	update( s );
	s.clear();
	s.seekg(0, s.beg);
	this->hash = final();
	return 0;
}

#define FILENOT_EXISTS -4
int OBJ::create_blob_object( const std::string filename )
{
	std::ifstream file( filename.c_str() );
	if( file.is_open() )
		return create_object( file, BLOB_OBJECT );	
	else
		return FILENOT_EXISTS;
}

int OBJ::hash_filecontents( const std::string filename )
{
	std::ifstream file( filename.c_str() );
	if( file.is_open() )
		return hash_contents( file, BLOB_OBJECT );	
	else
		return FILENOT_EXISTS;	
}

std::string OBJ::get_hash()
{
	return hash;
}

int OBJ::get_new_object( const std::string &hash )
{
	std::string obj_path;

	if( this->hash == hash )
		return 1;
	this->hash = hash;
	gzstreambuf *ptr;
	ptr = objstream.rdbuf();
	if( ptr->is_open() ){
		objstream.close();
	}
	obj_path = DEFAULT_OBJECTS_PATH+hash;
	objstream.open( obj_path.c_str() );
	if( ! ptr->is_open() ){
		return -1;
	}
	
	objstream.get( type );

	if( type < BLOB_OBJECT || type > TREE_OBJECT ){
		objstream.close();
		return -2;
	}

	return 1;
}

int OBJ::discard_object()
{
	// if( objstream == 0 )
	// 	return 1;
	hash.clear();
	gzstreambuf *ptr = objstream.rdbuf();
	if( ptr == NULL )
		return 1;
	if( !ptr->is_open() )
	       return 1;

	objstream.close();
	return 1;
}

int OBJ::read_object( char *buf, size_t size )
{
	objstream.read( buf, size );
	return objstream.gcount();
}

int OBJ::object_type()
{
	return type;
}

int OBJ::cat_blob_object( )
{
	if( hash.empty() )
		return -1;

	if( type != BLOB_OBJECT )
		return -3;


	char buf[BUF_SIZE+1];

	read_object( buf, BUF_SIZE );

	while( !objstream.eof() && !objstream.fail() ){
		buf[BUF_SIZE] = 0;
		std::cout<<buf;
		read_object( buf, BUF_SIZE );
	}

	buf[objstream.gcount()] = 0;
	std::cout<<buf;

	return 1;
}

int OBJ::create_file( std::string filename )
{
	if( hash.empty() )
		return -1;

	if( type != BLOB_OBJECT )
		return -3;

	std::ofstream file( filename.c_str() );

	char buf[BUF_SIZE+1];

	read_object( buf, BUF_SIZE );

	while( !objstream.eof() && !objstream.fail() ){
		buf[BUF_SIZE] = 0;
		file<<buf;
		read_object( buf, BUF_SIZE );
	}

	buf[objstream.gcount()] = 0;
	file<<buf;

	return 1;	
}

/**
 * @brief Creates file with `filename` from the given blob's hash
 * 
 * @param hash Hash of the blob
 * @param filename Name of the target file
 * 
 * @return CFB_SUCCESS on success,
 *	-ECFB_NOOBJ if hash doesnt exist
 *	-ECFB_INVOBJ if the object is invalid
 *	-ECFB_NOTBLOB if the object is not a blob
 *	-ECFB_BUG if there is a bug in this function
 */
int OBJ::create_file_from_blob( std::string& hash, std::string& filename )
{
	int ret;
	discard_object();
	ret = get_new_object( hash );
	if( ret != 1 )
		return ret;
	if( object_type() != BLOB_OBJECT )
		return -ECFB_NOTBLOB;
	ret = create_file( filename );
	switch(ret){
		case 1:
			return CFB_SUCCESS;
			break;
		case -1:
		case -3:
			return -ECFB_BUG;
			break;
	}

	return CFB_SUCCESS;
}


/*
void usage( char *arg )
{
	std::cout<<arg<<" <cmd> <infile>\n";
	std::cout<<"cmd can be r/w\n";
}

int main( int argc, char* argv[] )
{
	int ret;
	if( argc != 3 ){
		usage(argv[0]);
		exit(-1);
	}

	std::string filename( argv[2] );
	OBJ obj;

	if( argv[1][0] == 'w' ){
		ret = obj.create_blob_object( filename );
		if( ret == SUCCESS ){
			std::cout<<" object with name "<<obj.get_hash()<<" created\n";
		}
		else{
			std::cout<<" Failed\n";
		}
	}
	else if( argv[1][0] == 'r' ){
		ret = obj.get_new_object( filename );
		if( ret == 1 ){
			if( obj.object_type() == BLOB_OBJECT ){
				obj.cat_blob_object( );
			}
			else{
				std::cout<<"Unrecognized/Unsupported object format\n";
			}
		}
		else{
			std::cout<<"Not a proper object\n";
			usage(argv[0]);
		}
	}


	return 0;
}
*/
