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

#ifndef Notification_h
#define Notification_h

#include <ArduinoJson.h>

#include "Sensor.h"
#include "ShieldEvent.h"
#include "VirtualShield.h"

class Notification : public Sensor
{
public:
	Notification(const VirtualShield &shield);

	int toast(const char * message, const char * tag = NULL, const char * image = NULL, const char * audio = NULL);
	int toast(const String &message, const String &tag = "", const String &image = "", const String &audio = "");
	int tile(const char * message, const char * tag = NULL, const char * image = NULL);
	int tile(const String &message, const String &tag = "", const String &image = "");

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif