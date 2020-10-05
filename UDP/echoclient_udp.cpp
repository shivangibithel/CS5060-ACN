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

main()
{

	uint16_t serverPort=3002;
	string serverIpAddr = "127.0.0.1";
	cout<<"Enter the ip address and port number to listen the connections for"<<endl;
	cin>>serverIpAddr;
	cin>>serverPort;

	int sock= socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(!sock)
	{
	   cout<<"Error creating socket"<<endl;
	   exit(1);
	}

	struct sockaddr_in serverSockAddressInfo;
	serverSockAddressInfo.sin_family = AF_INET;
	serverSockAddressInfo.sin_port = htons(serverPort);
	serverSockAddressInfo.sin_addr.s_addr = INADDR_ANY;
	inet_pton(AF_INET, serverIpAddr.c_str(), &(serverSockAddressInfo.sin_addr));

	memset(&(serverSockAddressInfo.sin_zero), '\0', 8);


	socklen_t sinSize = sizeof(struct sockaddr_in);
	int flags = 0;
	struct sockaddr_in clientAddressInfo;
	char rcvDataBuf[maxDataSize], sendDataBuf[maxDataSize];

	 	
	   string sendDataStr, rcvDataStr; 	
	   cin.ignore();
	   cout<<"Enter data to send"<<endl;
	   memset(&sendDataBuf, 0, maxDataSize);
	   cin.clear();
	   getline(cin, sendDataStr);
	   socklen_t server_len;
	   
	  int dataSent = sendto(sock, sendDataStr.c_str(), sendDataStr.length(), flags,(struct sockaddr *)&serverSockAddressInfo, sizeof(serverSockAddressInfo));
		//cout<<dataSent;
	   cout<<"Data sent by server"<<endl;

	   memset(&rcvDataBuf, 0, maxDataSize);
	   int dataRecvd = recvfrom(sock, &rcvDataBuf, maxDataSize, flags,(struct sockaddr *)&serverSockAddressInfo,&server_len);
	   rcvDataStr = rcvDataBuf;
	   cout<<rcvDataStr.c_str()<<endl;
	

	close(sock);

}

