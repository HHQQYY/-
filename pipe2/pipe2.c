#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>  
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define READ 0

#define WRITE 1

int main() {

    int file_descriptors[2];
    pid_t pid1,pid2,pid3;
    char buf[256];
    int returned_count;
    
    char * sem_mutex_name="huchi";
	char * sem_r_name="tongbu1"; 

    sem_t * mutex = sem_open(sem_mutex_name,O_CREAT | O_RDONLY,0666, 1);
    sem_t * r = sem_open(sem_r_name,O_CREAT | O_RDONLY, 0666, -2);
   
    if(SEM_FAILED == mutex)
    {
      perror("sem_mutex_open error");
      return -1;
    }
    
    if(SEM_FAILED == r)
    {
      perror("sem_r_open error");
      return -1;
    }
    
  
    
    pipe(file_descriptors);  /*创建无名管道*/

    if((pid1 = fork()) == -1) { /*创建子进程*/
        printf("Error in fork\n");
        exit(1);
    }
    if(pid1 == 0) {
       
        printf("pid1:%d in the spawned (child) process...\n",getpid());
        //        子进程向父进程写数据，关闭管道的读端
        
        sem_wait(mutex);
        
        close(file_descriptors[READ]);
        write(file_descriptors[WRITE], "pid1 send string111\n", strlen("pid1 send string111\n"));
        
		sem_post(mutex);
        sem_post(r);
        exit(0);
    }



    if ((pid2 = fork()) == -1) {
        printf("ERROR in fork pid2\n");
        exit(1);
    }
    if (pid2 == 0) {
        
        printf("pid2:%d in the spawned (child) process...\n",getpid());
        sem_wait(mutex);
		
		close(file_descriptors[READ]);
        write(file_descriptors[WRITE], "pid2 send string222\n", strlen("pid2 send string222\n"));
        
		sem_post(mutex);
		sem_post(r);
		exit(0);
    }


    if ((pid3 = fork()) == -1) {
        printf("ERROR in fork pid3\n");
        exit(1);
    }
    if (pid3 == 0) {
        sleep(1);
        printf("pid3:%d in the spawned (child) process...\n",getpid());

		sem_wait(mutex);
		
        close(file_descriptors[READ]);
        write(file_descriptors[WRITE], "pid3 send string333\n", strlen("pid3 send string333\n"));
        
        sem_post(mutex);
        sem_post(r);
        exit(0);
    }
    else {
        
        sleep(1);
        sem_wait(r);
        sem_wait(mutex);
        /*父进程从管道读取子进程写的数据，关闭管道的写端*/
        close(file_descriptors[WRITE]);
        returned_count = (int)read(file_descriptors[READ], buf, sizeof(buf));
        sem_post(mutex);
		printf("%d bytes of data received from spawned process: \n%s\n",returned_count, buf);
    
	    sem_close(mutex);
        sem_unlink(sem_mutex_name);
        sem_close(r);
        sem_unlink(sem_r_name); 
	    pid1 = waitpid(pid1, NULL, WUNTRACED);
        pid2 = waitpid(pid2, NULL, WUNTRACED);
        pid3 = waitpid(pid3, NULL, WUNTRACED);
        printf("main process pid: %d\n",getpid());
        printf("pid: %d %d %d in the spawning (parent) process...\n",pid1,pid2,pid3);
	
	}
    return 0;
}
