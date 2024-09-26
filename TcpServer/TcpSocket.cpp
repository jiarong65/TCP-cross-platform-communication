#include "TcpSocket.h"
#include<arpa/inet.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>


TcpSocket::TcpSocket()
{

}

TcpSocket::TcpSocket(int fd)
{
	m_socket = fd;
}

TcpSocket::~TcpSocket()
{
	
}

int TcpSocket::connectToHost(const string& ip, unsigned short port, int timeout)
{
	int ret = 0;
	if (port <= 0 || port > 65535 || timeout < 0)
	{
		ret = ParamError;
		return ret;
	}

	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket < 0)
	{
		ret = errno;
		printf("func socket() err: %d\n", ret);
		return ret;
	}

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = inet_addr(ip.data());

	ret = connectTimeout((struct sockaddr_in*)(&servaddr),(unsigned int) timeout);
	if (ret < 0)
	{
		//超时
		if (ret == -1 && errno == ETIMEDOUT)
		{
			ret = TimeoutError;
			return ret;
		}
		else
		{
			printf("connectTimeout 调用异常，错误号：%d\n", errno);
			return errno;
		}
	}

	return ret;
}

void TcpSocket::disConnect()
{
	if (m_socket >= 0)
	{
		close(m_socket);
	}
}

int TcpSocket::sendMsg(string sendData, int timeout)
{
	int ret = writeTimeout(timeout);
	if (ret == 0)
	{
		int writed = 0;
		int dataLen = sendData.size() + 4;
		//添加4字节作为数据头，存储数据块长度
		unsigned char* netdata = (unsigned char*)malloc(dataLen);
		if (netdata == 0)
		{
			ret = MallocError;
			printf("func sckClient_send() malloc Err:%d\n", ret);
			return ret;
		}
		//转换为网络字节序
		int netlen = htonl(sendData.size());
		memcpy(netdata, &netlen, 4);
		memcpy(netdata + 4, sendData.data(), sendData.size());

		writed = writen(netdata,dataLen);
		if (writed < dataLen)
		{
			if (netdata != NULL)
			{
				free(netdata);
				netdata = NULL;
			}
			return writed;
		}
		//释放内存
		if (netdata != NULL)
		{
			free(netdata);
			netdata = NULL;
		}
	}
	else
	{
		//失败返回-1,超时返回-1并且errno=ETIMEDOUT
		if (ret == -1 && errno == ETIMEDOUT)
		{
			ret = TimeoutError;
			printf("func sckClient_send() mlloc Err:%d\n", ret);
		}
	}

	return ret;
}

string TcpSocket::recvMsg(int timeout)
{
	int ret = readTimeout(timeout);
	if (ret != 0)
	{
		if (ret == -1 || errno == ETIMEDOUT)
		{
			printf("readTimeout err:TimeoutError \n");
			return string();
		}
		else
		{
			printf("readTimeout(timeout) err:%d \n", ret);
			return string();
		}
	}

	int netdatalen = 0;
	ret = readn(&netdatalen, 4);//读包头，4个字节
	if (ret == -1)
	{
		printf("func readn() err:%d\n", ret);
		return string();
	}
	else if (ret < 4)
	{
		printf("func readn() err peer closed:%d \n", ret);
		return string();
	}

	int n = ntohl(netdatalen);
	char* tmpBuf = (char*)malloc(n + 1);
	if (tmpBuf == NULL)
	{
		ret = MallocError;
		printf("malloc() err \n");
		return NULL;
	}

	ret = readn(tmpBuf, n);
	if (ret == -1)
	{
		printf("func readn() err:%d\n", ret);
		return string();
	}
	else if (ret < n)
	{
		printf("func readn() err peer close:%d\n", ret);
		return string();
	}

	tmpBuf[n] = '\0';//多分配一个字节内容，兼容可见字符串 字符串的真实长度仍然为n
	string data = string(tmpBuf);
	//释放内存
	free(tmpBuf);

	return data;
}

//设置为非阻塞
int TcpSocket::setNoBlock(int fd)
{
	int flag = fcntl(fd, F_GETFL);
	if (flag == -1)
	{
		return flag;
	}
	flag |= O_NONBLOCK;
	int ret = fcntl(fd, F_SETFL, flag);
	return ret;
}

//设置为阻塞
int TcpSocket::setBlock(int fd)
{
	int flag = fcntl(fd, F_GETFL);
	if (flag == -1)
	{
		return flag;
	}
	flag &= ~O_NONBLOCK;
	int ret = fcntl(fd, F_SETFL, flag);
	return ret;
}

