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

#include "VirtualShield.h"
#if defined(_WINDOWS_)
  #include <malloc.h>
#else
  #include <alloca.h>
#endif
#include <string.h>
  
#include <ArduinoJson.h>
#include "SensorModels.h"

#if defined(ARDUINO) && ARDUINO > 100
  // Define the serial port that is used to talk to the virtual shield.
  #define VIRTUAL_SERIAL_PORT0 Serial

  // If it has dual serial ports (Leonardo), prefer the second one (pins 0, 1) for bluetooth.
  #if defined(__AVR_ATmega32U4__)
    #define VIRTUAL_SERIAL_PORT1 Serial1
    #define debugSerial
    #define debugSerialIn
  #else 
    #define VIRTUAL_SERIAL_PORT1 Serial
  #endif
#else
  #include "..\Time.h"
  using namespace Windows::Storage::Streams;
  using namespace Microsoft::Maker::Time;
#endif

static const int SERIAL_ERROR = -1;
static const int SERIAL_SUCCESS = 0;

// Strings used to build virtual shield commands.
const PROGMEM char MESSAGE_SERVICE_START[] = "{'Service':'";
const PROGMEM char MESSAGE_SERVICE_TO_ID[] = "','Id':";
const PROGMEM char MESSAGE_QUOTE[] = "'";
const PROGMEM char MESSAGE_SEPARATOR[] = ",'";
const PROGMEM char MESSAGE_PAIR_SEPARATOR[] = "':";
const PROGMEM char MESSAGE_END2[] = "}";
const PROGMEM char ARRAY_START[] = "[{";
const PROGMEM char ARRAY_END[] = "}]";
const PROGMEM char NONTEXT_END[] = "}";
const PROGMEM char MESSAGE_END[] = "'}";
const PROGMEM char SERVICE_NAME_SERVICE[] = "SYSTEM";
const PROGMEM char PONG[] = "PONG";
const PROGMEM char TYPE[] = "TYPE";
const PROGMEM char START[] = "START";
const PROGMEM char LEN[] = "LEN";

const char AWAITING_MESSAGE[] = "{}";
const char SYSTEM_EVENT = '!';

const int requestInterval = 1000;
const int perMessageInterval = 25;
const int maxRememberedSensors = 10;

const int maxReadBuffer = 160;
const int maxJsonReadBuffer = maxReadBuffer + 2;
const int maxJsonWriteBuffer = 65536;

char readBuffer[maxReadBuffer];
int readBufferIndex = 0;

int bracketCount = 0;
uint32_t lastOpenRequest = 0;
bool isArrayStarted = false;
int recentEventErrorId = 0;

Sensor* sensors[maxRememberedSensors];
int sensorCount = 0;

/// <summary>
/// Initializes a new instance of the <see cref="VirtualShield"/> class.
/// </summary>
VirtualShield::VirtualShield()
{
    //_VShieldSerial = &VIRTUAL_SERIAL_PORT1;
}

/// <summary>
/// Adds a sensor to the list of known sensors in order to match and dispatch for incoming events.
/// </summary>
/// <param name="sensor">The sensor to add.</param>
/// <returns>true if it was added, false if the maxRememberedSensors is reached.</returns>
bool VirtualShield::addSensor(Sensor* sensor) {
	if (sensorCount == maxRememberedSensors)
	{
		return false;
	}

	sensors[sensorCount++] = sensor;
	return true;
}
/*
/// <summary>
/// Sets the port for bluetooth (this only works for __AVR_ATmega32U4__ where there are more than one port).
/// </summary>
/// <param name="port">The port.</param>
void VirtualShield::setPort(int port) 
{
	if (port == 0) {
		_VShieldSerial = &VIRTUAL_SERIAL_PORT0;
	}
	else if (port == 1) {
		_VShieldSerial = &VIRTUAL_SERIAL_PORT1;
	}
}
*/
/// <summary>
/// Begins the specified bit rate.
/// </summary>
/// <param name="bitRate">The bit rate to use for the virtual shield serial connection.</param>
void VirtualShield::begin(long bitRate)
{
#if defined(ARDUINO) && ARDUINO >= 100
    reinterpret_cast<HardwareSerial *>(_VShieldSerial)->begin(bitRate);
#else
    _VShieldSerial->begin(static_cast<uint32_t>(bitRate), SerialConfig::SERIAL_8N1);
#endif

    delay(500);
    sendStart();

	if (this->onConnect)
	{
		this->onConnect(&recentEvent);
	}

	if (this->onRefresh)
	{
		this->onRefresh(&recentEvent);
	}
}

