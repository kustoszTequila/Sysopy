#include "library.h"

struct Client client[MAX_CLIENTS];
int clientsOnline = 0;
int localSocket = -1;
int netSocket = -1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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
void* areAlive()
{
    while(1)
    {
        sleep(PING_TIME);

        pthread_mutex_lock(&mutex);
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (client[i].isOnline)
            {
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
    struct pollfd* local = malloc(sizeof(struct pollfd));
    struct pollfd* network = malloc(sizeof(struct pollfd));
    struct pollfd* pollClients = calloc(MAX_CLIENTS,sizeof(struct pollfd));
    local->fd = localSocket;
    network->fd = netSocket;
    local->events = POLLIN;
    network->events = POLLIN;

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        pollClients[i].fd = client[i].socket;
        pollClients[i].events = POLLIN;
    }
    pthread_mutex_unlock(&mutex);

    poll(pollClients,MAX_CLIENTS,-1);

    if (local->revents == POLLIN)
        return local->fd;
    else if (network->revents == POLLIN)
        return network->fd;    

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (pollClients[i].revents == POLLIN)
        {
            return pollClients[i].fd;
        }
    }

    free(pollClients);
    return -1;
}
int findClient(int socket)
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

    while(1)
    {
        char message[MSG_LEN];
        int user = getNextClient();
        
        if (recv(user,message,MSG_LEN,0) <= 0)
        {
            continue;
        }

        pthread_mutex_lock(&mutex);

        if (strcmp(message,"PING") == 0) // setting client alive 
        {
            int id = findClient(user);
            if (id != -1)
            {
                        client[id].isOnline = 1;
                        clientsOnline++;
                        break;
            }
        }
        else if (message[0] >= '1' && message[0] <= '9' && strlen(message) == 1)
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
        else if (strcmp("WON",message) == 0 || strcmp("LOST",message) == 0 || 
        strcmp("DRAW",message) == 0)
        {
            int id = findClient(user);
            if (client[id].rival == -1)
                client[id].isOnline = 0;
            else
            {
                send(client[id].rival,message,MSG_LEN,0);
                client[id].isOnline = 0;
                int rivalId = findClient(client[id].rival);
                client[rivalId].isOnline = 0;
                clientsOnline = clientsOnline - 2;
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
                        }

                        // informuj że nie ma rival
                    }
                }
            }
        }
        pthread_mutex_unlock(&mutex);
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

    work();

    return 0;
}