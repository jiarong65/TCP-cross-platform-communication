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
	//string���ڽ���
	Respond(const string& encstr);
	Respond(RespondInfo* info);

	//���ڹ���
	void initMessage(const string& encstr);
	void initMessage(const RespondInfo* info);

	//���л�
	string encodeMsg();
	//�����л�
	void* decodeMsg();
	~Respond();
private:
	string m_encStr;
	RespondMsg m_msg;
};
