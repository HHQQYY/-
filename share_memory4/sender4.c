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
	char input_message[1024];
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
    char *p = shmat(ret,NULL,0);
    if((char *)-1 == p)
    {
        perror("shmat error");
        return -1;
    }
    
    fgets(input_message, 1024, stdin);
    
    sem_wait(mutex);
    strcpy(p,input_message);
    sem_post(mutex);
    sem_post(rr);
    
	char over_message[1024]; 
    sem_wait(sr); 
    sem_wait(mutex);
    strcpy(over_message,p);
    sem_post(mutex);
	 
    printf("The Process of sender gets message from receiver process is:%s\n",over_message); 
    shmdt(p);
    sem_close(mutex);
    sem_unlink(sem_mutex_name);
    sem_close(sr);
    sem_unlink(sem_sr_name); 
    sem_close(rr);
    sem_unlink(sem_rr_name); 
    printf("Sender:  Process End\n"); 
    return 0;
}
