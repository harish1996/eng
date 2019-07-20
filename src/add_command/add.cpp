#include "add.h"


/****************** TEMP FUNCTIONS
 *** functions that will be used when the actual functions are not ready
 */
#define T_OBJECT "8f8e85330221ad054d67516331c5bfbd117aa3c9"

int get_current_tree( TREE* tree )
{
	tree->open_tree(T_OBJECT);
	return 0;
}
/****************** END ************/


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


std::string new_path( std::string dirpath, std::string filename )
{
	if( dirpath[ dirpath.size() - 1 ] != '/' )
		dirpath += '/';
	return dirpath+filename;
}

int STAGE::add_all_files( std::string filepath, TREE *tree )
{
	// nftw( filepath.c_str(), update_wrapper, 20, 0 );
	DIR *iterator = opendir( filepath.c_str() );
	struct dirent *tmp;

	tmp = readdir( iterator );
	while( tmp ){

		if( tmp->d_type == DT_DIR ){
			// std::cout<< tmp->d_name<<std::endl;
			if( (std::string)tmp->d_name == "." || (std::string)tmp->d_name == ".." ){
				tmp = readdir( iterator );				
				continue;
			}
			add_all_files( new_path(filepath, tmp->d_name ), tree );
		}
		else if( tmp->d_type == DT_REG )
			try_add( new_path( filepath, tmp->d_name ), tree );
		else
			return -EADDALL_NOTSUPP;

		tmp = readdir( iterator );
	}

	return ADDALL_SUCCESS;
} 

int STAGE::_try_add_tree( std::string filepath, TREE *tree )
{
	std::string hash;
	std::string prev_hash;
	int ret;

	ret = tree->get_subtree( filepath, prev_hash );
	switch(ret){
		case GET_SUCCESS:
		case -EGET_NO_OBJECT:
		case -EGET_NO_SUBDIR:
		case -EGET_NO_ENTRY:
			add_all_files( filepath, tree );
			return TAT_SUCCESS;
		default:
			return -ETAT_GET_FAIL;
	}

	return ret;

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
				return TA_SUCCESS;
			case -EGET_INVNAME:
			case -EGET_TYPE:
				_try_add_tree( filepath, tree );
				return TA_SUCCESS;
			default:
				return -ETA_GET_FAIL;
		}
	}

	ret = obj.hash_filecontents( filepath );
	if( ret != 0 )
		return -ETA_HASH_FAIL;

	hash = obj.get_hash();

	if( hash == prev_hash )
		dontupdate( filepath );
	else
		update( filepath );

	return TA_SUCCESS;
}

int DEFAULT_ADD( std::vector<std::string> filelist )
{
	TREE tree;
	STAGE stager;
	int tmp;
	auto begin = filelist.cbegin();
	auto end=filelist.end();

	tmp = get_current_tree( &tree );
	for( ; begin != end; begin++ ){
		tmp = stager.try_add( *begin, &tree );
		switch(tmp){
			case TA_SUCCESS:
				continue;
		}
	}

	stager.flush();
	return 0;
}