#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/wait.h>
#include<sys/socket.h>

#define PORT 5000
#define BACKLOG 10
#define LENGTH 512

int main()
{
    int sockfd;
    int nsockfd;
    int num;
    int sin_size;
    char revbuf[LENGTH] = {0};
    struct sockaddr_in addr_local;
    struct sockaddr_in addr_remote;
    
    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
    {
	printf("ERROR:Fail to obtain Socket Descriptor!\n");
        return (0);
    }
    else
    {
	printf("OK:Obtain Socket Descriptor successfully.\n");
    }
    
    addr_local.sin_family = AF_INET;
    addr_local.sin_port = htons(PORT);
    addr_local.sin_addr.s_addr = INADDR_ANY;
    bzero(&(addr_local.sin_zero),8);
   
    if(bind(sockfd,(struct sockaddr*)&addr_local,sizeof(struct sockaddr)) == -1)
    {
	printf("ERROR:Failed to bind Port%d\n",PORT);
	return (0);
    }
    else
    {
	printf("OK:Bind the Port%d successfully!\n",PORT);
    }

    sin_size = sizeof(struct sockaddr);
   
    if((num = recvfrom(sockfd,revbuf,LENGTH,0,(struct sockaddr*)&addr_remote,&sin_size)) == -1)
    {
	printf("ERROR!\n");
    }
    else
    {
	printf("OK:%s\n",revbuf);
    }
    close(sockfd);
    return (0);
}
