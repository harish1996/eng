#include"refs.h"
/**
* @brief read branch reads a branch refrence and return a hash value
* @param branch name of having HASH
* @return 40 digits long string HASH value
**/
std::string read_branch(const std::string& s) {

	/**
	** path to get hash value argument of function contaion current branch name 
	*/
	std::string str2,str1 = ".eng/branches/" ;
	char * buffer;
	buffer = (char*)malloc(41);
	str2 = str1 + s;
	FILE *fp ;
	fp = fopen(str2.c_str(), "r");
	if (fp == NULL){
		str2.clear();
		return str2;
	}
	else{
			/*
			** SHA-1 is a cryptographic hash function which takes an input  and produces 20-byte has
			 value known as a message digest – typically rendered as a hexadecimal number, 40 digits long.
			*/
        	fread(buffer, 40, 1, fp); 
	}
	fclose(fp);
	buffer[40] = 0;
	/*
	** character pointer to string type cast
	*/
	return((std::string)buffer);
}
/*
* @brief writes to an existing branch
* @param HASH value in that branch
* @param branch name of which we will write HASH
* @return 
*** 0 success
*** -1 error in opening file
*/
int write_branch(const std::string& hash, const std::string& branch_name) {

	std::string str1 = ".eng/branches/" ;
	std::string str2;
	str2 = str1 + branch_name;
	FILE *fp;
	fp = fopen(str2.c_str(),"w");
	if(fp == NULL)
		return -1;
	else{
		/*
		** string to char pointer conversion
		*/
		fwrite(hash.c_str(),40,1,fp);
		fwrite("\n",1,1,fp);
	}
	fclose(fp);
	return 0;
}

/*
*@brief function returns a hash value corresponding to HEAD with the corresponding branch(if any)
@return 
*** return branch name(head points to branch) : success 
*** return empty string : failure
*/
int getHEAD( char &head_type, std::string& head) {

	char *buffer,ch;
	FILE *fp;
	int count = 0;
	std::string str1 = "emptystring";
	buffer = (char*)malloc(100);
	fp = fopen(".eng/HEAD","r");
	if(fp == NULL) {
		head.clear();
		return -EGETHEAD_NOFILE;
	}
	else {
		head_type = fgetc(fp);
		while((ch=fgetc(fp)) != (EOF) && ch !='\n' && ch !='\0'){
			buffer[count] = ch;
			count++;	
		}
	}
	fclose(fp);
	buffer[count] = 0;
	head = buffer;
	return GETHEAD_SUCCESS;
}

/*
* @brief overwrites HEAD pointer to new value
* @param current branch
* return
***  0 success
*** -1 failure
*/
int writeHEAD(char head_type, const std::string& cur_branch) {

	FILE *fp;
	fp = fopen(".eng/HEAD", "w");
	if(fp == NULL)
		return -1;
	else {
		std::string branch = cur_branch + "\n";
		fwrite(&head_type, sizeof(char), 1, fp);
		fwrite(branch.c_str(), branch.length(), 1, fp);
	}
	fclose(fp);
	return 0;
}
