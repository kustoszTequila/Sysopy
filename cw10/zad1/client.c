#include "library.h"

int server;
char* nick;
char myFigure;
char rivalFigure;
char board[3][3];
int moves = 0;

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
void showGrid()
{
    printf("---------\n");
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf(" %c ",board[i][j]);
        }
        printf("\n");
    }
    printf("---------\n");
}
void checkWin()
{
    int won = 0;
    for (int i = 0; i < 3; i++)
    {
        if( board[i][0] == board[i][1] && board[i][1] == board[i][2])
        {
            won = 1;
        }
        if(board[0][i] == board[1][i] && board[1][i] == board[2][i])
        {
            won = 1;
        }
    }
    if(board[0][0] == board[4][4] && board[4][4] == board[8][8])
        {
            won = 1;
        }
    if(board[2][2] == board[4][4] && board[4][4] == board[6][6])
        {
            won = 1;
        }
    if(won)
    {
        printf("YOU WON\n");
        send(server,"WON",MSG_LEN,0);
        exit(0);
    }  
    if(moves == 9)
    {
        printf("DRAW\n");
        send(server,"DRAW",MSG_LEN,0);
        exit(0);
    }    

}
void nextMove()
{   int num;
    while(1)
    {
        scanf("%d",&num);
        if (num >= 1 && num <= 9)
        {
            if (board[(int)((num - 1) / 3)][(num-1) % 3] == ' ')
            {
                printf("THIS PLACE IS ALREADY TAKEN\n");
            }
            else
            {
                board[(int)((num - 1) / 3)][(num-1) % 3] = myFigure;
                break;
            }
        }
        else
            printf("NUMBER MUST BE BETWEEN 1 AND 9\n");
    }

    moves++;
    showGrid();
    checkWin();
    char pos[MSG_LEN];
    sprintf(pos,"%d",num);
    send(server,pos,MSG_LEN,0);
    
}
void work()
{
    struct pollfd* pol = malloc(sizeof(struct pollfd));
    pol->fd = server;
    pol->events = POLLIN;


    while(1)
    {   
        poll(pol,1,-1);

        char message[MSG_LEN];

        recv(server,message,MSG_LEN,0);

        if (strcmp("PING",message) == 0)
        {
            send(server,"PING",MSG_LEN,0);
        }
        else if (strcmp("X",message) == 0)
        {
            myFigure = 'X';
            rivalFigure = 'O';
            printf("YOUR FIGURE IS: %c \n",myFigure);
        }
        else if (strcmp("O",message) == 0)
        {
            myFigure = 'O';
            rivalFigure = 'X';
            printf("YOUR FIGURE IS: %c \n",myFigure);       
            showGrid();
            printf("YOUR TURN\n");
            nextMove();
            printf("\n");
        }
        else if (strcmp("WON",message) == 0)
       {
            printf("WON\n");
            exit(0);
        }
        else if (strcmp("DRAW",message) == 0 )
        {
            printf("DRAW\n");
            exit(0);
        }
        else if (strcmp("LOST",message) == 0)
        {
            printf("LOST\n");
            exit(0);
        }
        else if (strcmp("TAKEN",message) == 0)
        {
            printf("YOUR CHOSEN NICK IS ALREADY TAKEN\n");
            exit(1);
        }
        else if (strcmp("NO_RIVAL",message) == 0 )
        {
            printf("NO FREE OPONENTS, PLEASE WAIT \n");

        }
        else if (message[0] >= '1' && message[0] <= 9 && strlen(message) == 1)
        {
            moves++;
            int pos = (int)(message[0]);
            board[(int)((pos - 1) / 3)][(pos-1) % 3] = message[0];
            showGrid();
            printf("YOUR TURN\n");
            nextMove();
            printf("\n");
        }
        else
        {
            printf("MESSAGE \n");
            printf("%s",message);
        }
    }
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

    for (int i = 0; i < 3 ; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board[i][j] = ' ';
        }
    }

    work();
    return 0;
}