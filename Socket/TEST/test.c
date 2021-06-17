#include <stdio.h>//표준 입출력 버퍼 전처리
#include <stdlib.h>//표준 라이브러리 전처리
#include <unistd.h>// 운영체제 엑세스 제공 전처리
#include <string.h>//표준 입출력 장치 전처리
#include <arpa/inet.h>//숫자로 IP주소를 조작하는 전처리
#include <sys/socket.h>//소켓활용 전처리

#define BUF_SIZE 1000000
void error_handling(char *message);//에러 핸들링 하는 함수


int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;//서버 소켓과 클라이언트 소켓 선언
	FILE *fp;
	struct sockaddr_in serv_addr, clnt_addr;// 서버와 클라이언트 구조체 선언
	socklen_t clnt_addr_size;//클라이언트 사이즈
	char buf[BUF_SIZE];
	int read_cnt;
	char clnt_sd;
	char webpage[]="HTTP/1.1 200 OK\r\n"//클라이언트에게 보내줄 배열값
					"Server:Linux Web Server\r\n"
					"Content-Type: text/html; charset=UTF-8\r\n\r\n"
					"<!DOCTYPE html>\r\n"
					"<html><head><title>My Web Page</title>\r\n"
					"<style>body{background-color: #FFFF00}</style></head>\r\n"
					"<body><center><h1>Hello World!!<h1><br>\r\n"
					"<img src=\"ring.jpg\"></center></body></html>\r\n";
					
	if(argc!=2){
		printf("Usage: %s <port>\n",argv[0]);//포트 에러
		exit(1);//에러 발생시 종료
	}
	fp=fopen("ring.jpg","rb");
	serv_sock=socket(PF_INET,SOCK_STREAM,0);//IPv4, 연결지향형 소켓으로 서버를 만들었다.
	if(serv_sock==-1)//서버 소켓문제로 서버가 비정상적으로 종료되면
		error_handling("sock() error");//다음과 같은 메일을 띄운다.
	memset(&serv_addr,0,sizeof(serv_addr));//서버의 멤버를 초기화한다.
	serv_addr.sin_family=AF_INET;//IPv4 형식으로 초기화
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);//서버 IP를 빅엔디안 방식으로 초기화하고 IP주소형식
	serv_addr.sin_port=htons(atoi(argv[1]));//서버 포트를 빅엔디안 방식으로 변환하고 문자열 주소를 정수형으로 처리

	if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)//바인드 작업이다. 서버에 IP와 포트를 할당하는 것이다. 만약 이것의 할당이 잘못되었다면 
		error_handling("bind() error");//바인드 에러라는 문구와 함께 오류발생 표시메시지를 띄운다.
	if(listen(serv_sock,5)==-1)//5개까지 클라이언트를 받을 수 있다. 그리고 5개 이상이라서 에러가 발새한다면 아래의 에러구문을 띄운다. 데이터 접근에 대해 청취하는 것이다.
		error_handling("listen() error");
	clnt_addr_size=sizeof(clnt_addr);//클라이언트 사이즈는 clnt_addr구조체의 사이즈이다.
	clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);//만약 적합한 경로로 데이터 접근이 확인되었다면 클라이언트를 접속시킨다. 첫번째 인자는 서버 소켓을 의미한다. 
	while(1)
	{
		read_cnt=fread((void*)buf,1,BUF_SIZE,fp);
		if(read_cnt<BUF_SIZE)
		{
			write(clnt_sd,buf,read_cnt);
			break;
		}
		write(clnt_sock,buf,BUF_SIZE);
	}

	
	//그리고 두번 째 인자는 소켓 구조체의 클라이언트를 나타내고, 마지막으로 세번 째 인자는 그 사이즈를 나타낸다.
	if(clnt_sock==-1)//만약 접근 에러가 발생한다면
		error_handling("accept() error");//접근에러 구문을 띄운다.
	write(clnt_sock,webpage,sizeof(webpage));//클라이언트측에서 웹페이지에 대해 출력하는 함수이다. 첫번째 인자는 클라이언트, 두번째 이낮는 웹페이지 정보에 대한 배열주소, 세번 째 인자는
//웹페이지 배열의 사이즈를 나타낸다.
	fclose(fp);
	close(clnt_sock);//클라이언트소켓을 닫는다.
	close(serv_sock);//서버소켓을 닫는다.
	return 0;
}

void error_handling(char *message)//에러 발생 시 에러를 관리하는 함수이다.
{
	fputs(message,stderr);//메시지를 띄우는 데 그 메시지의 내용은 표준 에러에 대한 메시지를 띄운다.
	fputc('\n',stderr);//개행문자를 날린다.
	exit(1);//에러가 발생하면 정상적으로 종료한다.
}
