#include "RequestFactory.h"

RequestFactory::RequestFactory(const std::string& encStr)
{
	m_flag = true;
	m_encStr = encStr;
}

RequestFactory::RequestFactory(RequestInfo* info)
{
	m_flag = false;
	m_info = info;
}

Codec* RequestFactory::createCodec()
{
	if (m_flag)
	{
		return new Request(m_encStr);
	}
	else
	{
		return new Request(m_info);
	}
}

RequestFactory::~RequestFactory()
{

}
