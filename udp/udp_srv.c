/*
 *
 * 传输层基于udp协议的服务端程序
 * 1.创建套接字
 * 2.绑定地址信息
 * 3.接收数据
 * 4.发送数据
 * 5.关闭套接字
 */

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
int main(int argc, char* argv[])
{
  if(argc != 3){
      printf("Usage: ./main 192.168.116.129 9000\n");
  }
  //创建套接字
  int sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  if(sockfd < 0){
      perror("create error");
      return -1;
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[2]));
  addr.sin_addr.s_addr = inet_addr("192.168.116.129");
  //绑定地址信息
  socklen_t len = sizeof(struct sockaddr_in);
  int ret = bind(sockfd,(struct sockaddr*)&addr,len);
  if(ret < 0){
      perror("bind error");
      return -1;
  }
  //接收信息
  while(1){
      char buff[1024] = {0};
      struct sockaddr_in cliaddr;
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = recvfrom(sockfd,buff,1023,0,(struct sockaddr*)& cliaddr,&len); //0默认阻塞
      if(ret < 0){
          perror("recvfrom error");
          close(sockfd);
          return -1;
      }
      printf("client say: %s\n",buff);

      //发送信息
      memset(buff,0x00,1024);
      scanf("%s",buff);
      ret = sendto(sockfd,buff,strlen(buff),0,(struct sockaddr*)& cliaddr,len);
      if(ret < 0){
          perror("sendto error");
          close(sockfd);
          return -1;
      }
  }
  return 0;
}
