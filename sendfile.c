#include <stdio.h>
#include "send.h"
#include "receive.h"
#include "connect.h"
#include <unistd.h>
#include <stdlib.h>



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

	while((c = getopt(argc, argv, "i:p:SRscf:")) != -1){
		switch(c){
			case 'i':
				iflag++;
				ip = optarg;
				break;
			case 'p':
				port = atoi(optarg);
				if(!port){
					fprintf(stderr, "Use valid port\n");
					exit(2);
				}
				break;
			case 'S':
				if(Rflag){
					fprintf(stderr, "You can't use R and S flag together\n");
					exit(1);
				}
				Sflag++;
				break;
			case 'R':
				if(Sflag){
					fprintf(stderr, "You can't use R and S flag together\n");
					exit(1);
				}
				Rflag++;
				break;
			case 's':
				if(cflag){
					fprintf(stderr, "You can't use s and c flag together\n");
					exit(1);
				}
				sflag++;
				break;
			case 'c':
				if(sflag){
					fprintf(stderr, "You can't use s and c flag together\n");
					exit(1);
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
					exit(3);
				}
				else{
					fprintf(stderr, "Unknown option\n");
					exit(3);
				}
		}
				
	}
	if(!cflag && !sflag){
		fprintf(stderr, "You have to specify server(-s) / client(-c)\n");
		exit(4);
	}
	if(!Rflag && !Sflag){
		fprintf(stderr, "You have to specify Send(-S) / Receive(-R)\n");
		exit(4);
	}
	if(!iflag && cflag){
		fprintf(stderr, "Client has to specify servers IP(-i IP)\n");
		exit(4);
	}
	if(Sflag && !fflag){
		fprintf(stderr, "Send option needs filename(-f FILENAME)\n");
		exit(4);
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
