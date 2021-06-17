#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
void error_handling(char *message);


int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size;
	
	char webpage[]="HTTP/1.1 200 OK\r\n"
					"Server:Linux Web Server\r\n"
					"Content-Type: text/html; charset=UTF-8\r\n\r\n"
					"<!DOCTYPE html>\r\n"
					"<html><head><title>My Web Page</title>\r\n"
					"<style>body{background-color: #FFFF00}</style></head>\r\n"
					"<body><center><h1>Hello World!!<h1><br>\r\n"
					"<img src=\"D\ring.png\"></center></body></html>\r\n";
	if(argc!=2){
		printf("Usage: %s <port>\n",argv[0]);
		exit(1);
	}
	serv_sock=socket(PF_INET,SOCK_STREAM,0);
	if(serv_sock==-1)
		error_handling("sock() error");
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock,5)==-1)
		error_handling("listen() error");
	clnt_addr_size=sizeof(clnt_addr);
	clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
	if(clnt_sock==-1)
		error_handling("accept() error");
	write(clnt_sock,webpage,sizeof(webpage));
	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
