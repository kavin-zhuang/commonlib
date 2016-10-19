#include <stdio.h>

#include <sys/socket.h>

#define PORT    8080
#define HOSTIP  "192.168.10.134"

#define SEND_BUF_SIZE   1024

int main(void)
{
    printf("tcpcli, by jinfeng\n");

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr = inet_addr(HOSTIP);
    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(-1 == ret) {
        goto end;
    }

    unsigned char *buffer = (unsigned char*)malloc(SEND_BUF_SIZE);
    if(NULL == buffer) {
        goto end;
    }
    memset(buffer, 0, SEND_BUF_SIZE);

    ret = send(fd, buffer, strlen(buffer), 0);
    if(-1 == ret) {
        goto end;
    }

    while(1) {
        sleep(1);
    }

end:
    printf("end\n");
    if(fd >= 0) {
        close(fd);
    }
}
