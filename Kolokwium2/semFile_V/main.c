#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <sys/wait.h>

#define FILE_NAME "common.txt"
#define SEM_NAME "/kol_sem"


int main(int argc, char** args){

   if(argc !=4){
    printf("Not a suitable number of program parameters\n");
    return(1);
   }

    /**************************************************
    Stworz semafor systemu V
    Ustaw jego wartosc na 1
    ***************************************************/
   key_t key = ftok(SEM_NAME,1);
   if (key < 0)
      perror("Ftok error");
   int semid = semget(key,1,IPC_CREAT| 0666);
   if (semid < 0)
      perror("Błąd semget");

   union semun {
   int  val;    
   struct semid_ds *buf;    
   unsigned short  *array;  
   struct seminfo  *__buf;  
                                           
};
   union semun arg;
   arg.val = 1;
   if (semctl(semid,0,SETVAL,arg) < 0 )
      perror("Błąd semctl");
    
     
     int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC , 0644);
     
     int parentLoopCounter = atoi(args[1]);
     int childLoopCounter = atoi(args[2]);
        
     char buf[60]; // zwiększyłem do 60 ponieważ bez tego występowal stack smashing
     pid_t childPid;
     int max_sleep_time = atoi(args[3]);
     


     if((childPid=fork())){
      int status = 0;
      srand((unsigned)time(0)); 

        while(parentLoopCounter--){
    	    int s = rand()%max_sleep_time+1;
    	    sleep(s);    
            
      struct sembuf* startSem = calloc(1,sizeof(struct sembuf));     
      if (startSem == NULL)
         perror("Błąd alokacji");
      startSem->sem_num = 0;
      startSem->sem_op = -1;
      startSem->sem_flg = 0; 
      semop(semid,startSem,1);
	    /*****************************************
	    sekcja krytyczna zabezpiecz dostep semaforem
	    **********************************************/
      
            sprintf(buf, "Wpis rodzica. Petla %d. Spalem %d\n", parentLoopCounter,s);
	    write(fd, buf, strlen(buf));
	    write(1, buf, strlen(buf));
            
	    /*********************************
	    Koniec sekcji krytycznej
	    **********************************/
      struct sembuf* endSem = calloc(1,sizeof(struct sembuf));     
      if (endSem == NULL)
         perror("Błąd alokacji");
      endSem->sem_num = 0;
      endSem->sem_op = 1;
      endSem->sem_flg = 0; 
      semop(semid,endSem,1);
        }
        waitpid(childPid,&status,0);
     }
     else{

	srand((unsigned)time(0)); 
        while(childLoopCounter--){

	    int s = rand()%max_sleep_time+1;
            sleep(s);              

      struct sembuf* startSem = calloc(1,sizeof(struct sembuf)); 
      if (startSem == NULL)
         perror("Błąd alokacji");    
      startSem->sem_num = 0;
      startSem->sem_op = -1;
      startSem->sem_flg = 0;             
      semop(semid,startSem,1);
	    /*****************************************
	    sekcja krytyczna zabezpiecz dostep semaforem
	    **********************************************/

            
            sprintf(buf, "Wpis dziecka. Petla %d. Spalem %d\n", childLoopCounter,s);
            write(fd, buf, strlen(buf));
	    write(1, buf, strlen(buf));

	    /*********************************
	    Koniec sekcji krytycznej
	    **********************************/
      struct sembuf* endSem = calloc(1,sizeof(struct sembuf));  
      if (endSem == NULL)
         perror("Błąd alokacji");   
     endSem->sem_num = 0;
      endSem->sem_op = 1;
      endSem->sem_flg = 0; 
      semop(semid,endSem,1);
        }
        _exit(0);
     }
     
    /*****************************
    posprzataj semafor
    ******************************/
     semctl(semid,0,IPC_RMID,NULL);
     close(fd);
    return 0;
}
     
        
