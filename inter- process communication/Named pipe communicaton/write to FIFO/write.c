

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#if 0
int main(int argc,char **argv)
 {
  mode_t mode=0666;
  if(argc!=2)
   {
    printf("Usage:[%s]fifo_filename\n",argv[0]);
    return -1;
    

   }
   if(mkfifo(argv[1],mode)<0)
   {   perror("mkfifo");
  return -1;

   }
 return 0;
}
#endif


#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define FIFO_SERVER "/home/tom/wzc/wzc"
main(int argc,char** argv)
{
int fd;
char w_buf[100];
int nwrite;
if(fd==-1)
if(errno==ENXIO)
printf("open error;no reading process\n");
fd=open(FIFO_SERVER,O_WRONLY|O_NONBLOCK,0);
if(argc==1)
printf("Please send something\n");
strcpy(w_buf,argv[1]);
if((nwrite=write(fd,w_buf,100))==-1)
{
if(errno==EAGAIN)
printf("The FIFO has not been read yet. Please try later\n");
}
else
printf("write %s to the FIFO\n",w_buf);
}

