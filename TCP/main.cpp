#include <iostream>
#include <thread>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
//using namespace std;
void initialization();

typedef struct {
	unsigned char 	Head; 						//֡ͷ  			ֵ����ֵ����
	unsigned short	packetID;					//��ϢID			ֵ����ֵ����
	unsigned char 	frameID;					//֡��				(0x00 ~ 0xFF)
	short 			sw_angle; 					//�����̽Ƕ�		(-8000 ~ 8000) ��λ0.1��
	unsigned char	speed;						//����				��λkm/h
	unsigned char 	xor_verify;  				//���У��			(�Խṹ�����г���֡ͷ֡β����ĳ�Ա�������У��)
	unsigned char	Tail;						//֡β				ֵ����ֵ����
}STEERING_WHEEL_UDP;

int main() {
	//���峤�ȱ���
	int send_len = 0;
	int recv_len = 0;
	int len = 0;
	//���巢�ͻ������ͽ��ܻ�����
	char send_buf[100];
	char recv_buf[sizeof(STEERING_WHEEL_UDP)];
	//���������׽��֣����������׽���
	SOCKET s_server;
	SOCKET s_accept;
	//����˵�ַ�ͻ��˵�ַ
	SOCKADDR_IN server_addr;
	SOCKADDR_IN accept_addr;
	initialization();
	//���������Ϣ
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(5030);
	//�����׽���
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(s_server, (SOCKADDR *)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		std::cout << "�׽��ְ�ʧ�ܣ�" << std::endl;
		WSACleanup();
	}
	else {
		std::cout << "�׽��ְ󶨳ɹ���" << std::endl;
	}
	//�����׽���Ϊ����״̬
	if (listen(s_server, SOMAXCONN) < 0) {
		std::cout << "���ü���״̬ʧ�ܣ�" << std::endl;
		WSACleanup();
	}
	else {
		std::cout << "���ü���״̬�ɹ���" << std::endl;
	}
	std::cout << "��������ڼ������ӣ����Ժ�...." << std::endl;
	//������������
	len = sizeof(SOCKADDR);
	s_accept = accept(s_server, (SOCKADDR *)&accept_addr, &len);
	if (s_accept == SOCKET_ERROR) {
		std::cout << "����ʧ�ܣ�" << std::endl;
		WSACleanup();
		return 0;
	}
	std::cout << "���ӽ�����׼����������" << std::endl;


	//setsockopt(,);
	//��������
	while (1) {
		recv_len = recv(s_server, recv_buf, sizeof(STEERING_WHEEL_UDP), 0);
		if (recv_len < 0) {
			//std::cout << "����ʧ�ܣ�" << std::endl;
			//break;
		}
		else {
			//int *p = &recv_buf;
			std::cout << "�ͻ�����Ϣ:" << (int)recv_buf << std::endl;
		}
		/*std::cout << "������ظ���Ϣ:";
		std::cin >> send_buf;
		send_len = send(s_accept, send_buf, sizeof(send_buf), 0);
		if (send_len < 0) {
			std::cout << "����ʧ�ܣ�" << std::endl;
			break;
		}*/
	}
	//�ر��׽���
	closesocket(s_server);
	closesocket(s_accept);
	//�ͷ�DLL��Դ
	WSACleanup();
	return 0;
}
void initialization() {
	//��ʼ���׽��ֿ�
	WORD w_req = MAKEWORD(2, 2);//�汾��
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		std::cout << "��ʼ���׽��ֿ�ʧ�ܣ�" << std::endl;
	}
	else {
		std::cout << "��ʼ���׽��ֿ�ɹ���" << std::endl;
	}
	//���汾��
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		std::cout << "�׽��ֿ�汾�Ų�����" << std::endl;
		WSACleanup();
	}
	else {
		std::cout << "�׽��ֿ�汾��ȷ��" << std::endl;
	}
	//������˵�ַ��Ϣ

}
