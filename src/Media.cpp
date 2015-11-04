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

#include "Media.h"

#include "SensorModels.h"

const PROGMEM char SERVICE_PLAY[] = "PLAY";

/// <summary>
/// Initializes a new instance of the <see cref="Camera"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Media::Media(const VirtualShield &shield) : Sensor(shield, 'E') {
}


/// <summary>
/// Plays the specified media specified by the URL.
/// </summary>
/// <param name="url">The URL.</param>
/// <returns>int.</returns>
int Media::play(const char * url, long length)
{
	EPtr eptrs[] = { EPtr(MemPtr, URL, url), EPtr(MS, length, length ? Long : None) };
	return shield.block(writeAll(SERVICE_PLAY, eptrs, 2), onEvent == NULL);
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Media::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent)
{
	Sensor::onJsonReceived(root, shieldEvent);
}