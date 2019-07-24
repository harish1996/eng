#include "redirect.h"

int add_redirect(int argc, char** argv) {

	std::vector<std::string> paths; 
	for( int i=1; i<argc; i++ )
		paths.push_back( argv[i] );

	int ret = DEFAULT_ADD( paths );
	if( ret != 0 )
		return -1;
	return 0;
}

int commit_redirect(int argc, char** argv) {

	std::string message;
	int ret;
	for( int i=1; i<argc; i++ )
		message += (std::string)argv[i] + (std::string)" ";
	
	ret = DEFAULT_COMMIT( message );
	if( ret != 0 )
		return -1;
	return 0;
}

int init_redirect( int argc, char **argv) {
	return init();
}

int checkout_redirect( int argc, char **argv){
	int ret = DEFAULT_CHECKOUT( argv[1] );
	if( ret ) return -1;
	return 0;
}

int log_redirect( int argc, char **argv ){
	int ret = DEFAULT_LOG();
	if(ret) return -1;
	return 0;
}

int branch_redirect( int argc, char **argv ){
	int ret = DEFAULT_CREATE_BRANCH( argv[1] );
	if(ret) return -1;
	return 0;
}

int list_branch_redirect( int argc, char **argv ){
	int ret = DEFAULT_LIST_BRANCHES();
}

struct commands available[]={
	{ "add", add_redirect },
	{ "commit", commit_redirect },
	{ "init", init_redirect },
	{ "checkout", checkout_redirect },
	{ "log", log_redirect },
	{ "branch", branch_redirect },
	{ "branch_list", list_branch_redirect }
};

#define EREDIRECT_COMMAND_NOT_FOUND 3

int redirect( int argc, char *argv[] )
{

	int i,j;
	int ret;
	if( argc < 2 ){
		std::cerr<<"No command found\n";
		return 0;
	}

	std::map< std::string, int (*)(int, char **)> command_map;
	int no_contents = sizeof(available)/sizeof(struct commands);
	char **new_argv = new char*[argc-1] ;
	std::string command;


	for( int i=0 ; i< no_contents ; i++ ){
		command_map.insert( std::pair<std::string, int (*)(int,char**)>(available[i].name,available[i].redirect_function) );
	}

	j=0;

	for(int i=0; i< argc; i++ ){
		if( i == 1 ) {
			command = argv[1];
			continue;
		}
		else{
			new_argv[j] = argv[i];
			j++; 
		}
	}

	// std::cout<< command << std::endl;

	auto it = command_map.find( command );
	if( it != command_map.end() ){
		ret = it->second( argc - 1, new_argv );
		if( ret != 0 ){
			std::cerr<<command<<" failed\n";
			return ret;
		}
		return 0;
	}
	else{
		std::cerr<<command<<" : command not found\n";
		return -EREDIRECT_COMMAND_NOT_FOUND;
	}

}

