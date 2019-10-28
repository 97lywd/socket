#include<cstdlib>
#include"tcpsocket.hpp"


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
    //5.接受数据
    std::string buff;
    ret = newsock.Recv(buff);
    if(ret == false){
        newsock.Close();
        continue;
    }
    std::cout << "client say:" << buff << "\n";
    //6.发送数据
    buff.clear();
    std::cin >> buff;
    newsock.Send(buff);
    }
    //7.关闭套接字
    lst_sock.Close();
    return 0;
}
