#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define UNIX_PATH_MAX 108
#define SOCK_PATH "sock_path"

int main(int argc, char *argv[]) {
    

   if(argc !=2){
    printf("Not a suitable number of program parameters\n");
    return(1);
   }
    sleep(1);


    /*********************************************
    Utworz socket domeny unixowej typu datagramowego
    Utworz strukture adresowa ustawiajac adres/sciezke komunikacji na SOCK_PATH
    Polacz sie korzystajac ze zdefiniowanych socketu i struktury adresowej
    ***********************************************/
    int fd = socket(AF_UNIX,SOCK_DGRAM,0);
    if (fd < 0)
    	perror("socket error");
    struct sockaddr_un address; 
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path,SOCK_PATH);
    connect(fd,(struct sockaddr_un *)&address,sizeof(struct sockaddr_un));

    char buff[20];
    int to_send = sprintf(buff, argv[1]);

    if(write(fd, buff, to_send+1) == -1) {
        perror("Error sending msg to server");
    }


    /*****************************
    posprzataj po sockecie
    ********************************/
    shutdown(fd, SHUT_RDWR);
    close(fd);
    return 0;
}