/// <summary>
/// Blocks while waiting for a specific id-based response (only when blocking is true and allowAutoBlocking is true).
/// </summary>
int VirtualShield::block(int id, bool blocking, int32_t timeout, int watchForResultId)
{
    return allowAutoBlocking && blocking ? waitFor(id, timeout, watchForResultId) : id;
}

/// <summary>
/// Flushes this instance onto the serial port.
/// </summary>
void VirtualShield::flush()
{
	_VShieldSerial->flush();
	lastOpenRequest = millis();
}

long lastClosure = 0;
long lastOpening = 0;
bool inEvent = false;

/// <summary>
/// Gets zero or one available events for processing.
/// </summary>
/// <param name="shieldEvent">The address of ShieldEvent to populate.</param>
/// <returns>true if an event was populated</returns>
bool VirtualShield::getEvent(ShieldEvent* shieldEvent) {
	bool hasEvent = false;

	if (_VShieldSerial->available() == 0 && millis() > lastOpenRequest + requestInterval) //and timing!
	{
#ifdef debugSerial
		Serial.print(AWAITING_MESSAGE);
#endif
		_VShieldSerial->write(AWAITING_MESSAGE[0]);
        _VShieldSerial->write(AWAITING_MESSAGE[1]);
        _VShieldSerial->write(AWAITING_MESSAGE[2]);
        _VShieldSerial->flush();
		lastOpenRequest = millis();
	}

	bool hadData = false;
	while (!inEvent && _VShieldSerial->available() > 0) {
		hadData = true;
		char c = static_cast<char>(_VShieldSerial->read());

#ifdef debugSerialIn
		Serial.print(c);
#endif

		if (readBufferIndex < maxReadBuffer-1) {
			readBuffer[readBufferIndex++] = c;
        } 
        else
        {
            int i = readBufferIndex;
            while (i >= 0)
            {
                if (readBuffer[i] == '{')
                {
                    memmove(readBuffer, readBuffer + i, maxReadBuffer - i);
                    readBufferIndex = maxReadBuffer - i;
                    readBuffer[readBufferIndex++] = c;
                    bracketCount = 0;

                    for (int j = 0; j<readBufferIndex; j++)
                    {
                        if (readBuffer[j] == '{')
                        {
                            processInChar(shieldEvent, hasEvent, c);
                        }
                    }

                    break;
                }
                i--;
            }
        }
		
        if (processInChar(shieldEvent, hasEvent, c))
        {
            break;
        }
	}

	if (hadData)
	{
		lastOpenRequest = millis() - requestInterval + perMessageInterval;
	}

    if (lastClosure - lastOpening > requestInterval)
    {
        readBufferIndex = 0;
        bracketCount = 0;
    }

	return hasEvent;
}

/// <summary>
/// Process single incoming character - pass to full handler when recognized
/// </summary>
/// <param name="shieldEvent">The shield event.</param>
bool VirtualShield::processInChar(ShieldEvent* shieldEvent, bool& hasEvent, char c) 
{
    if (c == '{') {
        if (bracketCount++ == 0)
        {
            lastOpening = millis();
        }
    }
    else if (c == '}') {
        if (--bracketCount < 1) {
            bracketCount = 0;
            lastClosure = millis();

            if (readBufferIndex < maxReadBuffer) {
                readBuffer[readBufferIndex++] = '\0';
                inEvent = true;
#ifdef debugSerial
                Serial.print(AWAITING_MESSAGE);
#endif
                _VShieldSerial->write(AWAITING_MESSAGE[0]);
                _VShieldSerial->write(AWAITING_MESSAGE[1]);
                _VShieldSerial->write(AWAITING_MESSAGE[2]);
                _VShieldSerial->flush();
                lastOpenRequest = millis();
                onStringReceived(readBuffer, readBufferIndex, shieldEvent);
                inEvent = false;
                hasEvent = true;
                readBufferIndex = 0;
                return true;
            }

            readBufferIndex = 0;
        }
    }

    return false;
}

