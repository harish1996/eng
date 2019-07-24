#include "branch.h"


int DEFAULT_CREATE_BRANCH(std::string branch_name){
	std::string str,head ;
	int ret;
	char type;

	ret = getHEAD(type, head);
	if( ret != GETHEAD_SUCCESS ){
		return -ECREATE_BRANCH_FAILURE;
	}
	
	if( type == BRANCH ){
		str = read_branch(head);		
	}
	else if(type == HASH ){
		str = head;
	}
	
	ret = write_branch(str , branch_name);
	if( ret != 0 ){
		return -ECREATE_BRANCH_FAILURE;
	}

	return CREATE_BRANCH_SUCCESS;
}

