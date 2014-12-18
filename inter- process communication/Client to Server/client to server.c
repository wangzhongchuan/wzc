#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<unistd.h>
#include<stdlib.h>
#define MSGKEY 75
struct msgform
  { long mtype;
    char mtext[1024];
  }msg;
int msgqid,i;
void client()
  {int i;
   msgqid=msgget(MSGKEY,0777);
   for(i=1;i<100;i++)
     {
      msg.mtype=i;
      sleep(1);
      printf("(client)sent\n");
      msgsnd(msgqid,&msg,1024,0);
     



      }
  exit(0);



  }
 void server()
   {
   msgqid=msgget(MSGKEY,0777|IPC_CREAT);
   do
     {
     msgrcv(msgqid,&msg,1024,0,0);
     sleep(1);
     printf("i=%ld\n",msg.mtype);
     printf("(server)recevied\n");


     }while(msg.mtype!=1);
     msgctl(msgqid,IPC_RMID,0);
     exit(0);

    
   }
  main()
  {
   while((i=fork())==-1);
   if(!i)server();
   while((i=fork())==-1);
   if(!i)client();
   wait(0);
   wait(0);


  }
