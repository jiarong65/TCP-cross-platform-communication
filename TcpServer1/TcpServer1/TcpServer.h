#pragma once
#include"TcpSocket.h"

class TcpServer
{
public:
	TcpServer();
	~TcpServer();

	//��������������
	int setListen(unsigned short port);
	//�ȴ������ܿͻ�����������Ĭ�ϳ�ʱʱ��Ϊ10000s
	TcpSocket* acceptConn(int wait_sec = 10000);
	void closefd();
private:
	int m_lfd;
};

