#include "TcpServer.h"
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

TcpServer::TcpServer()
{

}

TcpServer::~TcpServer()
{

}

int TcpServer::setListen(unsigned short port)
{
	int ret = 0;
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	m_lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_lfd == -1)
	{
		ret = errno;
		return ret;
	}

	//设置端口复用
	int on = 1;
	ret = setsockopt(m_lfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	ret = bind(m_lfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if (ret == -1)
	{
		ret = errno;
		return ret;
	}

	return ret;
}

TcpSocket* TcpServer::acceptConn(int wait_sec)
{
	int ret;
	if (wait_sec > 0)
	{
		fd_set accept_fdset;
		struct timeval timeout;
		FD_ZERO(&accept_fdset);
		FD_SET(m_lfd, &accept_fdset);
		timeout.tv_sec = wait_sec;
		timeout.tv_usec = 0;
		do {
			ret = select(m_lfd + 1, &accept_fdset, NULL, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);
		if (ret <= 0)
		{
			return NULL;
		}
	}
	//调用accept不会阻塞
	struct sockaddr_in addrCli;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	int connfd = accept(m_lfd, (struct sockaddr*)&addrCli, &addrlen);
	if (connfd == -1)
	{
		return NULL;
	}

	return new TcpSocket(connfd);
}

void TcpServer::closefd()
{
	close(m_lfd);
}
