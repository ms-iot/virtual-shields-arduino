#ifndef ShieldEvent_h
#define ShieldEvent_h

#include "Arduino.h"

enum ShieldEventType {
	UnknownShieldEventType = 0,
	SensorShieldEventType = 1
};

struct ShieldEvent {
	ShieldEventType	shieldEventType;
	int id;
	long resultId;
	const char *result;
	int resultHash;
	const char* tag;
	const char* action;
	int actionHash;
	void* cargo;
	double value;
};

#endif 