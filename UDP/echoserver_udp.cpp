#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>


using namespace std;
const int backLog = 3;
const int maxDataSize = 1460;

int main()
{

uint16_t serverPort=3002;
string serverIpAddr = "127.0.0.1";
cout<<"Enter the ip address and port number to listen the connections for"<<endl;
cin>>serverIpAddr;
cin>>serverPort;

//creating a socket using socket(family,type,protocol)
int Sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//checking the error while creationg socket
if(!Sock)
{
   cout<<"Error creating socket"<<endl;
   exit(1);
}
//creating a socket structure and filling the values in its components
struct sockaddr_in clientAddressInfo;
struct sockaddr_in serverSockAddressInfo;
serverSockAddressInfo.sin_family = AF_INET;
serverSockAddressInfo.sin_port = htons(serverPort);
serverSockAddressInfo.sin_addr.s_addr = INADDR_ANY;

// print info on the screen
inet_pton(AF_INET, serverIpAddr.c_str(), &(serverSockAddressInfo.sin_addr));
memset(&(serverSockAddressInfo.sin_zero), '\0', 8);
printf("Server listening on IP %x:PORT %d \n",serverSockAddressInfo.sin_addr.s_addr, serverPort);
//bind(sockid, &addrport, size);
int ret = bind(Sock, (struct sockaddr *)&serverSockAddressInfo, sizeof(struct sockaddr));

//error checking while binding the socket 
if(ret<0)
{
   cout<<"Error binding socket"<<endl;
   close(Sock);
   exit(1);
}

socklen_t client_address_len = 0;

while (true) {
		char rcvDataBuf[maxDataSize];
		memset(&rcvDataBuf, 0, maxDataSize);
		string rcvDataString;
		// read content into buffer from an incoming client
		int len = recvfrom(Sock, &rcvDataBuf, 1460, 0,
		                   (struct sockaddr *)&clientAddressInfo,
		                   &client_address_len);

		// send same content back to the client ("echo")
	sendto(Sock, &rcvDataBuf, len, 0, (struct sockaddr *)&clientAddressInfo, 		sizeof(clientAddressInfo));
	
rcvDataString=rcvDataBuf;
cout<<"Message sent:" ;
cout<<rcvDataString.c_str()<<endl;
	}


return 0;
}


