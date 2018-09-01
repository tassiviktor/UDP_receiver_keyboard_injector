#define SBUFLEN 1024
#define PACKETLEN 508
typedef int (*DatagramHandler)(char[SBUFLEN]);

int sockfd;

int sock_initialize(int port);
int sock_listen(DatagramHandler d);