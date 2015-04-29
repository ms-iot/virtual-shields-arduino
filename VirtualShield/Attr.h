
#ifndef Attr_h
#define Attr_h

#include "SensorModels.h"

struct Attr : public EPtr
{
public:
	Attr(const char* key, const char* value) : EPtr(MemPtr, key, value)	{}
	Attr(const char* key, String value) : EPtr(MemPtr, key, value.c_str()) {}
	Attr(const char* key, int value) : EPtr(key, value) {}
	Attr(const char* key, double value) : EPtr(key, value) {}
	Attr(const char* key, long value) : EPtr(key, value) {}
	Attr(const char* key, bool value) : EPtr(key, value) {}
};

#endif