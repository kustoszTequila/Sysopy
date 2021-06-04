#include "library.h"

int localSocket = -1;
int netSocket = -1;

void startLocalServer(char* path)
{
    if ((localSocket = socket(AF_UNIX,SOCK_STREAM,0)) == -1)
    {
        perror("Error while starting local socket\n");
        exit(1);
    }

    struct sockaddr_un address;
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path,path);

    if (bind(localSocket,(struct sockaddr *)&address,sizeof(struct sockaddr_un)) == -1)
    {
        perror("Error while binding\n");
        exit(1);
    } 
    if (listen(localSocket,MAX_CLIENTS) == -1)
    {
        perror("Error with listening\n");
        exit(1);
    }
}
void startNetServer(int port)
{
    if ((netSocket = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("Error while starting local socket\n");
        exit(1);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(netSocket,(struct sockaddr *)&address,sizeof(struct sockaddr_in)) == -1)
    {
        perror("Error while binding\n");
        exit(1);
    } 
    if (listen(netSocket,MAX_CLIENTS) == -1)
    {
        perror("Error with listening\n");
        exit(1);
    }
}


int main(int argc, char** argv) 
{
    if (argc != 3)
    {
        perror("Wron number of arguments\n");
        exit(1);
    }
    startLocalServer(argv[2]);
    startNetServer(atoi(argv[1]));

    return 0;
}