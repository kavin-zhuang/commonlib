/*
 * tcpsvr.cpp
 *
 * 2016.10.12, by Jinfeng Zhuang <kavin.zhuang@gmail.com>
 *
 * BUG:
 *   1. listen() does not work
 */

#include <WINSOCK2.H>
#include <stdio.h>

#include <signal.h>

#include <process.h>

#define PORT	8080

#define CLIENT_BUFFER_LENGTH 1024

void ctrlhandler(int a)
{
	printf("user interrupt\n");
	exit(0);
}

void cli_thread(void *arg)
{
	int ret;

	int fd = (int)arg;

	printf("(ID: 0x%x)\n", GetCurrentThreadId());

	char *buffer = (char*)malloc(CLIENT_BUFFER_LENGTH);
	if (NULL == buffer) {
		printf("malloc %d bytes for client failed\n");
		goto end;
	}

	while (1) {
		ret = recv(fd, buffer, CLIENT_BUFFER_LENGTH, 0);
		if (ret <= 0) {
			printf("client channel down\n");
			goto end;
		}
	}

end:
	if (fd >= 0) {
		closesocket(fd);
	}
	if (buffer) {
		free(buffer);
	}
}

void main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	printf("(ID: 0x%x)\n", GetCurrentThreadId());

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return;
	}

	signal(SIGINT, ctrlhandler);

	SOCKET fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		printf("socket create failed\n");
		goto end;
	}

	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);

	int ret = bind(fd, (sockaddr*)&addr, sizeof(addr));
	if (ret < 0) {
		printf("port(%d) has been used\n", PORT);
		goto end;
	}

	ret = listen(fd, 2);
	if (ret < 0) {
		printf("listen failed\n");
		goto end;
	}

	printf("Server Listening\n");

	int clifd;
	SOCKADDR_IN cliaddr;

	/* 必须赋值为可接受长度，否则accept失败 */
	int addrlen = sizeof(cliaddr);

	while (1) {
		clifd = accept(fd, (sockaddr*)&cliaddr, &addrlen);
		if (clifd < 0) {
			printf("accept failed\n");
			goto end;
		}

		printf("Client from %s\n", inet_ntoa(cliaddr.sin_addr));

		_beginthread(cli_thread, 0x1000, (void*)clifd);
	}

end:
	closesocket(fd);

	WSACleanup();
}
