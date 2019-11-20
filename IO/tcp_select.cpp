/*
 *
 * 基于select实现一个并发tcp服务器，对select进行封装，
 * 封装一个类实例化一个对象tcpsocket
 *
 */
#include<iostream>
#include<vector>
#include<string>
#include<sys/select.h>
#include"tcpsocket.hpp"
class Select{
public:
    Select(){}
    ~Select(){}
    bool Add(Tcpsocket &sock){
        int fd = sock.GetFd();
        FD_SET(fd,&_rfds);
        _maxfd = _maxfd > fd ? _maxfd : fd;
        return true;
    }
    bool Del(Tcpsocket &sock){
        int fd = sock.GetFd();
        FD_CLR(fd,&_rfds);
        for(int i = _maxfd; i >= 0; i--){
            if(FD_ISSET(i,&_rfds)){
                _maxfd = i;
                return true;
            }
        }
        _maxfd = -1;
        return true;
    }
    bool Wait(std::vector<Tcpsocket> &list,int sec = 3){
        struct timeval tv;
        tv.tv_sec = sec;
        tv.tv_usec = 0;
        fd_set tmp = _rfds;
        int count = select(_maxfd + 1,&tmp,NULL,NULL,&tv);
        if(count < 0){
            std::cout << "select error\n";
            return false;
        }else if(count == 0){
            std::cout << "wait timeout\n";
            return false;
        }
        for(int i = 0; i <= _maxfd; i++)
        {
            if(FD_ISSET(i,&_rfds)){
                Tcpsocket sock;
                sock.SetFd(i);
                list.push_back(sock);
            }
        }
        return true;
    }
private:
      fd_set _rfds;
      int _maxfd;
};



int main(int argc, char* argv[])
{
    if(argc != 3){
        std::cout << "./ip port\n";
        return -1;
    }
    std::string srv_ip = argv[1];
    uint16_t srv_port = atoi(argv[2]);
    Tcpsocket sock;
    CHECK_RET(sock.Sock());
    CHECK_RET(sock.Bind(srv_ip,srv_port));
    CHECK_RET(sock.Listen());
    Select s;
    s.Add(sock);
    while(1){
        std::vector<Tcpsocket> list;
        if(s.Wait(list) == false)
            continue;
        for(auto clisock : list){
            if(clisock.GetFd() == sock.GetFd()){
                //当前就绪的描述符是监听套接字 --- accept
                Tcpsocket clisock;
                if(sock.Accept(clisock) == false){
                     continue;
                }
                s.Add(clisock);
            }else{
                //当前就绪的描述符是通信套接字，应该recv
                std::string buff;
                if(clisock.Recv(buff) == false){
                    s.Del(clisock);
                    clisock.Close();
                    continue;
                }
                std::cout << "client say:" << buff << "\n";
                buff.clear();
                std::cin >> buff;
                if(clisock.Send(buff) == false){
                    s.Del(clisock);
                    clisock.Close();
                    continue;
                }
            }
        }
    }
    sock.Close();
    return 0;
}
