// Server.cpp : Defines the entry point for the console application.
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
using namespace std;

int InitSocket()
{
	WSADATA            wsd;            //WSADATA����
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
		WSACleanup();//�ͷ��׽�����Դ;
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
		closesocket(sServer);    //�ر��׽���
		WSACleanup();            //�ͷ��׽�����Դ;
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
		closesocket(sServer);    //�ر��׽���
		WSACleanup();            //�ͷ��׽�����Դ;
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
		closesocket(sServer);    //�ر��׽���
		WSACleanup();            //�ͷ��׽�����Դ;
		return -1;
	}
	return 2;
}

int main()
{
	const int BUF_SIZE = 64;
	WSADATA            wsd;            //WSADATA����
	SOCKET            sServer;        //�������׽���
	SOCKET            sClient;        //�ͻ����׽���
	SOCKADDR_IN        addrServ;        //��������ַ
	char            buf[BUF_SIZE];    //�������ݻ�����
	char            sendBuf[BUF_SIZE];//���ظ��ͻ��˵�����
	int                retVal;            //����ֵ
	//��ʼ���׽��ֶ�̬��
	InitSocket();

	//�����׽���
	CreateSocket(sServer);

	//�������׽��ֵ�ַ 
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(4999);
	addrServ.sin_addr.s_addr = INADDR_ANY;
	//���׽���
	BindingSocket(retVal,sServer,addrServ);

	//��ʼ���� 
	StartMonitor(retVal,sServer);

	//���ܿͻ�������
	sockaddr_in addrClient;
	//int addrClientlen = sizeof(addrClient);
	ReceiveClientRequest(addrClient,sServer, sClient);

	while (true)
	{
		//���տͻ�������
		ZeroMemory(buf, BUF_SIZE);
		retVal = recv(sClient, buf, BUF_SIZE, 0);
		if (SOCKET_ERROR == retVal)
		{
			cout << "recv failed!" << endl;
			closesocket(sServer);    //�ر��׽���
			closesocket(sClient);    //�ر��׽���        
			WSACleanup();            //�ͷ��׽�����Դ;
			return -1;
		}
		if (buf[0] == '0'){ break; }

		cout << "�ͻ��˷��͵�����: " << buf << endl;

		cout << "��ͻ��˷�������: ";
		cin >> sendBuf;

		send(sClient, sendBuf, strlen(sendBuf), 0);
	}

	//�˳�
	closesocket(sServer);    //�ر��׽���
	closesocket(sClient);    //�ر��׽���
	WSACleanup();            //�ͷ��׽�����Դ;

	return 0;
}