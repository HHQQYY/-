#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <stdlib.h>
#include <sys/wait.h>
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
    int fd[2];
    int ret = pipe(fd);
    if (ret == -1)
    {
        perror("pipe error\n");
        return 1;
    }
    pid_t id = fork();
    if (id == 0)
    {//child
    //printf("ÎÒÊÇ¶ù×Ó°¡°¡°¡°¡°¡\n");
        int i = 0;
        close(fd[0]);
        char *child = "I am  child!";
        while (++i)
        {
            write(fd[1], child, strlen(child) + 1);
            printf("pipe capacity: %d\n", (int)(i*(strlen(child) + 1)));
        }
        close(fd[1]);
    }
    else if (id>0)
    {//father
        sleep(2);
        close(fd[1]);
        waitpid(id, NULL, 0);
    }
    else
    {//error
        perror("fork error\n");
        return 2;
    }
    return  0;
}
