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

#include <stdint.h>
#include <stdlib.h>

#include <Arduino.h>
#include <ArduinoJson.h>

#include "Attr.h"
#include "Sensor.h"
#include "SensorModels.h"
#include "ShieldEvent.h"

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
    ShieldEvent::callback_t onEvent = NULL;
    ShieldEvent::callback_t onConnect = NULL;
    ShieldEvent::callback_t onRefresh = NULL;
    ShieldEvent::callback_t onSuspend = NULL;
    ShieldEvent::callback_t onResume = NULL;

    VirtualShield();

	void begin(long bitRate = DEFAULT_BAUDRATE);
	void setPort(int port);

	bool hasError(ShieldEvent* shieldEvent = NULL);
	bool checkSensors(int watchForId = 0, int32_t timeout = 0, int waitForResultId = -1);
    int waitFor(int id, uint32_t timeout = WAITFOR_TIMEOUT, bool asSuccess = true, int resultId = -1);

	bool getEvent(ShieldEvent* shieldEvent);

	int directToSerial(const char* cmd);

	bool addSensor(Sensor* sensor);

	virtual void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent);

	void write(const char* text);
	int writeAll(const char* serviceName, EPtr values[], unsigned int count, Attr extraAttributes[] = 0, int extraAttributeCount = 0, const char sensorType = '\0');

	int writeAll(const char* serviceName);

	int beginWrite(const char* serviceName) ;
	int write(EPtr eptr) const;

	int endWrite();

    int block(int id, bool blocking, int32_t timeout = WAITFOR_TIMEOUT, int waitForResultId = -1);

	void setOnEvent(ShieldEvent::callback_t onEvent)
	{
		this->onEvent = onEvent;
	}

	void setOnConnect(ShieldEvent::callback_t onConnect)
	{
		this->onConnect = onConnect;
	}

	void setOnRefresh(ShieldEvent::callback_t onRefresh)
	{
		this->onRefresh = onRefresh;
	}

	void setOnSuspend(ShieldEvent::callback_t onSuspend)
	{
		this->onSuspend = onSuspend;
	}

	void setOnResume(ShieldEvent::callback_t onResume)
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
	static uint16_t hash(const char* s, size_t len = static_cast<size_t>(-2), unsigned int seed = 0);

protected:
	int sendFlashStringOnSerial(const char* flashStringAdr, int start = -1, bool encode = false) const;

	void onJsonStringReceived(char* json, ShieldEvent* shieldEvent);
	int onStringReceived(char* buffer, size_t length, ShieldEvent* shieldEvent);

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