/// <summary>
/// Sends the ping back form a ping request.
/// </summary>
/// <param name="shieldEvent">The shield event.</param>
void VirtualShield::sendStart()
{
    EPtr eptrs[] = { EPtr(ACTION, START), EPtr(MemPtr, TYPE, "!"), EPtr(LEN, maxReadBuffer) };
    writeAll(SERVICE_NAME_SERVICE, eptrs, 3);
}

/// <summary>
/// Sends the ping back form a ping request.
/// </summary>
/// <param name="shieldEvent">The shield event.</param>
void VirtualShield::sendPingBack(ShieldEvent *)
{
	EPtr eptrs[] = { EPtr(ACTION, PONG), EPtr(MemPtr, TYPE, "!") };
	writeAll(SERVICE_NAME_SERVICE, eptrs, 2);
}

/// <summary>
/// Event called when a valid json message was received. 
/// Dispatches to added sensors that match the incoming Type.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void VirtualShield::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) {
	const char* sensorType = static_cast<const char *>(root["Type"]);

	shieldEvent->tag = static_cast<const char*>(root["Tag"]);

	shieldEvent->id = static_cast<int>(root["Pid"]);
	if (shieldEvent->id == 0) {
		shieldEvent->id = static_cast<int>(root["Id"]);
	}

	shieldEvent->resultId = static_cast<long>(root["ResultId"]);

	shieldEvent->result = static_cast<const char *>(root["Result"]);
	shieldEvent->resultHash = hash(shieldEvent->result);
	shieldEvent->action = static_cast<const char *>(root["Action"]);
	shieldEvent->actionHash = hash(shieldEvent->action);
	shieldEvent->value = static_cast<double>(root["Value"]);
	
	if (sensorType) {
		// special '!' Type which means remote device just connected/reconnected
		if (sensorType[0] == SYSTEM_EVENT)
		{
			shieldEvent->cargo = &root;
			bool refresh = false;
			switch (shieldEvent->resultHash)
			{
			case PING_HASH:
				sendPingBack(shieldEvent);
				break;
			case REFRESH_HASH:
				refresh = true;
				break;
			case CONNECT_HASH:
				refresh = true;
				if (onConnect)
				{
					onConnect(shieldEvent);
				}
				break;
			case SUSPEND_HASH:
				if (onSuspend)
				{
					onSuspend(shieldEvent);
				}
				break;
			case RESUME_HASH:
				refresh = true;
				if (onResume)
				{
					onResume(shieldEvent);
				}
				break;
			}

			if (refresh && onRefresh)
			{
				onRefresh(shieldEvent);
			}					  
		} 
		else
		{
			const char sensorTypeChar = sensorType[0];
			for (int i = 0; i < sensorCount; i++)
			{
				// check each sensor for matching Type
				if (sensors[i]->sensorType == sensorTypeChar) {
					sensors[i]->onJsonReceived(root, shieldEvent);

					if (shieldEvent->shieldEventType == SensorShieldEventType) {
						SensorEvent* sensorEvent = static_cast<SensorEvent*>(shieldEvent);
						sensorEvent->sensor = sensors[i];
					}

					break;
				}
			}
		}
	}

	if (onEvent)
	{
		onEvent(shieldEvent);
	}
}

/// <summary>
/// Event callback for when a full json string is received.
/// </summary>
/// <param name="json">The json string.</param>
/// <param name="shieldEvent">The shield event to populate.</param>
void VirtualShield::onJsonStringReceived(char* json, ShieldEvent* shieldEvent) {
#if defined(_WINDOWS_)
	DynamicJsonBuffer jsonBuffer;
#else
    StaticJsonBuffer<maxJsonReadBuffer> jsonBuffer;
#endif
	JsonObject& root = jsonBuffer.parseObject(json);
	if (root.success()) {
		onJsonReceived(root, shieldEvent);
	} 
	else
	{
		//Serial.print("FAILED");
	}
}


