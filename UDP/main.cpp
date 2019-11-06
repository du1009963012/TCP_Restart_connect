#include<WinSock2.h>
#include <iostream>
#include <stdio.h>
#include <thread>
#include <queue>
#pragma comment(lib,"ws2_32.lib")
typedef struct {
	unsigned char 	Head; 						//֡ͷ  			ֵ����ֵ����
	unsigned short	packetID;					//��ϢID			ֵ����ֵ����
	unsigned char 	frameID;					//֡��				(0x00 ~ 0xFF)
	short 			sw_angle; 					//�����̽Ƕ�		(-8000 ~ 8000) ��λ0.1��
	unsigned char	speed;						//����				��λkm/h
	unsigned char 	xor_verify;  				//���У��			(�Խṹ�����г���֡ͷ֡β����ĳ�Ա�������У��)
	unsigned char	Tail;						//֡β				ֵ����ֵ����
}STEERING_WHEEL_UDP;
std::queue<STEERING_WHEEL_UDP> queue_data;

void data_revice() {
	char *cech_data = new char[sizeof(STEERING_WHEEL_UDP)];
	std::cout << "thread start!";
	WSADATA wsd;    // ��ʼ��Socket�ı���
	SOCKET s;        // ����ͨ�ŵ�Socket���
	SOCKADDR_IN sRecvAddr, sSendAddr;    // �ֱ�Ϊ���յ�ַ�ͷ��͵�ַ
	USHORT uPort = 1401;                // ͨ�Ŷ˿�
    char szBuf[sizeof(STEERING_WHEEL_UDP)] = { 0 };            // ͨ�����ݻ�����
   int nBufLen = 1024, nResult = 0, nSenderAddrSize = sizeof(sSendAddr);

   // ��ʼ��Socket2.2�汾
    nResult = WSAStartup(MAKEWORD(2, 2), &wsd);
    if (nResult != NO_ERROR)
   {
       printf("WSAStartup failed:%d\n", WSAGetLastError());
        return ;
    }

		     // ����һ��Socket,SOCK_DGRAM��ʾUDP����
		     s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == INVALID_SOCKET)
   {
       printf("socket failed:%d\n", WSAGetLastError());
       return ;
    }

  // ���Socket�ӿ�
	sRecvAddr.sin_family = AF_INET;        // ��ַЭ��,AF_INET֧��TCP��UDP
	sRecvAddr.sin_port = htons(uPort);    // ͨ�Ŷ˿�,htonsתΪ�����ֽ�˳��
	sRecvAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);    // ���������ַ����

    // ��Socket������
    nResult = bind(s, (SOCKADDR *)&sRecvAddr, sizeof(sRecvAddr));
    if (nResult != 0)
   {
     printf("bind failed:%d\n", WSAGetLastError());
      return ;
   }

   printf("Waiting recv data...\n");
   // ����ʽ��������
   while (1) {
	   //std::cout << "thread 1!";

	   int len = recvfrom(s, szBuf, sizeof(STEERING_WHEEL_UDP), 0, (SOCKADDR *)&sSendAddr, &nSenderAddrSize);
	   //std::cout << "thread 2!";
	   if (len > 0) {
		  // std::cout <<"thread data !"<< " ";
		  // std::cout << szBuf[0] << " ";
		   if ((szBuf[0] == (char)0xca) && (szBuf[8] ==(char)0xac)) {
			  // std::cout << " data jiexi!" << " ";
			   STEERING_WHEEL_UDP data;
			   data.Head = 0xCA;
			   data.packetID = (0xFFFF & (szBuf[1] << 8))& szBuf[2];
			   data.frameID = szBuf[3];
			   data.sw_angle = (0xFFFF & (szBuf[4] << 8))& szBuf[5];
			   data.speed = szBuf[6];
			   data.xor_verify = szBuf[7];
			   data.Tail = 0xAC;
			   //if()
			  // std::cout << std::hex << szBuf[6];
			   queue_data.push(data);
		   }

	   }
   }
  // nResult = recvfrom(s, szBuf, nBufLen, 0, (SOCKADDR *)&sSendAddr, &nSenderAddrSize);
   if (nResult == SOCKET_ERROR)
   {
	   printf("recvfrom failed:%d\n", WSAGetLastError());
   }else {
      printf("SenderIP  :%s\n", inet_ntoa(sSendAddr.sin_addr));
      printf("SenderData:%s\n", szBuf);

	}
    // �ر�Socket����
   nResult = closesocket(s);
    if (nResult == SOCKET_ERROR)
  {
       printf("closesocket failed:%d\n", WSAGetLastError());
        return ;
   }
    // ����Socket
    WSACleanup();

   // system("pause");
	    return ;
	
}
int main(int argc, char* argv[])
{
	//STEERING_WHEEL_UDP date;
	//data_test.Head =0 ;
	WSADATA WSAData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &WSAData) != 0)
		return 0;
	std::thread t_fun(data_revice);
	t_fun.detach();
	while (1) {
		while (!queue_data.empty())
		{
			//std::cout << "main Thread data!";
			std::cout <<queue_data.front().speed<<"\n";
			//std::cout << "pop data!";
			queue_data.pop();
		}
	};
	//
	//char buff[1024];	//�������ջ����ֽ����� 
	//while (true)
	//{
	//	memset(buff, 0, 1024);	//��ս��ջ�������
	//							//��ʼ�������� 
	//	int len = recvfrom(serSocket, buff, 1024, 0, (sockaddr*)&clientAddr, &iAddrlen);
	//	if (len>0)
	//	{
	//		cout << "�ͻ��˵�ַ��" << inet_ntoa(clientAddr.sin_addr) << endl;
	//		cout << buff;

	//		//	sendto(serSocket,buff,1024,0,(sockaddr*)&clientAddr,iAddrlen);
	//	}
	//}

	
	WSACleanup();
	

	return 0;
}

