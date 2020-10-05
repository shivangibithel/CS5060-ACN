#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<pthread.h>


using namespace std;
const int backLog = 3;
const int maxDataSize = 1460;

main()
{

uint16_t serverPort=3002;
string serverIpAddr = "127.0.0.1";
cout<<"Enter the ip address and port number to listen the connections for"<<endl;
cin>>serverIpAddr;
cin>>serverPort;

//creating a socket using socket(family,type,protocol)
int serverSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//checking the error while creationg socket
if(!serverSocketFd)
{
   cout<<"Error creating socket"<<endl;
   exit(1);
}
//creating a socket structure and filling the values in its components
struct sockaddr_in serverSockAddressInfo;
serverSockAddressInfo.sin_family = AF_INET;
serverSockAddressInfo.sin_port = htons(serverPort);
serverSockAddressInfo.sin_addr.s_addr = INADDR_ANY;

// print info on the screen
inet_pton(AF_INET, serverIpAddr.c_str(), &(serverSockAddressInfo.sin_addr));
memset(&(serverSockAddressInfo.sin_zero), '\0', 8);
printf("Server listening on IP %x:PORT %d \n",serverSockAddressInfo.sin_addr.s_addr, serverPort);


//bind(sockid, &addrport, size);
int ret = bind(serverSocketFd, (struct sockaddr *)&serverSockAddressInfo, sizeof(struct sockaddr));

//error checking while binding the socket 
if(ret<0)
{
   cout<<"Error binding socket"<<endl;
   close(serverSocketFd);
   exit(1);
}
//ret =-1...means error
//listen(sockid, queueLimit);
ret = listen(serverSocketFd, backLog);
//error checking
if(!serverSocketFd)
{
   cout<<"Error listening socket"<<endl;
   close(serverSocketFd);
   exit(1);
}

socklen_t sinSize = sizeof(struct sockaddr_in);
int flags = 0;
int dataRecvd = 0, dataSent = 0;
struct sockaddr_in clientAddressInfo;
char rcvDataBuf[maxDataSize], sendDataBuf[maxDataSize];
//string sendDataStr;
string rcvDataString;
   memset(&clientAddressInfo, 0, sizeof(struct sockaddr_in));
   memset(&rcvDataBuf, 0, maxDataSize);

   //accept(sockid, &clientAddr, &addrLen);
   int newClientFd = accept(serverSocketFd, (struct sockaddr *)&clientAddressInfo, &sinSize);
   if (!newClientFd)
   {
       cout<<"Error with new client connection "<<endl;
       close(serverSocketFd);
       exit(1);
   }

   cout<<"New client arrived"<<endl;
   cin.ignore();
int recvMsgSize=1;
while(recvMsgSize)
{
   memset(&rcvDataBuf, 0, maxDataSize);
   if ((recvMsgSize = recv(newClientFd, &rcvDataBuf, maxDataSize, flags)) < 0)
	{
   cout<<"recv() failed"<<endl;
   close(serverSocketFd);
   exit(1);
	}		

	if(recvMsgSize)
		{  send(newClientFd, &rcvDataBuf, recvMsgSize, flags);
		rcvDataString=rcvDataBuf;		
		cout<<"Message sent:" ;
		cout<<rcvDataString.c_str()<<endl;

		if(!strcmp(rcvDataBuf, "bye"))
	   	{
	      		close(newClientFd);
	      		break;
	   	}
		}
}
cout<<"Client closed "<<endl;
close(newClientFd);

}


