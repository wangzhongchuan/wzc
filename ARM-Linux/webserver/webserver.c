#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define PORT 81
#define BACKLOG 10
#define LENGTH 1024

char httpweb[] = {
    "HTTP/1.0 200OK\r\n"
    "Date:Mon,24 Nov2005 10:26:00 GMT\r\n"
    "Server:microHttp/1.0 ZHIYUAN Electronics CO.,LTD\r\n"
    "Accept-Ranges:bytes\r\n"
    "Connection:Keep-Close\r\n"
    "Content-Type:text/htm\r\n"
    "\r\n"	
};

char web[] = {
    "<HTML>\r\n"
    "<HEAD>\r\n"
    "<TITLE>MagicARM241 yanshiwangye </TITLE>\r\n"
    "<BODY>aLink=green bgColor=#flfdd link=red\r\n"
    "vLink=#32lafd>\r\n"
    "<H1>HELLO WELCOME TO MagicARM2410 WEBSERVER</H1>\r\n"
    "<UL>\r\n"
    "<LI><A HREF=\"http://www.zyinside.com\">gzzydianzi </A> \r\n"
    "</BODY>\r\n"
    "<HTML>\r\n"
};

char httpgif[] = {

    "HTTP/1.0 200OK\r\n"
    "Date:Mon,24 Nov2005 10:26:00 GMT\r\n"
    "Server:microHttp/1.0 ZHIYUAN Electronics CO.,LTD\r\n"
    "Accept-Ranges:bytes\r\n"
    "Connection:Keep-Close\r\n"
    "Content-Type:image/bmp\r\n"
    "\r\n"	
};

int main()
{
    int sockfd;
    int nsockfd;
    int i,num;
    int flag = 1;
    int sin_size;
    char revbuf[LENGTH];
    struct sockaddr_in addr_local;
    struct sockaddr_in addr_remote;

    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
	printf("ERROR:Cannot obtain Socket Descriptor!\n");
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

    if(bind(sockfd,(struct sockaddr*)&addr_local,sizeof(struct sockaddr)) == 1)
    {
	printf("ERROR:Cannot bind Port%d.\n",PORT);
	return 0;
    }

    else
    {
	printf("OK:Bind the Port%d successfully.!\n",PORT);
    }

    if(listen(sockfd,BACKLOG) == -1)
    {
    	printf("ERROR:Cannot listen Port%d.\n",PORT);
        return 0; 
    }
    else
    {
	printf("OK:Listening the Port%d successfully.!\n",PORT);
    }

    while(1)
    {
	sin_size  = sizeof(struct sockaddr_in);

	if((nsockfd = accept(sockfd,(struct sockaddr*)&addr_remote,&sin_size)) == -1) 
    	{
    	    printf("ERROR:Obtain new Socked Descriptor error.\n");
            continue; 
    	}
    	else
    	{
	    printf("OK:Server has got connect from%s.\n",inet_ntoa(addr_remote.sin_addr));
    	}

	num = recv(nsockfd,revbuf,LENGTH,0);
	
	if(!fork())
	{
	    printf("OK:Http web is servering.\n");
	    if(revbuf[5] == ' ')
	    {
		send(nsockfd,httpweb,sizeof(httpweb),0);
		send(nsockfd,web,sizeof(web),00);
	    }	
	    else if(revbuf[5] == '1')
	    {
		send(nsockfd,httpgif,sizeof(httpgif),0);
	    }	    	
	}
	close(nsockfd);
	while(waitpid(-1,NULL,WNOHANG) > 0);
    }
}




















