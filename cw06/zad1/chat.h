#ifndef chat_h
#define chat_h


#define PROJECT_ID 0x099
#define MSG_MAXS 4056
#define CLIENTS_MAX  12

enum mtype {
    LIST = 1, 
    CONNECT = 2, 
    DISCONNECT = 3, 
    STOP = 4,
    INIT = 5
}; 

struct messageText 
{  
    char buffor [MSG_MAXS];
};

struct message 
{
    long mType;
    key_t mKey;
    int qid;
    struct messageText messageT;
};


#endif
