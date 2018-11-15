#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <Windows.h>
#pragma comment(lib,"..\\Debug\\ServerDll.lib")
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
int StartMonitor(int &retVal, SOCKET &sServer)
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
int ReceiveClientRequest(sockaddr_in &addrClient, SOCKET &sServer, SOCKET &sClient)
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
	//const int BUF_SIZE = 64;
	//WSADATA            wsd;            //WSADATA����
	//SOCKET            sServer;        //�������׽���
	//SOCKET            sClient;        //�ͻ����׽���
	//SOCKADDR_IN        addrServ;        //��������ַ
	//char            buf[BUF_SIZE];    //�������ݻ�����
	//char            sendBuf[BUF_SIZE];//���ظ��ͻ��˵�����
	//int                retVal;            //����ֵ
	////��ʼ���׽��ֶ�̬��
	//InitSocket();
	////�����׽���
	//CreateSocket(sServer);
	////�������׽��ֵ�ַ 
	//addrServ.sin_family = AF_INET;
	//addrServ.sin_port = htons(4999);
	//addrServ.sin_addr.s_addr = INADDR_ANY;
	////���׽���
	//BindingSocket(retVal,sServer,addrServ);
	////��ʼ���� 
	//StartMonitor(retVal,sServer);
	////���ܿͻ�������
	//sockaddr_in addrClient;
	////int addrClientlen = sizeof(addrClient);
	//ReceiveClientRequest(addrClient,sServer, sClient);
	//while (true)
	//{
	//	//���տͻ�������
	//	ZeroMemory(buf, BUF_SIZE);
	//	retVal = recv(sClient, buf, BUF_SIZE, 0);
	//	if (SOCKET_ERROR == retVal)
	//	{
	//		cout << "recv failed!" << endl;
	//		closesocket(sServer);    //�ر��׽���
	//		closesocket(sClient);    //�ر��׽���        
	//		WSACleanup();            //�ͷ��׽�����Դ;
	//		return -1;
	//	}
	//	if (buf[0] == '0'){ break; }
	//	cout << "�ͻ��˷��͵�����: " << buf << endl;
	//	cout << "��ͻ��˷�������: ";
	//	cin >> sendBuf;
	//	send(sClient, sendBuf, strlen(sendBuf), 0);
	//}
	////�˳�
	//closesocket(sServer);    //�ر��׽���
	//closesocket(sClient);    //�ر��׽���
	//WSACleanup();            //�ͷ��׽�����Դ;

	int num1 = 30;
	int num2 = 40;
	HMODULE hDll = LoadLibraryA("..\\Debug\\ServerDll.dll");
	typedef int(*Function)(int&, int&);
	typedef int(*__InitSocket)();
	typedef int(*__CreateSocket)(SOCKET&);
	typedef int(*__BindingSocket)(int&, SOCKET&, u_short);
	typedef int(*__StartMonitor)(int&, SOCKET&);
	typedef int(*__ReceiveClientRequest)(SOCKET&, SOCKET&);
	typedef int(*__ReceiveClientData)(SOCKET&, SOCKET&, char*, int, int&);
	typedef int(*__ServerLogic)(char *Data);
	typedef int(*__SendClientData)(SOCKET&, char*);
	typedef int(*__ExitServer)(SOCKET&, SOCKET&);
	if (hDll)
	{
		Function function = (Function)GetProcAddress(hDll, "Function");
		int num = function(num1, num2);
		std::cout << "Ok " << num << std::endl;
		std::cout << "Num1:" << num1 << std::endl;
		std::cout << "Num2:" << num2 << std::endl;

		SOCKET sServer;
		SOCKET sClient;
		int RetValue;
		char Buf[64] = { 0 };
		u_short Port = 4999;

		__InitSocket InitSocket = (__InitSocket)GetProcAddress(hDll, "InitSocket");
		int InitSocketReturn = InitSocket();
		if (InitSocketReturn == 2)
		{
			std::cout << "��ʼ���ɹ���" << std::endl;
		}
		__CreateSocket CreateSocket = (__CreateSocket)GetProcAddress(hDll, "CreateSocket");
		int CreateSocketReturn = CreateSocket(sServer);
		if (CreateSocketReturn == 2)
		{
			std::cout << "�������ֽڳɹ���" << std::endl;
		}
		__BindingSocket BindingSocket = (__BindingSocket)GetProcAddress(hDll, "BindingSocket");
		int BindingSocketReturn = BindingSocket(RetValue, sServer, Port);
		if (BindingSocketReturn == 2)
		{
			std::cout << "�����ֽڳɹ���" << std::endl;
		}
		__StartMonitor StartMonitor = (__StartMonitor)GetProcAddress(hDll, "StartMonitor");
		int StartMonitorReturn = StartMonitor(RetValue, sServer);
		if (StartMonitorReturn == 2)
		{
			std::cout << "��ʼ�����ɹ���" << std::endl;
		}
		__ReceiveClientRequest ReceiveClientRequest = (__ReceiveClientRequest)GetProcAddress(hDll, "ReceiveClientRequest");
		int ReceiveClientRequestReturn = ReceiveClientRequest(sServer, sClient);
		if (ReceiveClientRequestReturn == 2)
		{
			std::cout << "���ܿͻ�������ɹ���" << std::endl;
		}
		__ReceiveClientData ReceiveClientData = (__ReceiveClientData)GetProcAddress(hDll, "ReceiveClientData");
		int ReceiveClientDataReturn = 0;
		int State = 0;
		while (true)
		{
			ReceiveClientDataReturn = ReceiveClientData(sServer, sClient, Buf, 64, RetValue);
			if (ReceiveClientDataReturn == -1)
			{
				std::cout << "�ͻ����˳���" << std::endl;
				closesocket(sClient);    //�ر��׽���
				sClient = NULL;
				if (State == 0)
				{
					State = 1;
				}
				if (State == 1)
				{
					int InitSocketReturn = InitSocket();
					if (InitSocketReturn == 2)
					{
						std::cout << "��ʼ���ɹ���" << std::endl;
					}
					int CreateSocketReturn = CreateSocket(sServer);
					if (CreateSocketReturn == 2)
					{
						std::cout << "�������ֽڳɹ���" << std::endl;
					}
					int BindingSocketReturn = BindingSocket(RetValue, sServer, Port);
					if (BindingSocketReturn == 2)
					{
						std::cout << "�����ֽڳɹ���" << std::endl;
					}
					int StartMonitorReturn = StartMonitor(RetValue, sServer);
					if (StartMonitorReturn == 2)
					{
						std::cout << "��ʼ�����ɹ���" << std::endl;
					}
					int ReceiveClientRequestReturn = ReceiveClientRequest(sServer, sClient);
					if (ReceiveClientRequestReturn == 2)
					{
						std::cout << "���ܿͻ�������ɹ���" << std::endl;
						ReceiveClientDataReturn = 0;//ֱ���пͻ������ӳɹ�
					}
					State = 0;
				}
			}
			std::cout << Buf << std::endl;

			if (sClient != NULL)
			{
				__SendClientData SendClientData = (__SendClientData)GetProcAddress(hDll, "SendClientData");
				char SendData[64] = "ServerOk Send Sclient";
				int SendClientDataReturn = SendClientData(sClient, SendData);
				if (SendClientDataReturn == 1)
				{
					std::cout << "���������ͳɹ���" << std::endl;
				}
			}

		}

		__ExitServer ExitServer = (__ExitServer)GetProcAddress(hDll, "ExitServer");
		int ExitServerReturn = ExitServer(sServer, sClient);
		if (ExitServerReturn == 0)
		{
			std::cout << "�˳��ɹ���" << std::endl;
		}
	}
	else
	{
		return 0;
	}
	FreeLibrary(hDll);
	hDll = NULL;

	system("pause");
	return 0;
}