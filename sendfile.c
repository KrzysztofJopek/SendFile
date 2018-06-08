#include <stdio.h>
#include "send.h"
#include "receive.h"
#include "connect.h"
#include <unistd.h>
#include <stdlib.h>


void printHelp();

int main(int argc, char* argv[])
{
	int c;
	int iflag = 0;
	int Sflag = 0;
	int Rflag = 0;
	int sflag = 0;
	int cflag = 0;
	int fflag = 0;
	char* ip = NULL;
	int port = 1233;
	char* filename;
	if(argc < 3){
		printHelp();
		exit(0);
	}

	while((c = getopt(argc, argv, "i:p:SRscf:")) != -1){
		switch(c){
			case 'i':
				iflag++;
				ip = optarg;
				break;
			case 'p':
				port = atoi(optarg);
				if(port<1024 || port>65535){
					fprintf(stderr, "Use valid port from range 1024-65535\n");
					exit(2);
				}
				
				break;
			case 'S':
				if(Rflag){
					fprintf(stderr, "You can't use R and S flag together\n");
					exit(2);
				}
				Sflag++;
				break;
			case 'R':
				if(Sflag){
					fprintf(stderr, "You can't use R and S flag together\n");
					exit(2);
				}
				Rflag++;
				break;
			case 's':
				if(cflag){
					fprintf(stderr, "You can't use s and c flag together\n");
					exit(2);
				}
				sflag++;
				break;
			case 'c':
				if(sflag){
					fprintf(stderr, "You can't use s and c flag together\n");
					exit(2);
				}
				cflag++;
				break;
			case 'f':
				fflag++;
				filename = optarg;
				break;
			case '?':
				if(optopt == 'i' || optopt == 'f'){
					fprintf(stderr, "Option '-%c' requiers argument\n", optopt);
					exit(2);
				}
				else{
					fprintf(stderr, "Unknown option\n");
					exit(2);
				}
		}
				
	}
	if(!cflag && !sflag){
		fprintf(stderr, "You have to specify server(-s) / client(-c)\n");
		exit(2);
	}
	if(!Rflag && !Sflag){
		fprintf(stderr, "You have to specify Send(-S) / Receive(-R)\n");
		exit(2);
	}
	if(!iflag && cflag){
		fprintf(stderr, "Client has to specify servers IP(-i IP)\n");
		exit(2);
	}
	if(Sflag && !fflag){
		fprintf(stderr, "Send option needs filename(-f FILENAME)\n");
		exit(2);
	}

	int sockfd;
	if(sflag)
		sockfd = startServer(port);
	else
		sockfd = startClient(port, ip);

	if(Sflag)
		sendFile(sockfd, filename);
	else
		reciveFile(sockfd);

	return 0;
}
 
void printHelp(){
	printf("Flags:\n");
	printf("-s - run as TCP server\n");
	printf("-c - run as TCP client\n\n");
	printf("-S - send file\n");
	printf("-R - receive file\n\n");
	printf("-i IP - ip adress of the server\n");
	printf("-p PORT - set port\n");
	printf("-f FILENAME - name of the file to send\n");
}