/*
* 读超时检测函数，不含读操作
* 成功返回0，失败返回-1并且errno = ETIMEDOUT
*/
int TcpSocket::readTimeout(unsigned int wait_sec)
{
	int ret = 0;
	if (wait_sec > 0)
	{
		fd_set read_fdset;
		FD_ZERO(&read_fdset);
		FD_SET(m_socket,&read_fdset);

		struct timeval timeout;
		timeout.tv_sec = wait_sec;
		timeout.tv_usec = 0;

		do {
			ret = select(m_socket + 1, &read_fdset, NULL, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);

		if (ret == 0)
		{
			ret = -1;
			errno = ETIMEDOUT;
		}
		else if(ret==1){
			ret = 0;
		}
	}

	return ret;
}

//成功返回0，失败返回-1，超时返回-1且errno=ETIMEDOUT
int TcpSocket::writeTimeout(unsigned int wait_sec)
{
	int ret = 0;
	if (wait_sec > 0)
	{
		fd_set write_fdset;
		struct timeval timeout;

		FD_ZERO(&write_fdset);
		FD_SET(m_socket, &write_fdset);
		timeout.tv_sec = wait_sec;
		timeout.tv_usec = 0;

		do {
			ret = select(m_socket+1, NULL, &write_fdset, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);
		if (ret == 0)
		{
			ret = -1;
			errno = ETIMEDOUT;
		}
		else if (ret == 1)
		{
			ret = 0;
		}
	}

	return ret;
}

//成功返回0，失败返回-1，超时返回-1且errno=ETIMEDOUT
int TcpSocket::connectTimeout(sockaddr_in* addr, unsigned int wait_sec)
{
	int ret;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	if (wait_sec > 0)
	{
		setNoBlock(m_socket);//设置connect为非阻塞
	}

	ret = connect(m_socket, (struct sockaddr*)&addr, addrlen);
	// 非阻塞模式连接, 返回-1, 并且errno为EINPROGRESS, 表示连接正在进行中
	if (ret < 0 && errno == EINPROGRESS) {
		fd_set connect_fdset;
		struct timeval timeout;

		FD_ZERO(&connect_fdset);
		FD_SET(m_socket, &connect_fdset);
		timeout.tv_sec = wait_sec;
		timeout.tv_usec = 0;

		do {
			ret = select(m_socket + 1, NULL, &connect_fdset, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);
		if (ret == 0)
		{
			//超时
			ret == -1;
			errno = ETIMEDOUT;
		}
		else if (ret < 0)
		{
			return -1;
		}
		else if (ret == 1)
		{
			/* ret返回为1（表示套接字可写），可能有两种情况，一种是连接建立成功，一种是套接字产生错误，*/
			/* 此时错误信息不会保存至errno变量中，因此，需要调用getsockopt来获取。 */
			int err;
			socklen_t sockLen = sizeof(err);
			int sockoptret = getsockopt(m_socket, SOL_SOCKET, SO_ERROR, &err, &sockLen);
			if (sockoptret == -1)
			{
				return -1;
			}
			if (err == 0)
			{
				ret = 0;//成功建立连接
			}
			else {
				//连接失败
				errno = err;
				ret = -1;
			}
		}
	}
	if (wait_sec > 0)
	{
		setBlock(m_socket);
	}
	return ret;
}

//成功返回count，失败返回-1，读到EOF返回<count
int TcpSocket::readn(void* buf, int count)
{
	size_t nleft = count;
	ssize_t nread;
	char* bufp = (char*)buf;

	while (nleft > 0)
	{
		if ((nread = read(m_socket, bufp, nleft))<0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			return -1;
		}
		else if (nread == 0)
		{
			return count - nleft;
		}

		bufp += nread;
		nleft -= nread;
	}

	return count;
}

//成功返回count,失败返回-1
int TcpSocket::writen(const void* buf, int count)
{
	size_t nleft = count;
	ssize_t nwritten;
	char* bufp = (char*)buf;

	while (nleft > 0)
	{
		if ((nwritten = write(m_socket, bufp, nleft)) < 0)
		{
			if (errno == EINTR)//被信号打断
			{
				continue;
			}
			return -1;
		}
		else if(nwritten==0)
		{
			continue;.
		}

		bufp += nwritten;
		nleft -= nwritten;
	}

	return count;
}
