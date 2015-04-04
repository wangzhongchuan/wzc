#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(int argc,char **argv)
{
    int val,stat;
    pid_t child;
    printf("\nTry to creat new process.\n");
    child = fork();
    
    switch(child)
    {
        case -1:
        
            printf("error fork.\n");
            exit(EXIT_FAILURE);
        
        case 0:
        
            printf("This is child.\n");
	    printf("\tchild pid is %d\n",getpid());
            printf("\tchild ppid is %d\n",getppid());
            exit(EXIT_SUCCESS);
        
        default:
        
            waitpid(child,&stat,0);
            printf("This is parent.\n");
	    printf("\tchild pid is %d\n",getpid());
            printf("\tchild ppid is %d\n",getppid());
            printf("\tchild exited with%d\n",stat);
        
     
     }   
    
     exit(EXIT_SUCCESS);  
}
