/*
    Copyright(c) Microsoft Open Technologies, Inc. All rights reserved.

    The MIT License(MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files(the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions :

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef VirtualShield_h
#define VirtualShield_h

#include "Arduino.h"

#include <ArduinoJson.h>

#include "Sensor.h"
#include "ShieldEvent.h"
#include "Attr.h"

typedef unsigned int UINT;

class Sensor;
struct SensorEvent;

const long DEFAULT_BAUDRATE = 115200;
const long WAITFOR_TIMEOUT = 30000;

#define REFRESH_HASH 0xC5BF
#define CONNECT_HASH 0xA91A
#define PING_HASH 0x2CFE
#define SUSPEND_HASH 0xC15E
#define RESUME_HASH 0x3549

class VirtualShield
{
public:
	void(*onEvent)(ShieldEvent*) = 0;
	void(*onConnect)(ShieldEvent*) = 0;
	void(*onRefresh)(ShieldEvent*) = 0;
	void(*onSuspend)(ShieldEvent*) = 0;
	void(*onResume)(ShieldEvent*) = 0;

    VirtualShield();

	void begin(long bitRate = DEFAULT_BAUDRATE);
	void setPort(int port);

	bool checkSensors(int watchForId = 0, long timeout = 0, int waitForResultId = -1);
    int waitFor(int id, long timeout = WAITFOR_TIMEOUT, bool asSuccess = true, int resultId = -1);
	bool hasError(ShieldEvent* shieldEvent = 0);

	bool getEvent(ShieldEvent* shieldEvent);

	int directToSerial(const char* cmd);

	bool addSensor(Sensor* sensor);

	virtual void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent);

	void write(const char* text);
	int writeAll(const char* serviceName, EPtr values[], int count, Attr extraAttributes[] = 0, int extraAttributeCount = 0, const char sensorType = '\0');

	int writeAll(const char* serviceName);

	int beginWrite(const char* serviceName) ;
	int write(EPtr eptr) const;

	int endWrite();

    int block(int id, bool blocking, long timeout = WAITFOR_TIMEOUT, int waitForResultId = -1);

	void setOnEvent(void(*onEvent)(ShieldEvent*))
	{
		this->onEvent = onEvent;
	}

	void setOnConnect(void(*onConnect)(ShieldEvent*))
	{
		this->onConnect = onConnect;
	}

	void setOnRefresh(void(*onRefresh)(ShieldEvent*))
	{
		this->onRefresh = onRefresh;
	}

	void setOnSuspend(void(*onSuspend)(ShieldEvent*))
	{
		this->onSuspend = onSuspend;
	}

	void setOnResume(void(*onResume)(ShieldEvent*))
	{
		this->onResume = onResume;
	}

	/// <summary>
	/// Enables or disables block() to block for specific id-based responses.
	/// </summary>
	void enableAutoBlocking(bool enable) {
		this->allowAutoBlocking = enable; 
	}

	int parseToHash(const char* text, unsigned int *hash, int hashCount, char separator = ' ', unsigned int length = -1);
	static unsigned int hash(const char* s, unsigned int len = -1, unsigned int seed = 0);

protected:
	int sendFlashStringOnSerial(const char* flashStringAdr, int start = -1, bool encode = false) const;

	void onJsonStringReceived(char* json, ShieldEvent* shieldEvent);
	void onStringReceived(char* buffer, int length, ShieldEvent* shieldEvent);

	void flush();

    Stream* _VShieldSerial;
private:
	int nextId = 1;
	ShieldEvent recentEvent;
	bool allowAutoBlocking = true;

	void sendPingBack(ShieldEvent* shieldEvent);
    void sendStart();
	int writeValue(EPtr eptr, int start = 0) const;
    bool processInChar(ShieldEvent* shieldEvent, bool& hasEvent, char c);
};

#endif 