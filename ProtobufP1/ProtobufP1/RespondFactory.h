#pragma once
#include "CodecFactory.h"
#include"Codec.h"
#include"Response.h"

class RespondFactory :public CodecFactory
{
public:
	RespondFactory(const std::string& encStr);
	RespondFactory(RespondInfo* info);
	~RespondFactory();
	Codec* createCodec()override;
private:
	bool m_flag;
	std::string m_encStr;
	RespondInfo* m_info;
};

