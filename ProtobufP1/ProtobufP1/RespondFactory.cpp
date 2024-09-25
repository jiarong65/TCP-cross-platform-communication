#include "RespondFactory.h"

RespondFactory::RespondFactory(const std::string& encStr)
{
	m_flag = true;
	m_encStr = encStr;
}

RespondFactory::RespondFactory(RespondInfo* info)
{
	m_flag = false;
	m_info = info;
}

Codec* RespondFactory::createCodec()
{
	if (m_flag)
	{
		return new Respond(m_encStr);
	}
	else
	{
		return new Respond(m_info);
	}
}

RespondFactory::~RespondFactory()
{

}