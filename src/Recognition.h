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

#ifndef Recognition_h
#define Recognition_h

#include <ArduinoJson.h>

#include "Sensor.h"
#include "SensorModels.h"
#include "ShieldEvent.h"
#include "VirtualShield.h"

const int Confidence_High = 0;
const int Confidence_Medium = 1;
const int Confidence_Low = 2;
const int Confidence_Rejected = 3;
const int Confidence_Error = 5;

class Recognition : public Sensor {
public:
	int recognizedIndex = 0;
	int confidence;

	Recognition(const VirtualShield &shield);

    int listen(char* openTextBuffer, int* length, bool useUI = true, long timeout = 0);

    int listenFor(const char* constraint, bool useUI = true, int expectedConfidence = Confidence_Medium, long timeout = 0);
    int listenFor(EPtr constraint, bool useUI = true, int expectedConfidence = Confidence_Medium, long timeout = 0);

	int stop() override;

	bool heard(const char * text);
	bool heard(int spokenNumber);

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
private:
	char* openTextBuffer;
	int* length;
};

#endif