#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>
#include<string>
#include "TcpClient.h"

TcpClient::TcpClient()
{
    sock = -1;
    port = 0;
    address = "";
}

bool TcpClient::conn(string address , int port)
{
    if(sock == -1)
    {
        sock = socket(AF_INET , SOCK_STREAM , 0);
    }


    if(inet_addr(address.c_str()) == -1)
    {
        struct hostent *he;
        struct in_addr **addr_list;

        if ( (he = gethostbyname( address.c_str() ) ) == NULL)
        {
            return false;
        }

        addr_list = (struct in_addr **) he->h_addr_list;

        for(int i = 0; addr_list[i] != NULL; i++)
        {
            server.sin_addr = *addr_list[i];

            break;
        }
    }

    else
    {
        server.sin_addr.s_addr = inet_addr( address.c_str() );
    }

    server.sin_family = AF_INET;
    server.sin_port = htons( port );
    connect(sock , (struct sockaddr *)&server , sizeof(server));
    return true;
}


bool TcpClient::sendData(string data)
{
    send(sock , data.c_str() , strlen( data.c_str() ) , 0);

    return true;
}


string TcpClient::receive(int size=512)
{
    char buffer[size];
    string data;

    recv(sock , buffer , sizeof(buffer) , 0);

    data = buffer;
    return data;
}
