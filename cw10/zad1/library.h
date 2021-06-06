#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <string.h>
#include <poll.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define NICK_LEN 64
#define MSG_LEN 512
#define PING_TIME 10

struct Client 
{
    char nick [NICK_LEN];
    int socket;
    int rival; // rival's socket
    int isOnline; // 0 - no, 1 - yes

};