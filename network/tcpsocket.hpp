
/*
 *
 * 这个demo用来封装一个tcpsoket类，向外提供tcp协议通信接口，可实现服务端与客户端流程
 *
 */
       /*
 *                   1.创建套接字
 *                   2.绑定地址信息
 *                   3.开始监听
 *                   4.获取已完成连接
 *                   5.发送数据
 *                   6.接受数据
 *
       */
#include<iostream>
#include<string>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include <arpa/inet.h>
#define CHECK_RET(q) if((q) == false){return -1;}
class Tcpsocket{
    public:
        Tcpsocket():_sockfd(-1){}
        ~Tcpsocket(){Close();}
        bool Sock(){
            _sockfd = socket(AF_INET,SOCK_STREAM,0);
            if(_sockfd < 0){
              std::cerr << "create error\n";
                return false;
            }
            return true;
        }
        bool Bind(const std::string& ip,uint16_t port){
            struct sockaddr_in server_socket;
            server_socket.sin_family = AF_INET;
            server_socket.sin_port = htons(port);
            server_socket.sin_addr.s_addr = inet_addr(&ip[0]);
            int ret = bind(_sockfd,(struct sockaddr*)& server_socket,sizeof(sockaddr_in));
            if(ret < 0){
              std::cerr << "bind error\n";
                return false;
            }
            return true;
        }
        bool Listen(int BACKLOGS = 5){
            int ret = listen(_sockfd,BACKLOGS); 
            if(ret < 0){
              std::cerr << "listen error\n";
                return false;
            }
            return true;
        }
        void SetFd(int fd){
            _sockfd = fd;
        }
        bool Accept(Tcpsocket& cli_sock){
            struct sockaddr_in addr;
            socklen_t len = sizeof(struct sockaddr_in);
            int fd = accept(_sockfd,(struct sockaddr*)&addr,&len);
            if(fd < 0){
                std::cerr << "accept error\n";
                return false;
            }
            cli_sock.SetFd(fd);
            return true;
        }
        bool Connect(std::string& ip,uint16_t port){
            int ret;
            struct sockaddr_in client_socket;
            client_socket.sin_family = AF_INET;
            client_socket.sin_port = htons(port);
            client_socket.sin_addr.s_addr = inet_addr(&ip[0]);
            socklen_t len = sizeof(struct sockaddr_in);
            ret = connect(_sockfd,(struct sockaddr*)& client_socket,len);
            if(ret < 0) {
                std::cerr << "connect error\n";
                return false;
            }
            return true;
        }
        bool Send(std::string buff){
            int ret = send(_sockfd,&buff[0],buff.size(),0);
            if(ret < 0){
                std::cerr << "send error\n";
                return false;
            }
            return true;
        }
        bool Recv(std::string buff){
            char tmp[4096] = {0};
            int ret = recv(_sockfd,tmp,4096,0);
            if(ret < 0){
                std::cerr << "recv error\n";
                return false;
            }else if(ret == 0){           //recv返回值为0表示连接已断开
                std::cerr << "peer shutdown\n";
                return false;
            }
            buff.assign(tmp,ret);
            return true;
        }
        bool Close(){
            if(_sockfd >= 0){
                close(_sockfd);
                _sockfd = -1;
            }
        }
  private:
      int _sockfd;
};
