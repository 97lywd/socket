/*
 * 实现一个简单的http服务器
 *
 */

#include"tcpsocket.hpp"
#include<sstream>
int main(int argc, char* argv[])
{
    Tcpsocket sock;
    CHECK_RET(sock.Sock());
    CHECK_RET(sock.Bind("0.0.0.0",9000));
    CHECK_RET(sock.Listen());
    while(1)
    {
        Tcpsocket clisock;
        if(sock.Accept(clisock) == false){
            continue;
        }
        std::string buff;
        clisock.Recv(buff);
        std::cout << "req:[" << buff << "]\n";
        std::string body = "<html><body><h1>hello</h1></body></html>";
        std::string first = "HTTP/1.1 200 OK\r\n";
        std::string head;
        std::stringstream ss;
        ss << "Content-Length: " << body.size() << "\r\n";
        head = ss.str();
        std::string blank = "\r\n";
        clisock.Send(first);
        clisock.Send(head);
        clisock.Send(blank);
        clisock.Send(body);
        clisock.Close();
    }
    sock.Close();
    return 0;
}
