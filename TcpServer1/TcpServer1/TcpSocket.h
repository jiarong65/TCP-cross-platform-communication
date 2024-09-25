#pragma once
#include<iostream>
using namespace std;

static const int TIMEOUT = 10000;

class TcpSocket
{
public:
	enum ErrorType { ParamError = 3001, TimeoutError, PeerCloseError, MallocError };
	
	TcpSocket();
	TcpSocket(int fd);
	~TcpSocket();

	//���ӵ�������
	int connectToHost(const string& ip, unsigned short port, int timeout = TIMEOUT);
	int sendMsg(string sendData,int time = TIMEOUT);
	int recvMsg(int time = TIMEOUT);
	void disConnect();

private:
	int setNoBlock(int fd);//����I/OΪ������
	int setBlock(int fd); // ����I / OΪ����
	int readTimeout(unsigned int wait_sec);//����ʱ
	int writeTimeout(unsigned int wait_sec);//д��ʱ
	int connectTimeout(struct sockaddr_in* addr, unsigned int wait_sec);//���ӳ�ʱ
	int readn(void* buf, int count);//��������n�ַ�
	int writen(const void* buf, int count);//������дn�ַ�

	int m_socket;//����ͨ�ŵ��׽���
};

