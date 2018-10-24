// Server.cpp : Defines the entry point for the console application.
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
using namespace std;

int InitSocket()
{
	WSADATA            wsd;            //WSADATA变量
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return 1;
	}
	return 2;
}
int CreateSocket(SOCKET &sServer)
{
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		cout << "socket failed!" << endl;
		WSACleanup();//释放套接字资源;
		return  -1;
	}
	return 2;
}
int BindingSocket(int &RetValue, SOCKET &sServer, SOCKADDR_IN &addrServ)
{
	RetValue = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == RetValue)
	{
		cout << "bind failed!" << endl;
		closesocket(sServer);    //关闭套接字
		WSACleanup();            //释放套接字资源;
		return -1;
	}
	return 2;
}
int StartMonitor(int &retVal,SOCKET &sServer)
{
	retVal = listen(sServer, 1);
	if (SOCKET_ERROR == retVal)
	{
		cout << "listen failed!" << endl;
		closesocket(sServer);    //关闭套接字
		WSACleanup();            //释放套接字资源;
		return -1;
	}
	return 2;
}
int ReceiveClientRequest(sockaddr_in &addrClient,SOCKET &sServer,SOCKET &sClient)
{
	int addrClientlen = sizeof(addrClient);
	sClient = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientlen);
	if (INVALID_SOCKET == sClient)
	{
		cout << "accept failed!" << endl;
		closesocket(sServer);    //关闭套接字
		WSACleanup();            //释放套接字资源;
		return -1;
	}
	return 2;
}

int main()
{
	const int BUF_SIZE = 64;
	WSADATA            wsd;            //WSADATA变量
	SOCKET            sServer;        //服务器套接字
	SOCKET            sClient;        //客户端套接字
	SOCKADDR_IN        addrServ;        //服务器地址
	char            buf[BUF_SIZE];    //接收数据缓冲区
	char            sendBuf[BUF_SIZE];//返回给客户端得数据
	int                retVal;            //返回值
	//初始化套结字动态库
	InitSocket();

	//创建套接字
	CreateSocket(sServer);

	//服务器套接字地址 
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(4999);
	addrServ.sin_addr.s_addr = INADDR_ANY;
	//绑定套接字
	BindingSocket(retVal,sServer,addrServ);

	//开始监听 
	StartMonitor(retVal,sServer);

	//接受客户端请求
	sockaddr_in addrClient;
	//int addrClientlen = sizeof(addrClient);
	ReceiveClientRequest(addrClient,sServer, sClient);

	while (true)
	{
		//接收客户端数据
		ZeroMemory(buf, BUF_SIZE);
		retVal = recv(sClient, buf, BUF_SIZE, 0);
		if (SOCKET_ERROR == retVal)
		{
			cout << "recv failed!" << endl;
			closesocket(sServer);    //关闭套接字
			closesocket(sClient);    //关闭套接字        
			WSACleanup();            //释放套接字资源;
			return -1;
		}
		if (buf[0] == '0'){ break; }

		cout << "客户端发送的数据: " << buf << endl;

		cout << "向客户端发送数据: ";
		cin >> sendBuf;

		send(sClient, sendBuf, strlen(sendBuf), 0);
	}

	//退出
	closesocket(sServer);    //关闭套接字
	closesocket(sClient);    //关闭套接字
	WSACleanup();            //释放套接字资源;

	return 0;
}