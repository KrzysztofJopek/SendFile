#include <stdio.h>
#include <string.h>
#include "connect.h"
#include "md5calc.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/limits.h>
#include <openssl/md5.h>

#define SIZE 512

char* getName(char* name);
int getNameExtPos(char* name);
int checkMD5(unsigned char* md1, FILE* fp);
unsigned char* reciveMD5(int sockfd);


int reciveFile(int sockfd)
{
	char* name = (char*)calloc(sizeof(char), PATH_MAX);
	if(recv(sockfd, name, sizeof(char)*PATH_MAX, MSG_WAITALL) < 0){
		perror("Error: recive name");
		return 1;
	}
	char* vname = getName(name);
	FILE* fd = fopen(vname, "a+");
	if(!fd){
		perror("Error: fopen");
		return 2;
	}

	unsigned char* md = reciveMD5(sockfd);
	if(!md){
		perror("Error: reciveMD5");
		exit(4);
	}
	char* buff = (char*)calloc(sizeof(char), SIZE);
	if(!buff){
		perror("Error: Failed to allocate memory");
		return 3;
	}
	int read;
	while((read = recv(sockfd, buff, SIZE, 0)) > 0){
		fwrite(buff, sizeof(char), read, fd);
	}
	int res = checkMD5(md, fd);
	if(!res){
		printf("Warning: MD5 not matched\n");
	}
	else{
		printf("MD5 Checked\n");
	}
	fclose(fd);
	free(vname);
	free(buff);
	free(name);
	free(md);
	return 1;
}

//returns filenam(name+suffix) which is unical in given catalog
char* getName(char* name)
{
	char* buff = (char*)calloc(sizeof(char), PATH_MAX+1);
	char* nm = (char*)calloc(sizeof(char), PATH_MAX);
	char* ext = (char*)calloc(sizeof(char), PATH_MAX);
	int pos = getNameExtPos(name);
	strncpy(nm, name, pos);
	strcpy(ext, name+pos);
	strcpy(buff, name);
	int i=1;
	//Add number before extension
	while(access(buff, F_OK) != -1){
		sprintf(buff, "%s%d%s", nm, i, ext);
		i++;
	}
	free(nm);
	free(ext);
	return buff;
}

//return extension position in name
int getNameExtPos(char* name)
{
	int pos = 0;
	char c = *name;
	int i=1;
	for(; c!='\0'; i++){
		c=name[i];
		if(c == '.'){
			pos=i;
		}
	}
	if(pos==0)
		return i-1;
	return pos;
}
unsigned char* reciveMD5(int sockfd){
	unsigned char* md1 = (unsigned char*)calloc(sizeof(unsigned char), MD5_DIGEST_LENGTH);
	if(recv(sockfd, md1, sizeof(unsigned char)*MD5_DIGEST_LENGTH, MSG_WAITALL) < 0){
		perror("Error: recive name");
		free(md1);
		return NULL;
	}
	return md1;
}

int checkMD5(unsigned char* md1, FILE* fp)
{
	unsigned char* md2 = getMD5(fp);
	int res = compareMD5(md1, md2);
	free(md2);
	free(md1);
	return res;

}
