#ifndef _MD5CALC_H_
#define _MD5CALC_H_
#include <stdio.h>

unsigned char* getMD5(FILE* fp);
int compareMD5(unsigned char* md1, unsigned char* md2);

#endif
