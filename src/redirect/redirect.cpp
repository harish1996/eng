#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void main(int argc, char** argv) {
int i = 0, j = 0;
for(;i < argc; i++) {
for(j = 0; j < strlen(argv[i]); j++)
printf("%c",argv[i][j]);
}

}