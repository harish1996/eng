#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char functionList[5][20] = {"add", "commit", "init"};
int functions = sizeof(functionList);



void add_redirect(int argc, char** argv) {
	printf("Inside add function\n Args: \n");
	for(int i = 0; i < argc; i++) {
		printf("%s ", argv[i]);
	}
}

void commit_redirect(int argc, char** argv) {
	printf("Inside commit function\n Args: \n");
	for(int i = 0; i < argc; i++) {
		printf("%s ", argv[i]);
	}
}

void init_redirect() {
	printf("Inside init function");
}


void main(int argc, char** argv) {
	int i = 0;
	int command = -1;

	if(argc >= 3) {
		for(i = 0; i < functions - 1; i++) {
			if(strcmp(functionList[i], argv[2]) == 0) {
				command = i;
			}
		}
		char** newArgList;
		newArgList = (char**)malloc((argc - 3)*sizeof(char*));
		for(i = 0; i < argc - 3; i++) {
			newArgList[i] = (char*)malloc(20*sizeof(char));
		}
		for(i = 0; i < argc - 3; i++) {
			for(int j = 0; j < 20; j++) {
				newArgList[i][j] = argv[i + 3][j];
			}
			printf("%s\n", newArgList[i]);
		}
		// switch case to navigate to the respective functions
		switch(command) {
			case 0 : add_redirect(argc - 3, newArgList); break; 
			case 1 : commit_redirect(argc - 3, newArgList); break;
			case 2 : init_redirect(); break;
			default: printf("\nInvalid Command! ");
		 	 	 exit(-1);
		}
	}
	else {
		printf("Invalid Command line argument! \n");
		exit(-1);
	}

}