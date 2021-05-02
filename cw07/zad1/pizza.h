#define CAPACITY 5
#define SEMNUM 6
/*struct sembuf
{
    unsigned short sem_num;  
    short  sem_op;  
    short  sem_flg;  
};*/
struct pizza
{
	int number[CAPACITY];
};
struct tm* getCurTime()
{
    time_t rawtime;  
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    return timeinfo;
}