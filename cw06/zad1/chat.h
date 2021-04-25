#ifndef chat_h
#define chat_h


#define CHAT_ID 0x099
#define MSG_MAXS 4056
#define CLIENTS_MAX 12

enum mtype { 
    DISCONNECT = 1, 
    STOP = 2,
    LIST = 3, 
    CONNECT = 4,
    INIT = 5,
    CHAT = 6
}; 

struct messageText 
{  
    char buffor [MSG_MAXS];
};

struct message 
{
    long mtype;
    key_t mKey;
    int qid;
    pid_t sndPid;
    struct messageText messageT;
};


#endif
