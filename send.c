#include <stdio.h>
#include "connect.h"
#include "md5calc.h"
#include "send.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/limits.h>
#include <openssl/md5.h>

#define SIZE 512

int sendName(int sockfd, char* name);
int sendMD5(int sockfd, FILE* fp);
int getNameFromPath(char* path, char* name);


int sendFile(int sockfd, char* path)
{
	FILE* fd = fopen(path, "r");
	if(!fd){
		perror("Error: fopen");
		return 2;
	}
	char* buff = (char*)calloc(sizeof(char), SIZE);
	if(!buff){
		printf("Error: Failed to allocate memory");
		return 3;
	}
	
	int read = 0;
	//send filename, MD5 then file
	if(sendName(sockfd, path) < 0){
		perror("Error: sendName");
		return -1;
	}
	if(sendMD5(sockfd, fd) < 0){
		perror("Error: sendMD5");
		return -1;
	}
	while((read = fread(buff, sizeof(char), SIZE, fd)) > 0){
		if(send(sockfd, buff, read, 0) != read){
			perror("Error: send");
			return 4;
		}
	}
	free(buff);
	if(feof(fd)){
		printf("File sent");
	}
	if(ferror(fd)){
		perror("Error: read");
		fclose(fd);
		return -1;
	}
	fclose(fd);
	return 1;
}

//conver path to name and send it throught socket
int sendName(int sockfd, char* path)
{
	char* sname = (char*)calloc(sizeof(char), PATH_MAX);
	getNameFromPath(path, sname);
	int res = send(sockfd, sname, PATH_MAX*sizeof(char), 0);
	free(sname);
	return res;
}

//calculate MD5 and send it throught socket
int sendMD5(int sockfd, FILE* fp)
{
	unsigned char* md = getMD5(fp);
	int res =  send(sockfd, md, sizeof(unsigned char)*MD5_DIGEST_LENGTH, 0);
	rewind(fp);
	free(md);
	return res;
}

int getNameFromPath(char* path, char* name)
{
	char* c = path;
	int pos=-1;
	int i=0;
	for(; *c; i++){
		if(*c++ == '/')
			pos=i;
	}
	if(path[pos+1] != '\0'){
		strcpy(name, path+pos+1);
	}
	return 0;
}

