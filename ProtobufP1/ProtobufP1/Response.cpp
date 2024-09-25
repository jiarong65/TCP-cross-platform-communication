#include "Response.h"
#include"Message.pb.h"

Respond::Respond()
{

}

Respond::Respond(const string& encstr)
{
	initMessage(encstr);
}

Respond::Respond(RespondInfo* info)
{
	initMessage(info);
}

void Respond::initMessage(const string& encstr)
{
	m_encStr = encstr;
}

void Respond::initMessage(const RespondInfo* info)
{
	m_msg.set_status(info->status);
	m_msg.set_seckeyid(info->seckeyID);
	m_msg.set_clientid(info->clientID);
	m_msg.set_serverid(info->serverID);
	m_msg.set_data(info->data);
}

std::string Respond::encodeMsg()
{
	string output;
	m_msg.SerializeToString(&output);
	return output;
}

void* Respond::decodeMsg()
{
	m_msg.ParseFromString(m_encStr);
	return &m_msg;
}

Respond::~Respond()
{

}
