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

#include "Notification.h"

#include "SensorModels.h"

const PROGMEM char SERVICE_NOTIFICATION[] = "NOTIFY";
const PROGMEM char TOAST[] = "Toast";
const PROGMEM char TILE[] = "Tile";

/// <summary>
/// Initializes a new instance of the <see cref="Microphone"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Notification::Notification(const VirtualShield &shield) : Sensor(shield, 'N') {
}

/// <summary>
/// Send/Show a toast notification.
/// </summary>
/// <param name="milliseconds">The length in milliseconds.</param>
/// <param name="url">The URL.</param>
/// <returns>int.</returns>
int Notification::toast(const char * message, const char * tag, const char * image, const char * audio)
{
	EPtr eptrs[] = { EPtr(MemPtr, MESSAGE, message), EPtr(tag ? MemPtr : None, TAG, tag), 
		EPtr(image ? MemPtr : None, IMAGE, image), EPtr(audio ? MemPtr : None, AUDIO, audio), 
		EPtr(ACTION, TOAST) };
	return writeAll(SERVICE_NOTIFICATION, eptrs, 5);
}

/// <summary>
/// Send/Show a toast notification.
/// </summary>
/// <param name="milliseconds">The length in milliseconds.</param>
/// <param name="url">The URL.</param>
/// <returns>int.</returns>
int Notification::tile(const char * message, const char * tag, const char * image)
{
	EPtr eptrs[] = { EPtr(MemPtr, MESSAGE, message), EPtr(tag ? MemPtr : None, TAG, tag),
		EPtr(image ? MemPtr : None, IMAGE, image), EPtr(ACTION, TILE) };
	return writeAll(SERVICE_NOTIFICATION, eptrs, 4);
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Notification::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent)
{
	Sensor::onJsonReceived(root, shieldEvent);
}