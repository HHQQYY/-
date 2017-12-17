#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>  
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
	int i=0;
	char results_message[1024];
	char * sem_mutex_name="huchi";
	char * sem_sr_name="tongbu1"; 
    char * sem_rr_name="tongbu2"; 
    sem_t * mutex = sem_open(sem_mutex_name,O_CREAT | O_RDONLY,0666, 1);
    sem_t * sr = sem_open(sem_sr_name,O_CREAT | O_RDONLY, 0666, 0);
    sem_t * rr = sem_open(sem_rr_name,O_CREAT | O_RDONLY, 0666, 0);
    
    if(SEM_FAILED == mutex)
    {
      perror("sem_mutex_open error");
      return -1;
    }
    
    if(SEM_FAILED == sr)
    {
      perror("sem_wr_open error");
      return -1;
    }
    
    if(SEM_FAILED == rr)
    {
      perror("sem_wr_open error");
      return -1;
    }
    
    
    key_t key =ftok(".",12);
    if(key<0)
    {
       perror("ftok error");
       return -1;
    }
    
    int ret = shmget(key,1024,IPC_CREAT | 0666);
    if(ret<0)
    {
        perror("shmget error");
        return -1;
    }
    
    char *base = shmat(ret,NULL,0);
    if((char *)-1 == base)
    {
        perror("shmat error");
        return -1;
    }
    
    sem_wait(rr);
    sem_wait(mutex);
    strcpy(results_message,base);
    sem_post(mutex);
    printf("Receiver : message is %s",results_message);
    
    char * confirm_message="over"; 
    sem_wait(mutex);
    for(i=0;i<strlen(results_message);i++){
    	base[i]=NULL;
	} 
	strcpy(base,confirm_message);  
	sem_post(mutex);
	sem_post(sr);
   
    printf("Receiver :  Process End \n");
    return 0;
}
