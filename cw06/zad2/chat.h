#ifndef chat_h
#define chat_h


#define CHAT_ID 'A'
#define MSG_MAXS 4056
#define CLIENTS_MAX 12
#define SPATH "/serv"

enum mtype { 
    DISCONNECT = 1, 
    STOP = 2,
    LIST = 3, 
    CONNECT = 4,
    INIT = 5,
    CHAT = 6
}; 

struct message 
{
    pid_t sndPid;
   char buffor [MSG_MAXS];
};


#endif
