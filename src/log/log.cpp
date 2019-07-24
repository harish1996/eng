#include "log.h"


int print_log_recurse(std::string &commit_hash){

	commit cobj(commit_hash);
	if(!cobj.is_opened())
		return -EPLR_INVALID_COMMIT_HASH;
	
	std::cout << "Commit " << commit_hash << "\n";
	std::string commit_message, parents[2];
	cobj.get_message(commit_message);
	std::cout  << commit_message << "\n\n";

	int ret = cobj.get_commit_type();
	if(0 == ret)
		return PLR_SUCCESS;
	cobj.get_parents(parents);
	print_log_recurse(parents[0]);
	if(2 == ret)
		print_log_recurse(parents[1]);

	return PLR_SUCCESS;
}

int DEFAULT_LOG(){
	std::string commit_hash;
	commit cobj;
	int ret = get_current_commit(cobj);
	if(ret != GCC_SUCCESS)
		return -EDPL_NO_COMMIT;

	commit_hash = cobj.get_hash();

	print_log_recurse(commit_hash);

	return DPL_SUCCESS;
}