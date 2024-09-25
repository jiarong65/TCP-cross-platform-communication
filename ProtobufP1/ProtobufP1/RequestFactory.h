#pragma once
#include "CodecFactory.h"
#include"Codec.h"
#include"Request.h"

class RequestFactory :public CodecFactory
{
public:
	RequestFactory(const std::string& encStr);
	RequestFactory(RequestInfo* info);
	~RequestFactory();
	Codec* createCodec()override;
private:
	bool m_flag;
	std::string m_encStr;
	RequestInfo* m_info;
};

