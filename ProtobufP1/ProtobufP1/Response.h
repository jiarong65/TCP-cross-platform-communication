#pragma once
#include "Codec.h"
#include"Message.pb.h"
#include<iostream>
using namespace std;

struct RespondInfo
{
	int status;
	int seckeyID;
	string clientID;
	string serverID;
	string data;
};

class Respond :public Codec
{
public:
	Respond();
	//string用于解码
	Respond(const string& encstr);
	Respond(RespondInfo* info);

	//用于构造
	void initMessage(const string& encstr);
	void initMessage(const RespondInfo* info);

	//序列化
	string encodeMsg();
	//反序列化
	void* decodeMsg();
	~Respond();
private:
	string m_encStr;
	RespondMsg m_msg;
};
