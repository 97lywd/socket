#include<cstdlib>
#include"tcpsocket.hpp"
int main(int argc, char* argv[])
{
  if(argc != 3){
      std::cout << "./tcp_cli severip severport\n";
      return -1;
  }
  Tcpsocket sock;
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
    //1.创建套接字
    CHECK_RET(sock.Sock());
    //2.绑定地址信息
    //3.向服务端发起连接请求
    CHECK_RET(sock.Connect(ip,port));
    while(1){
        //4.发送数据
      std::string buff;
      std::cin >> buff;
      bool ret = sock.Send(buff);
      if(ret == false){
          sock.Close();
          return -1;
      }
        //5.接受数据
      buff.clear();
      ret = sock.Recv(buff);
      if(ret == false){
          sock.Close();
          return -1;
      }
      std::cout << "sever say:" << buff << "\n";
    }
    //6.关闭套接字
    sock.Close();
    return 0;
}
