#include"refs.h"

int main (int argc, char* argv[]){

	//char name[30];
	// printf("enter file name\n");
	// scanf("%s", name);
	if(argc == 1){
		printf("command line error\n");
	}else{
		char * f,*p ;
		int t,s;
		s = writeHEAD( (char*)argv[1]);

		p = getHEAD();
	
		f = read_branch((char*)p);
		printf("%s\n",f );

		// t = write_branch(f,(char*)f);
	

	 	free(f);
	 	free(p);
	}
	return(0);
}