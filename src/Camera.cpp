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

#include "Camera.h"

#include "SensorModels.h"

const PROGMEM char SERVICE_CAMERA[] = "CAMERA";
const PROGMEM char PREVIEW[] = "PREVIEW";

/// <summary>
/// Initializes a new instance of the <see cref="Camera"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Camera::Camera(const VirtualShield &shield) : Sensor(shield, 'Z') {
}

/// <summary>
/// Enables the camera preview.
/// </summary>
/// <param name="enable">true to enable, false to disable.</param>
/// <returns>int.</returns>
int Camera::enablePreview(bool enable)
{
	EPtr eptrs[] = { EPtr(ACTION, enable ? ENABLE : DISABLE), EPtr(MESSAGE, PREVIEW) };
	return writeAll(SERVICE_CAMERA, eptrs, 2);
}

/// <summary>
/// Captures a picture and optionally saves/sends it to the specified URL.
/// </summary>
/// <param name="url">The URL.</param>
/// <returns>int.</returns>
int Camera::capture(const char * url)
{
    EPtr eptrs[] = { EPtr(url ? MemPtr : None, URL, url) };
    return shield.block(writeAll(SERVICE_CAMERA, eptrs, 1), onEvent == NULL);
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Camera::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent)
{
	Sensor::onJsonReceived(root, shieldEvent);
}