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

#include "Speech.h"

#include "SensorModels.h"

const PROGMEM char SERVICE_NAME_SPEECH[] = "SPEECH";
const int MEDIA_PAUSED = 4;

/// <summary>
/// Initializes a new instance of the <see cref="Speech"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Speech::Speech(const VirtualShield &shield) : Sensor(shield, 'T')
{
}

/// <summary>
/// Speaks the specified message.
/// </summary>
/// <param name="message">The message.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Speech::speak(const char * message)
{
    IsSpeaking = true;
	EPtr eptrs[] = { EPtr(MemPtr, MESSAGE, message) };
    return shield.block(writeAll(SERVICE_NAME_SPEECH, eptrs, 1), onEvent == NULL, WAITFOR_TIMEOUT, MEDIA_PAUSED);
}

int Speech::stop()
{
	return Sensor::sendStop(SERVICE_NAME_SPEECH);
    IsSpeaking = false;
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Speech::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) 
{
    if (shieldEvent->resultId < 1 || shieldEvent->resultId > 3) {
        IsSpeaking = false;
    }
    else if (shieldEvent->resultId > 0 && shieldEvent->resultId < 4) {
        IsSpeaking = true;
    }

	Sensor::onJsonReceived(root, shieldEvent);
}
