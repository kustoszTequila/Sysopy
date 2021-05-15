#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

int n;
int W,H,M;
int** Img;
int** newImg;

void loadPGM(char* input)
{
    printf("INPUT %s\n",input);
    FILE* fp = fopen(input,"r");
    if (fp == NULL)
    {
        perror(" Error while opening file\n");
        exit(1);
    }
    char ch[512];
    fscanf(fp,"%s",ch);
    fscanf(fp,"%s",ch);
    W = atoi(ch);
    fscanf(fp,"%s",ch);
    H = atoi(ch);
    fscanf(fp,"%s",ch);
    M = atoi(ch);
    printf("%d %d %d \n",W,H,M);
    Img = calloc(H,sizeof(int*));
    newImg = calloc(H,sizeof(int*));
    for (int i = 0;i < W;i++)
    {
        Img[i] = calloc(W,sizeof(int));
        newImg[i] = calloc(W,sizeof(int));
    }
    int iter = 0;
    int in1 = 0;
    while(fscanf(fp,"%s",ch) == 1)
    {
        Img[in1][iter] = atoi(ch);
        iter ++;
        if (iter % W == 0)
        {
            in1++;
            iter = 0;
        }
    }

    fclose(fp);
}
void savePGM(char* output)
{
    FILE* fp = fopen(output,"w");
    if (fp == NULL)
    {
        perror("Error while saving file\n");
        exit(1);
    }
    fprintf(fp,"P2\n");
    fprintf(fp,"%d %d\n",W,H);
    fprintf(fp,"%d\n",M);
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            fprintf(fp,"%d ",newImg[i][j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}
void *blocks(void *arg)
{
    struct timeval startT,endT;
    gettimeofday(&startT,NULL);
    int indx = *((int *) arg);

    int start = (indx-1)*ceil(W/n);
    int end = indx*ceil(W/n)-1;
    if (end >= W)
        end = W -1;
    if (start < 0)
        start = W -1;
    for (int i = start ; i <= end; i++ )
    {
        for(int j = 0; j < H; j++)
        {
            newImg[j][i] = 255 - Img[j][i];
        }
    }  

    gettimeofday(&endT,NULL);
    unsigned int* tim;
    tim = malloc(sizeof(unsigned int));
    *tim = (endT.tv_usec - startT.tv_usec);
    pthread_exit(tim); 
}
void *numbers(void *arg)
{
    struct timeval startT,endT;
    gettimeofday(&startT,NULL);
    int indx = *((int *) arg);

    int range = ceil(M/n);
    int start = (indx-1) * range;
    int end = indx * range - 1; 
    if (end >= W)
        end = W -1;
    if (start < 0)
        start = W -1;

    for(int i = 0 ; i < H; i++)
    {
        for (int j = 0 ; j < W; j++)
        {
            if (Img[j][i]>=start && Img[j][i] < end)
            {
                newImg[j][i] = 255 - Img[j][i];
            }
        }
    }
    
    gettimeofday(&endT,NULL);
    unsigned int* tim;
    tim = malloc(sizeof(unsigned int));
    *tim = (endT.tv_usec - startT.tv_usec);
    pthread_exit(tim); 
}
int main(int argc, char** argv) 
{
    char* input,*output;
    char* type;
    if (argc == 5 )
    {
        n = atoi(argv[1]);
        if ( n <= 0)
        {
            perror("n must be positive!\n");
            exit(1);
        }
        type = calloc(1,1 + strlen(argv[2]));
        input = calloc(1,1 + strlen(argv[3]));
        output = calloc(1,1 + strlen(argv[4]));
        strcpy(type,argv[2]);
        strcpy(input,argv[3]);
        strcpy(output,argv[4]);
        if (input == NULL || output == NULL || type == NULL)
        {
            perror("Error with input\n");
            exit(1);
        }
    }
    else
    {
        perror("wrong number of arguments!\n");
        exit(1);
    }
   // printf("%d\n",n);
   // printf("%s %s %s \n",type,input,output);
    char* realInput = calloc(1,strlen(input) + 1 + strlen("./Images/"));
    strcat(realInput,"./Images/");
    strcat(realInput,input);
    free(input);
    loadPGM(realInput);
    struct timeval start,end;
    pthread_t threads[n];
    gettimeofday(&start,NULL);
    for (int i = 0; i<n; i++)
    {
        
        if (strcmp(type,"numbers") == 0)
        {
            int *arg = malloc(sizeof(int));
            *arg = (i+1);
            int result = pthread_create(&threads[i], NULL,&numbers, arg);
            if (result != 0) 
            {
                perror("Error while making a thread\n");
                exit(1);
            }
        }
        else if (strcmp(type,"block") == 0)
        {
            int *arg = malloc(sizeof(int));
            *arg = (i+1);
            int result = pthread_create(&threads[i], NULL, &blocks, arg);
            if (result != 0) 
            {
                perror("Error while making a thread\n");
                exit(1);
            }
        }
        else
        {
            perror("Wrong type of conversion\n");
            exit(1);
        }
    }
    printf("Type: %s\n",type);
    for (int i = 0; i < n; i++)
    {
        unsigned int *tim;
        pthread_join(threads[i], (void **) &tim);
        printf("Thread num. %d, spent: %u microsecunds\n",i+1,*tim);
    }
    gettimeofday(&end,NULL);
    unsigned int *tim = malloc(sizeof(unsigned int));
    *tim = end.tv_usec - start.tv_usec;
    printf("Main,time spent: %u microsecunds\n",*tim);

    char* realOutput = calloc(1,strlen(input) + 1 + strlen("./Negative/"));
    strcat(realOutput ,"./Negative/");
    strcat(realOutput,output);
    free(output);
    savePGM(realOutput);

    free(type);
    free(realInput);
    free(realOutput);

    return 0;

}
