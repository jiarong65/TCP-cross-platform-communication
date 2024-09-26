#include<cstdio>
#include<iostream>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include"TcpServer.h"
using namespace std;

void* callback(void* arg)
{
	TcpSocket* sockTcp = (TcpSocket*)arg;
	while (1)
	{
		string msg = sockTcp->recvMsg();
		if (msg.empty())
		{
			break;
		}
		cout << "recvMsg:" << msg << endl;

		string sendMsg = "hello,client...";
		sockTcp->sendMsg(sendMsg);
		//sockTcp->disConnect();
	}
}

int main()
{
	TcpServer* server = new TcpServer;
	server->setListen(9999);

	//等待并接受连接请求
	while (1)
	{
		cout << "开始accept..." << endl;
		TcpSocket* sockTcp = server->acceptConn();
		cout << "接受了一个客户端连接请求..." << endl;
		if (sockTcp)
		{
			pthread_t tid;
			pthread_create(&tid, NULL, callback, sockTcp);
			pthread_detach(tid);
		}
	}
	return 0;
}