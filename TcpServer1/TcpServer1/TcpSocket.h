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

	//连接到服务器
	int connectToHost(const string& ip, unsigned short port, int timeout = TIMEOUT);
	int sendMsg(string sendData,int time = TIMEOUT);
	int recvMsg(int time = TIMEOUT);
	void disConnect();

private:
	int setNoBlock(int fd);//设置I/O为非阻塞
	int setBlock(int fd); // 设置I / O为阻塞
	int readTimeout(unsigned int wait_sec);//读超时
	int writeTimeout(unsigned int wait_sec);//写超时
	int connectTimeout(struct sockaddr_in* addr, unsigned int wait_sec);//连接超时
	int readn(void* buf, int count);//缓存区读n字符
	int writen(const void* buf, int count);//缓存区写n字符

	int m_socket;//用于通信的套接字
};