/// <summary>
/// Event callback for when a full string is received.
/// </summary>
/// <param name="buffer">The buffer.</param>
/// <param name="length">The length.</param>
/// <param name="shieldEvent">The shield event.</param>
int VirtualShield::onStringReceived(char* buffer, size_t length, ShieldEvent* shieldEvent) {

	if (length >= maxJsonReadBuffer) { return SERIAL_ERROR; }

	char *json = static_cast<char*>(alloca(length + 1));
	if (!json) { return SERIAL_ERROR; }

    strncpy(json, buffer, length);
    json[length] = '\0';
    onJsonStringReceived(json, shieldEvent);
	return SERIAL_SUCCESS;
}

/// <summary>
/// Receives events as long as they exist, or until an optional timeout occurs.
/// </summary>
/// <param name="watchForId">An id to return true if found. Otherwise true is returned for any events processed.</param>
/// <param name="timeout">The timeout in milliseconds.</param>
/// <returns>true if the id matched an incoming event, or if no id, any event.</returns>
bool VirtualShield::checkSensors(int watchForId, int32_t timeout, int watchForResultId) {
	bool hadEvents = false;

	uint32_t started = millis();
	recentEventErrorId = 0;
	while (getEvent(&recentEvent) && (timeout == 0 || started+timeout <= millis()) ) {
		hadEvents = (watchForId == 0 || recentEvent.id == watchForId) && (watchForResultId == -1 || recentEvent.resultId == watchForResultId);
	}

	return hadEvents;
}

//------------------------------------------------------------//

/// <summary>
/// Writes the specified text to the communication channel.
/// </summary>
/// <param name="text">The text.</param>
void VirtualShield::write(const char* text)
{
#if defined(_WINRT_DLL)
    _VShieldSerial->write(Platform::ArrayReference<uint8_t>(reinterpret_cast<uint8_t *>(const_cast<char *>(text)), strnlen(text, maxJsonWriteBuffer)));
#else
	_VShieldSerial->write(text);
#endif
}

/// <summary>
/// Writes the service name only to the communication channel.
/// </summary>
/// <param name="serviceName">Name of the service.</param>
/// <returns>int.</returns>
int VirtualShield::writeAll(const char* serviceName)  {
	uint8_t id = beginWrite(serviceName);
	if (endWrite() != SERIAL_SUCCESS) return SERIAL_ERROR;

	return id;
}

/// <summary>
/// Blocks and awaits an event with an id.
/// </summary>
/// <param name="id">The id.</param>
/// <param name="timeout">The timeout.</param>
/// <returns>The matching id, or zero if not found.</returns>
int VirtualShield::waitFor(int id, uint32_t timeout, bool asSuccess, int resultId)
{
	if (id < 0)
	{
		return id;
	}

	timeout = timeout + millis();

	bool found = false;
	while (!found && millis() < timeout) {
		found = checkSensors(id, 0, resultId);
	}

	return found ? (asSuccess && id < 0 ? 0 : id) : 0;
}

/// <summary>
/// Returns true
/// </summary>
/// <param name="id">The id.</param>
/// <param name="timeout">The timeout.</param>
/// <returns>The matching id, or zero if not found.</returns>
bool VirtualShield::hasError(ShieldEvent* shieldEvent) {
	return shieldEvent ? shieldEvent->resultId < 0 : recentEvent.resultId < 0;
}

/// <summary>
/// Begins a service write operation to the communication channel. Increments a message id, includes and returns the id.
/// </summary>
/// <param name="serviceName">Name of the service.</param>
/// <returns>The new id of the message or a negative error..</returns>
int VirtualShield::beginWrite(const char* serviceName)  
{
	int id = nextId++; 

	if (nextId < 0) //let's stay positive
	{
		nextId = 1;
	}

	if (sendFlashStringOnSerial(MESSAGE_SERVICE_START) != SERIAL_SUCCESS) return SERIAL_ERROR;
	if (sendFlashStringOnSerial(serviceName) != SERIAL_SUCCESS) return SERIAL_ERROR;
	if (sendFlashStringOnSerial(MESSAGE_SERVICE_TO_ID) != SERIAL_SUCCESS) return SERIAL_ERROR;
	_VShieldSerial->print(id);
#ifdef debugSerial
	Serial.print(id);
#endif

	return id;
}

