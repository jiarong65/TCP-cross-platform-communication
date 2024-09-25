#pragma once
#include"Codec.h";

class CodecFactory
{
public:
	CodecFactory();
	virtual ~CodecFactory();
protected:
	virtual Codec* createCodec()=0;
};

