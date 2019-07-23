#include "add.h"

int STAGE::change_staging_file( std::string s_file )
{
	staging_file = s_file;
	return 1;
}

static int extract_info( std::string str, std::string& name, std::string& hash )
{
	int tmp = str.find_first_of(1);
	name = str.substr(0,tmp);
	hash = str.substr(tmp+1);
	return 0;
}

#define BUF_SIZE 256
void STAGE::populate()
{
	char line[BUF_SIZE];
	std::ifstream stream( staging_file.c_str() );
	if( ! stream.is_open() )
		return;
	std::string line_str;
	std::string name,hash;
	bool end= false;

	while( !end ){
		stream.getline( line, BUF_SIZE );
		if( stream.good() ){
			line_str += line;
			// line_str >> name >> (char)0 >> hash; 
			if( ! line_str.empty() ){
				extract_info( line_str, name, hash );
				staged.insert( std::pair<std::string,std::string>(name,hash) );
			}
			line_str.clear();
		}
		else if( stream.eof() ){
			line_str += line;
			if( ! line_str.empty() ){
				extract_info( line_str, name, hash );
				staged.insert( std::pair<std::string,std::string>(name,hash) );
			}
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

	// while( !end ){
	// 	stream >> name >> (char)0 >> hash >> std::endl;
	// 	if( stream.eof() ){
	// 		end = true;
	// 	}
	// }
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
		stream<< it->first << (char)1 << it->second << std::endl;
	}

	stream.close();
	return 0;
}

void STAGE::update( std::string filepath , std::string hash )
{
	// std::cout<<"Updated "+filepath+"\n";
	staged.insert(std::pair<std::string,std::string>(filepath, hash));
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

}

void STAGE::dontupdate( std::string filepath, std::string hash )
{
	// std::cout<<"Not updated "+filepath+" with hash"+hash+" \n";
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
				ret = obj.create_blob_object( filepath );
				update( filepath, obj.get_hash() );
				return TA_SUCCESS;
			case -EGET_INVNAME:
			case -EGET_TYPE:
				_try_add_tree( filepath, tree );
				return TA_SUCCESS;
			default:
				return -ETA_GET_FAIL;
		}
	}

	ret = obj.create_blob_object( filepath );
	if( ret == -3 )
		return -ETA_HASH_FAIL;

	hash = obj.get_hash();

	if( hash == prev_hash )
		dontupdate( filepath, hash );
	else
		update( filepath, hash );

	return TA_SUCCESS;
}

int STAGE::try_remove( std::string filepath )
{
	auto it = staged.find( filepath );
	if( it != staged.end() ){
		staged.erase(it);
		return TR_SUCCESS;
	}
	else
		return -ETR_EXIST;

}

int DEFAULT_ADD( std::vector<std::string> filelist )
{
	TREE tree;
	STAGE stager;
	int tmp;
	auto begin = filelist.cbegin();
	auto end=filelist.end();

	tmp = get_current_tree( tree );
	if( tmp != GCT_SUCCESS )
		return -EDEFAULT_ADD_CURRENT_TREE;
	for( ; begin != end; begin++ ){
		tmp = stager.try_add( *begin, &tree );
		switch(tmp){
			case TA_SUCCESS:
				continue;
			default:
				return -EDEFAULT_TRY_ADD_FAIL;
		}
	}

	stager.flush();
	return 0;
}

int DEFAULT_RMCACHE( std::vector<std::string> filelist )
{
	// TREE tree;
	STAGE stager;
	int tmp;
	auto begin = filelist.cbegin();
	auto end=filelist.end();

	// tmp = get_current_tree( &tree );
	for( ; begin != end; begin++ ){
		tmp = stager.try_remove( *begin);
		switch(tmp){
			case TR_SUCCESS:
				continue;
		}
	}

	stager.flush();
	return 0;
}