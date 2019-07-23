#include"refs.h"

char* read_branch(char s[]){

	char  *buffer,str1[] = ".eng/branches/" ;

	int a,b,c;

	a = strlen(str1);
	b = strlen(s);
	c = a+b+1;

	char *str2;

	str2 = (char*)malloc(c); 
	buffer = (char*)malloc(41);
	strcpy(str2,str1);

	strcat(str2,s);

	FILE *fp ;

	fp = fopen(str2, "r");
	if (fp == NULL)
	{
		printf("error\n");
		return '\0';
	}
	else{
		
        	fread(buffer, 40, 1, fp); 
        	//cout << buffer; 
 
	}
	fclose(fp);
	buffer[40] = 0;
	//printf("%s\n",buffer );
	return(buffer);

}

int write_branch(char *hash, char *branch_name){

	char str1[] = ".eng/branches/" ;

	int a,b,c;

	a = strlen(str1);
	b = strlen(branch_name);
	c = a+b+1;

	char *str2;

	str2 = (char*)malloc(c); 
	//buffer = (char*)malloc(41);
	strcpy(str2,str1);
	strcat(str2,branch_name);

	FILE *fp;
	fp = fopen(str2,"w");
	if(fp == NULL){
		printf("error code\n");
		return '\0';
	}else{
		fwrite(hash,40,1,fp);
	}
	fclose(fp);

	return 0;
}


char * getHEAD(){
	char *buffer,ch;
	FILE *fp;
	int count = 0;

	buffer = (char*)malloc(100);
	fp = fopen(".eng/HEAD","r");

	if(fp == NULL){
		printf("errorcode\n");
		return NULL;
	}else{
		while((ch=fgetc(fp)) != (EOF) && ch !='\n' && ch !='\0'){
			buffer[count] = ch;
			count++;
			// printf("%c\n",ch );
		}
	}
	fclose(fp);
	buffer[count] = 0;
	return(buffer);
}

int writeHEAD(char *cur_branch){

	
	FILE *fp;
	fp = fopen(".eng/HEAD","w");
	if(fp == NULL){
		printf("error code\n");
		return '\0';
	}else{
		fwrite(cur_branch,100,1,fp);
	}
	fclose(fp);
	return 0;
}