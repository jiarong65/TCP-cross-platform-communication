#include "Request.h"
#include"Message.pb.h"

Request::Request()
{

}

Request::Request(const string& encstr)
{
	initMessage(encstr);
}

Request::Request(RequestInfo* info)
{
	initMessage(info);
}

void Request::initMessage(const string& encstr)
{
	m_encStr = encstr;
}

void Request::initMessage(const RequestInfo* info)
{
	m_msg.set_cmdtype(info->cmd);
	m_msg.set_clientid(info->clientID);
	m_msg.set_serverid(info->serverID);
	m_msg.set_sign(info->sign);
	m_msg.set_data(info->data);
}

string Request::encodeMsg()
{
	string output;
	m_msg.SerializeToString(&output);
	return output;
}

void* Request::decodeMsg()
{
	m_msg.ParseFromString(m_encStr);
	return &m_msg;
}

Request::~Request()
{

}

