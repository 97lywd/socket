#include<cstdlib>
#include"tcpsocket.hpp"

void *thr_start(void* arg)
{
    Tcpsocket* newsock = (Tcpsocket*)arg;
    while(1){
        std::string buff;
        bool ret = newsock->Recv(buff);
        if(ret == false){
            newsock->Close();
            continue;
        }
    std::cout << "client say:" << buff << "\n";
    buff.clear();
    std::cin >> buff;
    newsock->Send(buff);
    }
    newsock->Close();
    return NULL;
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
    //创建新线程处理与客户端的通信
      pthread_t tid;
      pthread_create(&tid,NULL,thr_start,(void*)& newsock);
      pthread_detach(tid);
    }
    //7.关闭套接字
    lst_sock.Close();
    return 0;
}
