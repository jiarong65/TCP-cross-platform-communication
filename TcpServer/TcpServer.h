#pragma once
#include"TcpSocket.h"

class TcpServer
{
public:
	TcpServer();
	~TcpServer();

	//服务器监听设置
	int setListen(unsigned short port);
	//等待并接受客户端连接请求，默认超时时间为10000s
	TcpSocket* acceptConn(int wait_sec = 10000);
	void closefd();
private:
	int m_lfd;
};

