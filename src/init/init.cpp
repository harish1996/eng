#include "init.h"

int init() {
	int check = -2;
	if((check = mkdir(".eng",0777)) == -1) {
		printf("eng folder already exists! \n");
		return -1;
	}
	chdir(".eng");
	if((check = mkdir("objects", 0777)) == -1) {
		printf("Cannot create objects folder\n");
		return -1;
	}
	if((check = mkdir("refs", 0777)) == -1) {
		printf("Cannot create refs folder\n");
		return -1;
	}
	if((check = mkdir("branches", 0777)) == -1) {
		printf("Cannot create branches folder\n");
		return -1;
	}
	chdir("branches");

	FILE* fp;
	if((fp = fopen("master", "w")) == NULL) {
		printf("Cannot create master file \n");
		return -1;
	}
	fclose(fp);

	chdir("..");
	if((fp = fopen(".config", "w")) == NULL) {
		printf("Cannot create .config file \n");
		return -1;
	}
	fclose(fp);
	if((fp = fopen("HEAD", "w")) == NULL) {
		printf("Cannot create HEAD file \n");
		return -1;
	}
	fclose(fp);
	return 0;
}
