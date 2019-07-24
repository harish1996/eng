#include "branch.h"


int DEFAULT_CREATE_BRANCH(std::string& branch_name){
	std::string str ;
	getHEAD(head);
	str = read_branch(head);
	write_branch(str , branch_name);
}

