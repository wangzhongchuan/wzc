#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<pthread.h>

int task1(int *cnt)
{
    while(*cnt < 5)
    {
        sleep(1);
	(*cnt)++;
	printf("task1 cnt = %d.\n",*cnt);
    }
    
    return (*cnt);
}

int task2(int *cnt)
{
    while(*cnt < 5)
    {
        sleep(2);
	(*cnt)++;
	printf("task2 cnt = %d.\n",*cnt);
    }
    
    return (*cnt);
}

int main(int argc,char **argv)
{
    int result;
    int t1 = 0;
    int t2 = 0;
    int rt1,rt2;
    pthread_t thread1,thread2;

    result = pthread_create(&thread1,PTHREAD_CREATE_JOINABLE,(void*)task1,(void*)&t1);
    
    if(result)
    {
        perror("pthread_create:task1.\n");
	exit(EXIT_FAILURE);
    }

    result = pthread_create(&thread2,PTHREAD_CREATE_JOINABLE,(void*)task2,(void*)&t2);
    
    if(result)
    {
        perror("pthread_create:task2.\n");
	exit(EXIT_FAILURE);
    }

    pthread_join(thread1,(void*)&rt1);
    pthread_join(thread2,(void*)&rt2);
    
    printf("total%d times.\n",t1 = t2);
    printf("return value of task1:%d.\n",rt1);
    printf("return value of task2:%d.\n",rt2);

    exit(EXIT_SUCCESS);
    
}
