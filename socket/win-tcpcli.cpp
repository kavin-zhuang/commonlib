#include <WINSOCK2.H>
#include <stdio.h>

#define PORT	8080
#define HOSTIP	"192.168.10.137"

char gator_version[] = "VERSION 20\n";
char gator_display[] = "STREAMLINE\n";

unsigned char msg01[] = {0x01,0x9b,0x01,0x00,0x00};
unsigned char msg02[] = {0x00,0x42,0x00, 0x00,0x00 };
unsigned char msg03[] = { 0x00, 0x40, 0x00, 0x00, 0x00 };
unsigned char msg04[] = { 0x02, 0x00, 0x00, 0x00, 0x00 };

extern char msg411[];
extern char msg66[];
extern char msg64[];

void main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

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

	SOCKET fd = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(HOSTIP);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(PORT);

	connect(fd, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	// STEP 1: Magic exchange

	send(fd, gator_version, strlen(gator_version), 0);
	send(fd, gator_display, strlen(gator_display), 0);

	char recv_buffer[1024];

	int ret;

	ret = recv(fd, recv_buffer, 1024, 0);
	if (ret <= 0) {
		printf("channel down\n");
		goto end;
	}
	printf("[%d] recv %d bytes\n", __LINE__, ret);

	// STEP 2: requst xml
	send(fd, (char*)msg01, sizeof(msg01), 0);
	send(fd, (char*)msg411, strlen(msg411), 0);
	ret = recv(fd, recv_buffer, 1024, 0);
	if (ret <= 0) {
		printf("channel down\n");
		goto end;
	}
	printf("[%d] recv %d bytes\n", __LINE__, ret);

	// STEP 3: deliver xml
	send(fd, (char*)msg02, sizeof(msg02), 0);
	send(fd, (char*)msg66, strlen(msg66), 0);
	ret = recv(fd, recv_buffer, 1024, 0);
	if (ret <= 0) {
		printf("channel down\n");
		goto end;
	}
	printf("[%d] recv %d bytes\n", __LINE__, ret);

	ret = recv(fd, recv_buffer, 1024, 0);
	if (ret <= 0) {
		printf("channel down\n");
		goto end;
	}
	printf("[%d] recv %d bytes\n", __LINE__, ret);

	// STEP 4: ??
	send(fd, (char*)msg03, sizeof(msg03), 0);
	send(fd, (char*)msg64, strlen(msg64), 0);

	ret = recv(fd, recv_buffer, 1024, 0);
	if (ret <= 0) {
		printf("channel down\n");
		goto end;
	}
	printf("[%d] recv %d bytes\n", __LINE__, ret);

	do {
		ret = recv(fd, recv_buffer, 1024, 0);
		if (ret <= 0) {
			printf("channel down\n");
			goto end;
		}
		printf("[%d] recv %d bytes\n", __LINE__, ret);
	} while (ret == 1024);

	// STEP 5: start APC, now continue receive realtime data
	printf("5\n");
	send(fd, (char*)msg04, sizeof(msg04), 0);

	do {
		ret = recv(fd, recv_buffer, 1024, 0);
		if (ret <= 0) {
			printf("channel down\n");
			goto end;
		}
		printf("[%d] recv %d bytes\n", __LINE__, ret);
	} while (1);

end:
	closesocket(fd);

	WSACleanup();
}
