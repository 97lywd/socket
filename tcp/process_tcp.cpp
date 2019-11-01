#include<cstdlib>
#include<signal.h>
#include<sys/wait.h>
#include"tcpsocket.hpp"

void sigcb(int signo)
{
    //SIGCHLD是一个非可靠信号
    //多个信号同时退出，有可能造成事件丢失，导致僵尸进程未处理
    while(waitpid(-1,NULL,WNOHANG) > 0);
}
int main(int argc, char* argv[])
{
     if(argc != 3){
        std::cout << "./tcp_srv 192.168.116.129 9000\n";
        return false;
     } 
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    Tcpsocket lst_sock;
    Tcpsocket newsock;
    //1.创建套接字
    CHECK_RET(lst_sock.Sock());
    //2.绑定地址信息
    CHECK_RET(lst_sock.Bind(ip,port));
    //3.开始监听
    CHECK_RET(lst_sock.Listen());
    while(1){
        //4.获取已完成新连接
      bool ret = lst_sock.Accept(newsock);
      if(ret == false){
          continue;
      }
    //创建子进程进行任务处理
    //进程只负责一个任务，主进程负责新连接获取，子进程负责与客户端进行通信
    if(fork() == 0){
        while(1){
            std::string buff;
            ret = newsock.Recv(buff);
            if(ret == false){
                newsock.Close();
                continue;
             }
             std::cout << "client say:" << buff << "\n";
             buff.clear();
             std::cin >> buff;
             newsock.Send(buff);
        }
          newsock.Close();
          exit(0);
      }
    newsock.Close();
    }
    //关闭套接字
    lst_sock.Close();
    return 0;
}
