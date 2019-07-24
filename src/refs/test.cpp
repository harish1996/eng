#include"branch.h"

int main (int argc, char* argv[]){

	if(argc != 3){
		printf("command line error\n");
	}else{
		
		std::string f,p;
		int t,s;
		s = writeHEAD( argv[1]);

		p = getHEAD();
	
		f = read_branch((std::string)p);
		printf("%s\n",f.c_str() );
		
		 t = write_branch(f,argv[2]);

	}
	return(0);
}