/// <summary>
/// Writes all EPtr values to the communication channel.
/// </summary>
/// <param name="serviceName">Name of the service.</param>
/// <param name="values">The values.</param>
/// <param name="count">The count of values.</param>
/// <returns>The new id of the message or a negative error.</returns>
int VirtualShield::writeAll(const char* serviceName, EPtr values[], unsigned int count, Attr extraAttributes[], int extraAttributeCount, const char sensorType) {
	uint8_t id = beginWrite(serviceName);

	for (size_t i = 0; i < count; i++)
	{
		write(values[i]);
	}

	if (sensorType)
	{
		write(EPtr(TYPE, sensorType));
	}

	for (int i = 0; i < extraAttributeCount; i++)
	{
		write(extraAttributes[i]);
	}

	if (endWrite() != SERIAL_SUCCESS) return SERIAL_ERROR;

	return id;
}

/// <summary>
/// Writes the specified eptr.
/// </summary>
/// <param name="eptr">The eptr.</param>
/// <returns>Zero if no error, negative if an error.</returns>
int VirtualShield::write(EPtr eptr)	const
{
	if (eptr.ptrType == None)
	{
		return SERIAL_SUCCESS;
	}

	if (eptr.ptrType == ArrayEnd)
	{
		if (sendFlashStringOnSerial(ARRAY_END) != SERIAL_SUCCESS) return SERIAL_ERROR;
		return SERIAL_SUCCESS;
	}

	if (isArrayStarted)
	{
		if (sendFlashStringOnSerial(MESSAGE_QUOTE) != SERIAL_SUCCESS) return SERIAL_ERROR;
		isArrayStarted = false;
	} 
	else
	{
		if (sendFlashStringOnSerial(MESSAGE_SEPARATOR) != SERIAL_SUCCESS) return SERIAL_ERROR;
	}

	if (eptr.keyIsMem)
	{
#if defined(_WINRT_DLL)
        _VShieldSerial->write(Platform::ArrayReference<uint8_t>(reinterpret_cast<uint8_t *>(const_cast<char *>(eptr.key)), strnlen(eptr.key, maxJsonWriteBuffer)));
#else
        _VShieldSerial->print(eptr.key);
  #ifdef debugSerial
		Serial.print(eptr.key);
  #endif
#endif
    }
	else
	{
		if (sendFlashStringOnSerial(eptr.key) != SERIAL_SUCCESS) return SERIAL_ERROR;
	}

	if (sendFlashStringOnSerial(MESSAGE_PAIR_SEPARATOR) != SERIAL_SUCCESS) return SERIAL_ERROR;

	if (eptr.asText)
	{
		if (sendFlashStringOnSerial(MESSAGE_QUOTE) != SERIAL_SUCCESS) return SERIAL_ERROR;
	}

	writeValue(eptr);

	if (eptr.asText)
	{
		if (sendFlashStringOnSerial(MESSAGE_QUOTE) != SERIAL_SUCCESS) return SERIAL_ERROR;
	}

	return SERIAL_SUCCESS;
}

int VirtualShield::writeValue(EPtr eptr, int start) const
{
	int valueIndex = 0;
	int formatPositionIndex = 0;

	int result = SERIAL_SUCCESS;
	switch (eptr.ptrType)
	{
	case ArrayStart:
		result = sendFlashStringOnSerial(ARRAY_START);
		isArrayStarted = true;
		break;
	case ProgPtr:
		result = sendFlashStringOnSerial(eptr.value, start, true);
		break;
	case MemPtr:
	{
		const char* scanner = eptr.value;
		scanner = eptr.value;
		int count = eptr.length;
		while (count == -1 ? scanner[0] : count-- > 0) {
			if (!eptr.encoded && (scanner[0] == '\'' || scanner[0] == '\\' )) {
				_VShieldSerial->write('\\');
#ifdef debugSerial
				Serial.write('\\');
#endif
			}
			_VShieldSerial->write(scanner[0]);
#ifdef debugSerial
			Serial.write(scanner[0]);
#endif
			scanner++;
		}

		break;
	}
	case Char:
		_VShieldSerial->print(eptr.charValue);
#ifdef debugSerial
		Serial.print(eptr.charValue);
#endif
		break;
	case Int:
		_VShieldSerial->print(eptr.intValue);
#ifdef debugSerial
		Serial.print(eptr.intValue);
#endif
		break;
	case Uint:
		_VShieldSerial->print(eptr.uintValue);
#ifdef debugSerial
		Serial.print(eptr.uintValue);
#endif
		break;
	case Long:
		_VShieldSerial->print(static_cast<int32_t>(eptr.longValue));
#ifdef debugSerial
		Serial.print(eptr.longValue);
#endif
		break;
	case Double:
		_VShieldSerial->print(eptr.doubleValue, 4);
#ifdef debugSerial
		Serial.print(eptr.doubleValue, 4);
#endif
		break;
	case Bool:
		_VShieldSerial->print(eptr.boolValue);
#ifdef debugSerial
		Serial.print((bool)eptr.doubleValue);
#endif
		break;
	case Format:
		//Serial.print(eptr.eptrs[1].doubleValue);
		//assume format is from flash
		while (valueIndex == 0 || formatPositionIndex > 0)
		{
			formatPositionIndex = writeValue(eptr.eptrs[0], formatPositionIndex);
			if (formatPositionIndex == 0)
			{
				break;
			}

			result = writeValue(eptr.eptrs[++valueIndex], -1);
			if (result != SERIAL_SUCCESS)
			{
				break;
			}
		}

		break;
	default:
		break;
	}

	return result;
}

