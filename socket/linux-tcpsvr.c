/*
 *
 * ×ö³É¶ÌÁ´½Ó
 */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#define MONITOR_BUFFER_LENGTH   2048

#define MONITOR_SERVER_PORT 	5555

static void tcp_channel(void *arg)
{
	int ret;
	
	int fd = (int)arg;
	
	unsigned char *buffer = (unsigned char*)malloc(MONITOR_BUFFER_LENGTH);
	if(NULL == buffer) {
		printf("Malloc for TCP channel failed\n");
		goto exit;
	}

	while(1)
	{
		ret = recv(fd, buffer, MONITOR_BUFFER_LENGTH, 0);
		if(ret <= 0) {
			printf("TCP channel ERROR\n");
			goto exit;
		}
		else {
			printf("%s\n", buffer);
			ret = send(fd, "OK", 3, 0);
			if(-1 == ret) {
				printf("TCP send failed\n");
				goto exit;
			}
			else {
			    goto exit;
			}
		}
	}
	
exit:
	if(fd >= 0) {
		close(fd);
	}
	if(NULL != buffer) {
	    free(buffer);
	}
}

static pthread_t create_tcp_channel(int fd)
{
	pthread_t thread;
	
	pthread_create(&thread, NULL, tcp_channel, (void*)fd);
	
	return thread;
}

static void monitor(void *arg)
{
	struct sockaddr_in addr;
	socklen_t addrlen;
	
	fd_set readfds;
	
	struct sockaddr_in cliaddr;
	pthread_t thread_cli;
	
	int fd = -1;
	int maxfd;
    
    int ret;
    
    int clifd;
    
    unsigned char *buffer = (unsigned char *)malloc(MONITOR_BUFFER_LENGTH);
    if(NULL == buffer) {
        printf("ERROR: Not enough space for monitor.\n");
        goto exit;
    }

    while(1)
    {
    	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(-1 == fd) {
			printf("ERROR: TCP socket create failed.\n");
			goto rebuid_server;
		}
		
		printf("\nSERVER CREATED(%d).\n", fd);

		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(MONITOR_SERVER_PORT);
		addr.sin_addr.s_addr = INADDR_ANY;
		
		ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
		if(0 != ret) {
			printf("ERROR: monitor bind failed!\n");
			goto rebuid_server;
		}
		
		printf("\nSERVER BINDING.\n");
		
		ret = listen(fd, 1);
		if(-1 == ret) {
			printf("ERROR: monitor listen failed!\n");
			goto rebuid_server;
		}
		
		printf("\nSERVER LISTENNING.\n");
    	
		while(1)
		{
			// WARNING: the CPU directly point to here
			// create new client
			clifd = accept(fd, &cliaddr, &addrlen);

			if(-1 == clifd) {
				printf("ERROR: SERVER accept, Rebuild\n");
				goto rebuid_server;
			}
			
			create_tcp_channel(clifd);
		}
		
rebuid_server:
		if(fd >= 0) {
			close(fd);
			
		}
		sleep(1);
    }
    
exit:
	if(buffer) {
		free(buffer);
	}
	if(fd >= 0) {
		close(fd);
	}
}

void server_stop(int signo)
{
    printf("oops!!! exit!!!\n");
    exit(0);
}

void main(void)
{
	pthread_t thread;

	printf("----------server entry-----------\n");	
	
	signal(SIGINT, server_stop);

	int ret = pthread_create(&thread, NULL, monitor, NULL);
	if(0 != ret) {
		printf("create thread failed: %s\n", strerror(ret));
		return;
	}

	pthread_join(thread, NULL);

	printf("------server exit------\n");
}
