#include "library.h"

int server;
char* nick;

void localConnect(char* path)
{
    if ((server = socket(AF_UNIX,SOCK_STREAM,0)) == -1)
    {
        perror("Error while starting local socket (client)\n");
        exit(1);
    }

    struct sockaddr_un address;
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path,path);

    if (connect(server,(struct sockaddr*) &address,sizeof(struct sockaddr_un )) == -1)
    {
        perror("Error while connecting to local server\n");
        exit(1);
    }
}
void netConnect(int port)
{
    if ((server = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("Error while starting local socket\n");
        exit(1);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (connect(server,(struct sockaddr*)&address,sizeof(struct sockaddr_in)) == -1)
    {
        perror("Error while connecting to net server\n");
        exit(1);
    }
}
void joinServer()
{
    char* name;
    name = calloc(1,1 + strlen(nick));
    strcpy(name,nick);
    send(server,name,MSG_LEN,0);
}
int main(int argc, char** argv) 
{
    if (argc != 4)
    {
        perror("Wron number of arguments\n");
        exit(1);
    }

    // set the nick
    if (strlen(argv[1]) > NICK_LEN)
    {
        perror("Nick is too long !\n");
        exit(1);
    }
    else
    {
        nick = calloc(1,strlen(argv[1]) + 1);
        strcpy(nick,argv[1]);
    }

    // we connect to the right server
    if (strcmp(argv[2],"local") == 0 )
    {
        localConnect(argv[3]);
    }
    else if (strcmp(argv[2],"network") == 0 )
    {
        netConnect(atoi(argv[3]));
    }
    else
    {
        perror("Wrong type of connection\n");
        exit(1);
    }

    joinServer();


    return 0;
}