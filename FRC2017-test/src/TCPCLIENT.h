#ifndef TcpClient_H_
#define TcpClient_H_

#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>
#include<string>

using namespace std;

class TcpClient
{
private:
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;

public:
    TcpClient();
    bool conn(string, int);
    bool sendData(string data);
    string receive(int);
};

#endif
