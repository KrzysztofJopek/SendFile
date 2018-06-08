#include "md5calc.h"
#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHUNK 512
#define NCHUNKS 10
#define _DEBUG_

unsigned char* getMD5(FILE* fp)
{
	rewind(fp);
	MD5_CTX c;
	unsigned char* md = (unsigned char*)malloc(MD5_DIGEST_LENGTH);
	if(!md){
		printf("ERROR: malloc");
		exit(7);
	}
	char buf[CHUNK*NCHUNKS];
	int i;

	MD5_Init(&c);
	while((i=fread(buf, sizeof(char), NCHUNKS*CHUNK, fp)) > 0){
		MD5_Update(&c, buf, (unsigned long)i);
	}
	MD5_Final(md, &c);
	rewind(fp);
	return md;
}

int compareMD5(unsigned char* md1, unsigned char* md2)
{
	int res = 1;
	for(int i=0; i<MD5_DIGEST_LENGTH; i++){
		if(*md1++ != *md2++){
			res = 0;
			break;
		}
	}
	return res;
}
