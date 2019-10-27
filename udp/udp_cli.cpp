

/*
 *
 * 封装实现一个udpsocket类；向外提供更加容易使用的udp接口来实现udp实现流程
 *
 */


#include<iostream>
#include<string>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<stdlib.h>

class Udpsocket{
    public:
        Udpsocket():_sockfd(-1){
        }
        ~Udpsocket(){
            Close();
        }
        bool Socket(){
            _sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
            if(_sockfd < 0){
                std::cerr << "socket error\n";
                return false;
            }
            return true;
        }
        bool Bind(const std::string &ip,uint16_t port){
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            socklen_t len = sizeof(struct sockaddr_in);
            int ret = bind(_sockfd,(struct sockaddr*)& addr,len);
            if(ret < 0){
                std::cerr << "bind error\n";
                return false;
            }
            return true;
        }
        bool Recv(std::string& buff,std::string& ip, uint16_t& port){
            char tmp[4096];
            struct sockaddr_in peeraddr;
            socklen_t len = sizeof(struct sockaddr_in);
            int ret = recvfrom(_sockfd,tmp,4096,0,(struct sockaddr*)& peeraddr,&len);
            if(ret < 0){
                std::cerr << "recvfrom error\n";
                return false;
            }
            buff.assign(tmp,ret);
            port = ntohs(peeraddr.sin_port);
            ip = inet_ntoa(peeraddr.sin_addr);
            // inet_ntoa 将网络字节序的整数ip地址转换为字符串ip地址
            // 返回缓冲区首地址，内部实现使用了静态成员变量
            // 非线程安全
            return true;
        }
        bool Send(std::string &data,std::string& ip, uint16_t port){
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            socklen_t len = sizeof(struct sockaddr_in);
            int ret = sendto(_sockfd,&data[0],data.size(),0,(struct sockaddr*)& addr,len);
            if(ret < 0){
                std::cerr << "sendto error\n";
                return false;
            }
            return true;
        }
        bool Close(){
            if(_sockfd >= 0){
                close(_sockfd);
                _sockfd = -1;
            }
            return true;
        }
    private:
        int _sockfd;
};

#define CHECK_RET(q) if((q) == false){return -1;}
int main(int argc, char* argv[])
{
    if(argc != 3){
      std::cout << "./udp_cli serverip serverport";
      return -1;
    }
    std::string srv_ip = argv[1];
    uint16_t srv_port = atoi(argv[2]);
    Udpsocket sock;
    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Bind("192.168.116.129",8000));
    while(1){
        std::string buff;
        std::cin >> buff;
        CHECK_RET(sock.Send(buff,srv_ip,srv_port));
        buff.clear();
        CHECK_RET(sock.Recv(buff,srv_ip,srv_port));
        std::cout << "server say:\n" << buff << "\n";
    }
    CHECK_RET(sock.Close());
    return 0;
}
