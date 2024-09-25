#pragma once
#include"Message.pb.h"
#include "Codec.h"
#include<iostream>
using namespace std;

struct RequestInfo
{
	int cmd;
	string clientID;
	string serverID;
	string sign;
	string data;
};

class Request :public Codec
{
public:
	Request();
	Request(const string& encstr);
	Request(RequestInfo* info);
	void initMessage(const string& encstr);
	void initMessage(const RequestInfo* info);
	string encodeMsg();
	void* decodeMsg();
	~Request();
private:
	string m_encStr;
	RequestMsg m_msg;
};

