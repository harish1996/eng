#include "add.h"


int STAGE::change_staging_file( std::string s_file )
{
	staging_file = s_file;
	return 1;
}

#define BUF_SIZE 256
void STAGE::populate()
{
	char line[BUF_SIZE];
	std::ifstream stream( staging_file.c_str() );
	if( ! stream.is_open() )
		return;
	std::string line_str;
	bool end= false;

	while( !end ){
		stream.getline( line, BUF_SIZE );
		if( stream.good() ){
			line_str += line;
			staged.insert(line_str);
			line_str.clear();
		}
		else if( stream.eof() ){
			line_str += line;
			if( ! line_str.empty() )
				staged.insert(line_str);
			line_str.clear();
			end = true;
		}
		else if( stream.fail() && !stream.bad() ){
			line_str += line;
		}
		else{
			end = false;
		}

	}

	stream.close();

}

void STAGE::drop()
{

	staged.clear();
}

int STAGE::flush()
{
	std::ofstream stream( staging_file.c_str(), std::ofstream::out | std::ofstream::trunc );
	auto it= staged.cbegin(),end = staged.cend();

	for( ; it != end; it++ ){
		stream<< *it <<std::endl;
	}

	stream.close();
	return 0;
}

void STAGE::update( std::string filepath )
{
	// std::cout<<"Updated "+filepath+"\n";
	staged.insert(filepath);
}

void STAGE::dontupdate( std::string filepath )
{

	std::cout<<"Not updated "+filepath+"\n";
}

int STAGE::try_add( std::string filepath, TREE* tree )
{
	std::string hash;
	std::string prev_hash;
	OBJ obj;
	int ret;

	ret = tree->get_blob( filepath, prev_hash );
	if( ret != GET_SUCCESS ){
		switch(ret){
			case -EGET_NO_OBJECT:
			case -EGET_NO_SUBDIR:
			case -EGET_NO_ENTRY:
				update( filepath );
				return AFL_SUCCESS;
			default:
				return -EAFL_GET_FAIL;
		}
	}

	ret = obj.hash_filecontents( filepath );
	if( ret != 0 )
		return -EAFL_HASH_FAIL;

	hash = obj.get_hash();

	if( hash == prev_hash )
		dontupdate( filepath );
	else
		update( filepath );

	return AFL_SUCCESS;
}