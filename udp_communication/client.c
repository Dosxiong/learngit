//////////////////////////////////////////////////////////////////////////  

///     COPYRIGHT NOTICE  

///     Copyright (c) 2016, 恒为科技股份有限公司  

///     All rights reserved.  

///  

/// @file             server.c 

/// @brief            本程序实现了udp通讯的client端

///  

///

///  

/// @version 1.0        

/// @author           熊锐  

/// @date             2016-4-7  

///  

///  

///     修订说明：程序初版 

//////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

#define PORT 7070
#define REMOTEHOST "192.168.3.34"
#define HOST "192.168.8.144"
#define MAXBUFSIZE 1024
#define MAXMSGSIZE 1024

void *receive_thread()
{
	int r_socket_descriptor = 0;
	struct sockaddr_in sin;
	int sin_len = 0;
	char message[MAXMSGSIZE];
	fd_set fds;
	struct timeval timeout;

	printf("thread start()\n");
	timeout.tv_sec = 2;
	timeout.tv_usec = 500000;
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.s_addr = inet_addr(HOST);
	sin_len = sizeof(sin);

	r_socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
	bind(r_socket_descriptor, (struct sockaddr *)(&sin), sizeof(sin));

	while(1)
	{
		FD_ZERO(&fds);
		FD_SET(r_socket_descriptor, &fds);
		printf("alive\n");
		switch(select(r_socket_descriptor+1, &fds, NULL, NULL, NULL))
		{
			case 0:
				printf("0\n");
				break;
			case -1:
				printf("-1\n");
				break;
			default :
				recvfrom(r_socket_descriptor, message, sizeof(message), 0, (struct sockaddr *)(&sin), &sin_len);
				printf("%s\n", message);
				break;
		}
	}
	printf("thread end\n");
}

int main(int argc, char **argv)
{
	int socket_descriptor = 0;
	int i = 0;
	char buf[MAXBUFSIZE];
	struct sockaddr_in address;
	int count = 0;
	pthread_t receive;

	pthread_create(&receive, NULL, receive_thread, NULL);
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(REMOTEHOST);
	address.sin_port = htons(PORT);

	socket_descriptor = socket(AF_INET, SOCK_DGRAM,0);

	for(i = 0; i < 20; i++)
	{
		sprintf(buf,"%d hello server", ( i));
		count = sendto(socket_descriptor, buf, sizeof(buf), 0, (struct sockaddr *)(&address), sizeof(address));
		printf("count:%d\n", count);
	}
	sleep(10);

	close(socket_descriptor);

	return 0;
}
