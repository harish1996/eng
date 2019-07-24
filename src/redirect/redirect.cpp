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

int push_redirect( int argc, char **argv )
{
	int ret = DEFAULT_PUSH();
	if( ret != 0 ) return -1;
	return 0;
}

int pull_redirect( int argc, char **argv )
{
	int ret = DEFAULT_PULL();
	if( ret != 0 ) return -1;
	return 0;
}

int update_origin_redirect( int argc, char **argv )
{
	int ret = DEFAULT_UPDATE_ORIGIN( argv[1] );
	if( ret != 0 ) return -1;
	return 0;	
}

int revert( int argc, char **argv )
{
	if( argc < 2 ){
		std::cerr<<"Expecting argument \n";
		return -1;
	}
	int ret = DEFAULT_REVERT( argv[1] );
	if( ret != 0 ) return -1;
	return 0;	
}

int help( int, char** );

struct commands available[]={
	{ "add", add_redirect, "Adds files to be staged. Args: List of files" },
	{ "commit", commit_redirect, "Commits the files that are staged. Args: Commit message" },
	{ "init", init_redirect, "Initializes the repository. Args: None" },
	{ "checkout", checkout_redirect, "Modifies the working area to a different version. Args: Commit hash or branch name" },
	{ "log", log_redirect, "Prints log. Args: None" },
	{ "branch", branch_redirect, "Creates a branch with the current working area. Args: Branch name" },
	{ "branch_list", list_branch_redirect, "Lists the branches available. Args: None" },
	{ "push", push_redirect, "Pushes the objects to the registered remote. Args: None" },
	{ "pull", pull_redirect, "Pulls the objects from the registered remote. Args: None" },
	{ "remote-update", update_origin_redirect, "Registers the remote. Args: Remote path" },
	{ "revert", revert, "Reverts the current branch to another version. Args: Commit hash of the target" },
	{ "help", help, "Prints the help which is this :). Args: None" }
};

int help( int argc, char **argv )
{
	int no_contents = sizeof(available)/sizeof(struct commands);
	std::cout<<"Available commands: \n";
	for( int i=0; i<no_contents; i++ ){
		std::cout<< available[i].name << " : " << available[i].help << "\n\n";
	}
	return 0;
}


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
		help( argc, argv );
		return -EREDIRECT_COMMAND_NOT_FOUND;
	}

}

