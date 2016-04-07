//////////////////////////////////////////////////////////////////////////  

///     COPYRIGHT NOTICE  

///     Copyright (c) 2016, 恒为科技股份有限公司  

///     All rights reserved.  

///  

/// @file             server.c 

/// @brief            本程序实现了udp通讯的server端

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
#include<pthread.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/select.h>
#include<arpa/inet.h>

#define PORT 7070
#define REMOTEHOST "192.168.8.144"
#define HOST "192.168.3.34"
#define MAXMSGSIZE 1024
#define MAXBUFSIZE 1024

void rep_sendto(struct sockaddr_in addr)
{
    int r_socket_descriptor = 0;
    struct sockaddr_in address;
    char buf[MAXBUFSIZE];

    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = addr.sin_addr.s_addr;
    address.sin_port = htons(PORT);


    r_socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);

    sprintf(buf,"client answer");
    sendto(r_socket_descriptor, buf, sizeof(buf), 0, (struct sockaddr *)(&address), sizeof(address));

}


int main(int argc, char **argv)
{
    int socket_descriptor = 0;
    struct sockaddr_in sin, r_sin;
    int sin_len = 0,r_sin_len = 0;
    char message[MAXMSGSIZE];
    fd_set fds;
    struct timeval timeout;

    timeout.tv_sec = 2;
    timeout.tv_usec = 500000;
    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr = inet_addr(HOST);
    sin_len = sizeof(sin);
    bzero(&r_sin, sizeof(r_sin));
    r_sin_len = sizeof(r_sin);

    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    bind(socket_descriptor, (struct sockaddr *)(&sin), sizeof(sin));

    while(1)
    {

        FD_ZERO(&fds);
        FD_SET(socket_descriptor, &fds);
        switch(select(socket_descriptor+1, &fds, NULL, NULL, NULL) > 0)
        {
            case 0:
                printf("timeout\n");
                break;
            case -1:
                break;
            default :
                recvfrom(socket_descriptor, message, sizeof(message), 0, (struct sockaddr *)(&r_sin), &r_sin_len);
                printf("message:%s\n", message);
                printf("port:%d,addr:%s,len:%d,rsin_addr:%s\n", sin.sin_port, inet_ntoa(sin.sin_addr), sin_len, inet_ntoa(r_sin.sin_addr));
               // rep_sendto(sin);

                break;

        }
        /*recvfrom(socket_descriptor, message, sizeof(message), 0, (struct sockaddr *)(&sin), &sin_len);
          printf("message:%s\n", message);*/

    }

    close(socket_descriptor);

    return 0;
}

