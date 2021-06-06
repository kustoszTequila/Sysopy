#include "library.h"

struct Client client[MAX_CLIENTS];
int clientsOnline = 0;
int localSocket = -1;
int netSocket = -1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char pth[256];

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
void* areAlive() // ping to see if client's are online
{
    while(1)
    {
        sleep(PING_TIME);

        pthread_mutex_lock(&mutex);
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (client[i].isOnline)
            {
                printf("PINGING\n");
                send(client[i].socket,"PING",MSG_LEN,0);
                client[i].isOnline = 0;
                clientsOnline--;
            }
            else
                continue;
        }
        pthread_mutex_unlock(&mutex);
    }
}
int getNextClient()
{

    struct pollfd* pollClients = calloc(MAX_CLIENTS + 2,sizeof(struct pollfd));

    pollClients[MAX_CLIENTS].fd = localSocket;
    pollClients[MAX_CLIENTS+1].fd = netSocket;
    pollClients[MAX_CLIENTS].events = POLLIN;
    pollClients[MAX_CLIENTS+1].events = POLLIN;

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        pollClients[i].fd = client[i].socket;
        pollClients[i].events = POLLIN;
    }
    pthread_mutex_unlock(&mutex);

    poll(pollClients,MAX_CLIENTS + 2,-1);

    for (int i = 0; i < MAX_CLIENTS + 2; i++)
    {
        
        if (pollClients[i].revents == POLLIN)
        {
            if(pollClients[i].fd == localSocket || pollClients[i].fd == netSocket)
                return accept(pollClients[i].fd ,NULL,NULL);
            else    
                return pollClients[i].fd;
        }
    }

    free(pollClients);
    return -1; // there is no user with such id 
}
int findClient(int socket) // find client's id by socket
{
    for (int i =0; i < MAX_CLIENTS; i++)
    {
        if (client[i].socket == socket)
        {
            return i;
        }
    }
    return - 1;
}
void work()
{
    pthread_t ping; // thread for periodically pinging clients
    pthread_create(&ping,NULL,areAlive,NULL); 
    printf("SERVER IS WORKING\n");
    while(1)
    {
        char message[MSG_LEN];
        int user = getNextClient();
        if (recv(user,message,MSG_LEN,0) <= 0)
        {
            sleep(0.5);
            continue;
        }

        pthread_mutex_lock(&mutex);
        if (message[0] >= '1' && message[0] <= '9' && strlen(message) == 1) // set board
        {
            int id = findClient(user);
            if (client[id].rival == -1)
            {
                send(user,"NO_RIVAL",MSG_LEN,0);
            }
            else
            {
                send(client[id].rival,message,MSG_LEN,0);
            }
        }
        else if (strcmp(message,"PING") == 0) // setting client alive 
        {
            int id = findClient(user);
            if (id != -1)
            {
                        client[id].isOnline = 1;
                        clientsOnline++;
            }
        }
        else if (strcmp("WON",message) == 0 || strcmp("LOST",message) == 0 || 
        strcmp("DRAW",message) == 0) // finish the game
        {
            int id = findClient(user);
            if (id == -1)
            {
                perror("ERROR WITH ID \n");
                exit(1);
            }
            if (client[id].rival == -1)
                client[id].isOnline = 0;
            else
            {
                send(client[id].rival,message,MSG_LEN,0);
                client[id].isOnline = 0;
                strcpy(client[id].nick," ");  
                int rivalId = findClient(client[id].rival);
                client[rivalId].isOnline = 0;
                strcpy(client[rivalId].nick," ");  
                clientsOnline = clientsOnline - 2;
            }    
        }
        else if (strcmp("QUIT",message) == 0) //disconnect user
        {
            int id = findClient(user);
            if (id != -1)
            {
                client[id].isOnline = 0;
                strcpy(client[id].nick," ");
                if (client[id].rival != -1)
                {
                    int rivalId = findClient(client[id].rival);
                    client[rivalId].isOnline = 0;
                    strcpy(client[rivalId].nick," ");
                    send(client[rivalId].socket,"QUIT",MSG_LEN,0);
                }
                client[id].rival = -1;
            }
        }
        else // we check if it is a new user
        {
            int id = findClient(user);
            if (id == -1) // we have a new client
            {
                int taken = 0;
                for (int i = 0; i < MAX_CLIENTS; i++)
                {
                    if (strcmp(client[i].nick,message) == 0 )
                    {
                        taken = 1;
                        send(user,"TAKEN",MSG_LEN,0);
                        break;
                    }
                }
                
                if (!taken) // new client and nick is not taken
                {
                    int newPlace;
                    for (int i = 0; i < MAX_CLIENTS; i++)
                    {
                        if (client[i].isOnline == 0 )
                        {
                            newPlace = i;
                            break;
                        }
                    }
                    strcpy(client[newPlace].nick,message);
                    client[newPlace].isOnline = 1;
                    client[newPlace].rival = -1;
                    client[newPlace].socket = user;
                    clientsOnline ++;

                    int hasRival = 0;
                    // looking for rival 
                    for (int i = 0; i < MAX_CLIENTS; i++)
                    {
                        if(client[i].isOnline == 1 && client[i].socket != user
                        && client[i].rival == -1)
                        {
                            client[i].rival = user;
                            client[newPlace].rival = client[i].socket;
                            send(user,"X",MSG_LEN,0);
                            send(client[i].socket,"O",MSG_LEN,0);
                            hasRival = 1;
                            break;
                        }
                        
                    }
                    if (!hasRival)
                        send(user,"NO_RIVAL",MSG_LEN,0);
                }
            }
            else
            {
                printf("Client already registered\n");
            }
        }
        pthread_mutex_unlock(&mutex);
    }
}
void end(int signum)
{
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        if (client[i].isOnline)
        {
            send(client[i].socket,"QUIT",MSG_LEN,0);
        }
    }
    unlink(pth);
    exit(0);
}
int main(int argc, char** argv) 
{
    if (argc != 3)
    {
        perror("Wron number of arguments\n");
        exit(1);
    }
    strcpy(pth,argv[2]);
    signal(SIGINT,&end);
    startLocalServer(argv[2]);
    startNetServer(atoi(argv[1]));

    work();

    return 0;
}