int VirtualShield::parseToHash(const char* text, unsigned int *hash, int hashCount, char separator, unsigned int length)
{
	int index = 0;
	int start = 0;
	int hashIndex = 0;
	int count = 0;

	while ((length == static_cast<unsigned int>(-1) || length-- > 0) && (text[index] || index > start))
	{
		if (!text[index] || text[index] == separator || length == 0)
		{
			hash[hashIndex++] = VirtualShield::hash(text+start, index-start + (length == 0));
			start = index + 1;

			if (++count == hashCount || !text[index])
			{
				break;
			}
		}

		index = index + 1;
	}

	return count;
}


// per Paul Larson - Microsoft Research
uint16_t VirtualShield::hash(const char* s, size_t len, unsigned int seed)
{
    if (!s) { return seed; }
	uint16_t hash = seed;
	while ((len == static_cast<size_t>(-2)) ? *s : len-- > 0)
	{
		hash = hash * 101 + *s++;
	}

	return hash;
}

/// <summary>
/// Ends the write operation.
/// </summary>
/// <returns>Zero if no error, negative if an error.</returns>
int VirtualShield::endWrite()
{
	if (sendFlashStringOnSerial(MESSAGE_END2) != SERIAL_SUCCESS) return SERIAL_ERROR;
	this->flush();
	return SERIAL_SUCCESS;
}

int VirtualShield::directToSerial(const char* cmd)
{
#if defined(_WINRT_DLL)
    _VShieldSerial->write(Platform::ArrayReference<uint8_t>(reinterpret_cast<uint8_t *>(const_cast<char *>(cmd)), strnlen(cmd, maxJsonWriteBuffer)));
#else
    _VShieldSerial->print(cmd);
#endif
	return SERIAL_SUCCESS;
}

/// <summary>
/// Sends the flash (PROGMEM) string on the communication channel.
/// </summary>
/// <param name="flashStringAdr">The flash (PROGMEM) string address.</param>
/// <returns>Zero if no error, negative if an error.</returns>
int VirtualShield::sendFlashStringOnSerial(const char* flashStringAdr, int start, bool encode) const
{
	char dataChar = '\0';
	const size_t actualStart = start < 0 ? 0 : start;
	const bool isFormatted = start > -1;

#if defined(_WINDOWS_)
	for (size_t i = actualStart; i < strlen(flashStringAdr); i++)
	{
		dataChar = *(flashStringAdr + i);
#else
	for (size_t i = actualStart; i < strlen_PF((uint_farptr_t)flashStringAdr); i++)
	{
		dataChar = pgm_read_byte_near(flashStringAdr + i);
#endif
		if (isFormatted && dataChar == '~')
		{
			return i + 1;
		}

		if (encode && dataChar == '\'') 
		{
			_VShieldSerial->write('\\');
#ifdef debugSerial
			Serial.write('\\');
#endif
		}

		_VShieldSerial->print((char)dataChar);
#ifdef debugSerial
		Serial.print((char)dataChar);
#endif
	}

	return SERIAL_SUCCESS;
}
