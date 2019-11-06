#include <WinSock2.h>
#include <iostream>
#include <thread>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

typedef struct {
	unsigned char 	Head; 						//֡ͷ  			ֵ����ֵ����
	unsigned short	packetID;					//��ϢID			ֵ����ֵ����
	unsigned char 	frameID;					//֡��				(0x00 ~ 0xFF)
	short 			sw_angle; 					//�����̽Ƕ�		(-8000 ~ 8000) ��λ0.1��
	unsigned char	speed;						//����				��λkm/h
	unsigned char 	xor_verify;  				//���У��			(�Խṹ�����г���֡ͷ֡β����ĳ�Ա�������У��)
	unsigned char	Tail;						//֡β				ֵ����ֵ����
}STEERING_WHEEL_UDP;

const int BUF_SIZE = sizeof(STEERING_WHEEL_UDP);
WSADATA         wsd;            //WSADATA����  
SOCKET          sHost;          //�������׽���  
SOCKADDR_IN     servAddr;       //��������ַ  
char            buf[BUF_SIZE];  //�������ݻ�����  
char            bufRecv[BUF_SIZE];
int             retVal;         //����ֵ  



//BOOL RecvLine(SOCKET s, char* buf);  //��ȡһ������  
int g_nRevThread_flag = 0;
int g_nrevThread_run_flag = 0;


/********************���������߳�************************************/
void isConnect();
int out_time = 0;										//������ʱ������


int main(int argc, char* argv[])
{

									//��ʼ���׽��ֶ�̬��  
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout  << "WSAStartup failed!" << endl;
		return  - 1;
	}

	//�����׽���  
	sHost  = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET  == sHost)
	{
		cout  << "socket failed!" << endl;
		WSACleanup();//�ͷ��׽�����Դ  
		return   - 1;
	}

	//���÷�������ַ  
	servAddr.sin_family  = AF_INET;
	servAddr.sin_addr.s_addr  = inet_addr("192.168.1.171");
	servAddr.sin_port  = htons((short)6000);
	int nServAddlen   = sizeof(servAddr);

	//���ӷ�����  
	retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	
	if (SOCKET_ERROR  == retVal)
	{
		cout  << "connect failed!" << endl;
		//closesocket(sHost); //�ر��׽���  
		//WSACleanup();       //�ͷ��׽�����Դ  
		//return  - 1;
		g_nRevThread_flag = 0;
	}
	
	/********��ʼ���Զ������߳�***************/
	//std::thread p(isConnect, &sHost, &servAddr);
	
	while (true) {
		//���������������  
		//ZeroMemory(buf, BUF_SIZE);
		/*cout  << " ���������������:  ";
		cin  >> buf;*/
		//retVal  = send(sHost, buf, strlen(buf), 0);
		//if (SOCKET_ERROR  == retVal)
		//{
		//	cout  << "send failed!" << endl;
		//	closesocket(sHost); //�ر��׽���  
		//	//WSACleanup();       //�ͷ��׽�����Դ  
		//	//return  - 1;
		//}
		//RecvLine(sHost, bufRecv);
		char cace_buf[1024] = {0};
		retVal = recv(sHost, cace_buf, 1024,0);
		if (retVal>0) {
		   // ���շ������˵����ݣ� ֻ����5���ַ�  
			cout  << endl  << "�ӷ������������ݣ�" << cace_buf;
			out_time = 0;
		}
		else {
			if (g_nrevThread_run_flag ==0&& out_time<60) {
				std::cout << "�ж��̣߳�g_nrevThread_run_flag:" << g_nrevThread_run_flag << endl;
				std::thread t_restart_connect(isConnect);
				t_restart_connect.detach();
			}
			/*****************��ʱ������δ��Ӧ�Զ���������*********************/
			if (out_time > 70) {
					closesocket(sHost); //�ر��׽���  
					WSACleanup();       //�ͷ��׽�����Դ  
					std::cout << "���������̣߳�����2���Զ���������" <<endl;
					Sleep(2000);
					return -1;
			}
			//g_nRevThread_flag = 1;
			///****************
			//g_nRevThread_flag: 1��ʾ��Ҫ��������   0��ʾ����Ҫ�������������ӳɹ����߳��ֲ����Զ��ָ������⣩
			//*************************/
			//if (g_nRevThread_flag) {
			//	/****************�����߳��Ƿ��Ѿ�����***
			//	g_nrevThread_run_flag 1��ʾ���߳��Ѿ����� 0��ʾ���߳��Ѿ�����
			//	**************/
			//
			//	if (!t_restart_connect.joinable()) {
			//		std::cout << "����������";
			//		t_restart_connect.detach();
			//		g_nrevThread_run_flag =0;
			//	}
			//	
			//}
			//else {
			//	if(g_nrevThread_run_flag)
			//		t_restart_connect.~thread();
			//	g_nrevThread_run_flag = 1;
			//	closesocket(sHost); //�ر��׽���  
			//	WSACleanup();
			//	cout << "����ʧ�ܣ�"<<endl;
			//	}
		}
		

	}
	cout << "end main!";
	//�˳�  
	//t_restart_connect.~thread();
	closesocket(sHost); //�ر��׽���  
	WSACleanup();       //�ͷ��׽�����Դ  
	system("pause");
	return 0;
}
//

void isConnect() {
	g_nrevThread_run_flag = 1;
	for (USHORT i = 1; i < 0x1000; i = i << 1) {
		out_time = i;
		if (sHost != NULL)
			closesocket(sHost); //�ر��׽���
		sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == sHost)
		{
			cout << "socket failed!" << endl;
			//WSACleanup();//�ͷ��׽�����Դ  
			//return  -1;
			std::cout << "������������" << i << "s��ʼ��" << endl;
			Sleep(i*1000);
			
			continue;
		}
		int retVal;
		retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
		if (SOCKET_ERROR == retVal)
		{
			cout << "connect failed!" << endl;
			if (sHost != NULL)
				closesocket(sHost); //�ر��׽���  
									//WSACleanup();       //�ͷ��׽�����Դ  
									//return  -1;
			std::cout << "������������" << i << "s��ʼ��" << endl;
			Sleep(i * 1000);
		
			continue;
		}
		/***********���ӳɹ������ñ�־λ,���ٳ�������***********

		**************/
		g_nRevThread_flag = 0;
		g_nrevThread_run_flag = 0;
		std::cout << "�뿪�����̣߳�g_nrevThread_run_flag:"<< g_nrevThread_run_flag << endl;
		return;
	}
	std::cout << "�뿪�����̣߳�g_nrevThread_run_flag:" << g_nrevThread_run_flag << endl;
	g_nRevThread_flag = 0;
	g_nrevThread_run_flag = 0;
}