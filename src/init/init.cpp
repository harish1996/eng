#include<stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<stdlib.h>
#include<unistd.h>


int init() {
int check = -2;

if((check = mkdir(".eng",0777)) == -1) {
	printf("eng folder already exists! \n");
	exit(-1);
}
chdir(".eng");
if((check = mkdir("objects", 0777)) == -1) {
	printf("Cannot create objects folder\n");
	exit(-1);
}
if((check = mkdir("refs", 0777)) == -1) {
	printf("Cannot create refs folder\n");
	exit(-1);
}
if((check = mkdir("branches", 0777)) == -1) {
	printf("Cannot create branches folder\n");
	exit(-1);
}
FILE* fp;
if((fp = fopen(".config", "w")) == NULL) {
	printf("Cannot create .config file \n");
	exit(-1);
}

fclose(fp);

if((fp = fopen("HEAD", "w")) == NULL) {
	printf("Cannot create HEAD file \n");
	exit(-1);
}

fclose(fp);

return 0;
}




int main() {
	init();
}