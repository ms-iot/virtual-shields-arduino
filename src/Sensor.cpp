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

#include "Sensor.h"

const PROGMEM char SERVICE_SENSORS[] = "SENSORS";
const PROGMEM char SENSORS[] = "Sensors";
const PROGMEM char DELTA[] = "Delta";
const PROGMEM char INTERVAL[] = "Interval";

/// <summary>
/// Initializes a new instance of the <see cref="Sensor"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
/// <param name="sensorType">Filter for identifying a service.</param>
Sensor::Sensor(const VirtualShield &shield, const char sensorType) : shield(*const_cast<VirtualShield *>(&shield)), sensorType(sensorType) {
	this->shield.addSensor(this);
}

/// <summary>
/// Starts the sensor, triggered when longer than the interval and larger than the delta.
/// </summary>
/// <param name="interval">The interval in milliseconds. Zero is all.</param>
/// <param name="delta">The delta of expected change. Zero is all.</param>
int Sensor::start(double delta, long interval)  {
	int result = sensorAction(Start, delta, interval);
	this->isRunning = true;

	return result;
}

/// <summary>
/// Stops this sensor.
/// </summary>
int Sensor::stop()  {
	this->isRunning = false;
	return sensorAction(Stop);
}

/// <summary>
/// Gets a single, current sensor reading.
/// </summary>
int Sensor::get()  {
	this->isRunning = true;
	int result = sensorAction(Once);
	this->isRunning = false;

	return shield.block(result, onEvent == NULL);
}

/// <summary>
/// Requests a single reading when more than the delta beyond the current reading.
/// </summary>
/// <param name="delta">The delta of expected change. Zero is all.</param>
int Sensor::getOnChange(double delta)
{
	return sensorAction(OnceOnChange, delta);
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Sensor::onJsonReceived(JsonObject &, ShieldEvent *shieldEvent) {
	this->_isUpdated = true;
	recentEvent = shieldEvent;
	
	if (onEvent)
	{
		onEvent(shieldEvent);
	}
}

/// <summary>
/// Determines whether this sensor has an updated value. Resets to false after this call.
/// </summary>
/// <returns>true if updated from last check.</returns>
bool Sensor::isUpdated() {
	bool result = this->_isUpdated;
	if (result) {
		this->_isUpdated = false;
	}

	return result;
}

/// <summary>
/// Determines whether the specified shieldEvent matches the tag and action.
/// </summary>
/// <param name="tag">The tag.</param>
/// <param name="action">The action.</param>
/// <param name="shieldEvent">The shield event.</param>
/// <returns>bool.</returns>
bool Sensor::isEvent(const char* tag, const char* action, ShieldEvent* shieldEvent) {
	return strcmp(shieldEvent->tag, tag) == 0 && strcmp(shieldEvent->action, action) == 0;
}

/// <summary>
/// Determines whether the specified shieldEvent matches the id and action.
/// </summary>
/// <param name="id">The identifier.</param>
/// <param name="action">The action.</param>
/// <param name="shieldEvent">The shield event.</param>
/// <returns>bool.</returns>
bool Sensor::isEvent(int id, const char* action, ShieldEvent* shieldEvent) {
	return shieldEvent->id == id && strcmp(shieldEvent->action, action) == 0;
}

/// <summary>
/// Writes all EPtr values to the communication channel.
/// </summary>
/// <param name="serviceName">Name of the service.</param>
/// <param name="values">The values.</param>
/// <param name="count">The count of values.</param>
/// <returns>The new id of the message or a negative error.</returns>
int Sensor::writeAll(const char* serviceName, EPtr values[], int count, Attr extraAttributes[], int extraAttributeCount) {
	return shield.writeAll(serviceName, values, count, extraAttributes, extraAttributeCount, this->sensorType);
}

/// <summary>
/// Sends the specific action to start/stop/get/onChange the sensor using a delta and interval.
/// </summary>
/// <param name="sensorAction">The sensor action.</param>
/// <param name="delta">The delta.</param>
/// <param name="interval">The interval.</param>
/// <returns>int.</returns>
int Sensor::sensorAction(SensorAction sensorAction, double delta, long interval) const {
#ifdef debugSerial
	if (sensorAction > 0)
	{
		Serial.print("starting...");
	}
#endif
	const char sensorTypeSet[2] = { sensorType, '\0' };

	EPtr eptr2 = EPtr(sensorTypeSet, static_cast<int>(sensorAction));
	eptr2.keyIsMem = true;

	EPtr none = EPtr(None);

	EPtr eptrs[] = {
		EPtr(ArrayStart, SENSORS),
		eptr2,
		delta > 0 ? EPtr(DELTA, delta) : none,
		interval > 0 ? EPtr(INTERVAL, interval) : none,
		EPtr(ArrayEnd)
	};

	int id = this->shield.writeAll(SERVICE_SENSORS, eptrs, 5);

#ifdef debugSerial
	if (sensorAction < 2)
	{
		Serial.print("stopping...");
	}
#endif

	return id;
}

int Sensor::sendStop(const char* serviceName)
{
	EPtr eptrs[] = { EPtr(ACTION, STOP) };
	return shield.writeAll(serviceName, eptrs, 1);
}

int Sensor::filter(const char *, EPtr [], int)
{
    //TODO: Implement filter or remove
    return 0;
}

/// <summary>
/// Initializes a new instance of the <see cref="SensorEvent"/> struct.
/// </summary>
SensorEvent::SensorEvent() {
	this->shieldEventType = SensorShieldEventType;
}				 