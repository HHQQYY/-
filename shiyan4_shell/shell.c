#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


#define EXIT 0 //�����˳�


#define TRUE 1

char *cmd[4] = {"exit","cmd1","cmd2","cmd3"};

int getCmdIndex(char *cmd)
{
    int i;
    for(i=0;i<4;i++)
    {
        if(strcmp(cmd,cmd[i])==0)
        {
            return i;
        }
    }
    return -1;
}

void myFork(int cmdIndex)
{
    pid_t pid;
    if((pid = fork())<0)
    {
        printf("fork error");
        exit(0);
    }
    else if(pid == 0)
    {
        int execl_status = -1;
        printf("child is running");
        switch(cmdIndex)
        {
            case 1:
                execl_status = execl("./cmd1","cmd1",NULL);
                break;
            case 2:
                execl_status = execl("./cmd2","cmd2",NULL);
                break;
            case 3:
                execl_status = execl("./cmd3","cmd3",NULL);
                break;
            default:
                printf("Invalid Command\n");
                break;
        }
        if(execl_status<0)
        {
            printf("fork error");
            exit(0);
        }
        printf("fork success\n");
        exit(0);
    }
    else
    {
        return;
    }
}
void runCMD(int cmdIndex)
{
    
    if(cmdIndex<0||cmdIndex>3)
    {
    	printf("commant not found! \n");
	}
	else if(cmdIndex==0){
		exit(0);
	}
	else{
		myFork(cmdIndex);
	}	
	
    
}
int main()
{
    pid_t pid;
    char cmdStr[20];
    int cmdIndex;
    while(TRUE)
    {
        printf("������������ ");
        scanf("%s",cmdStr);
        cmdIndex = getCmdIndex(cmdStr);
        runCMD(cmdIndex);
        wait(0);
        printf("waiting for next command");
    }
    return 0;
}
