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

#ifndef Media_h
#define Media_h

#include <ArduinoJson.h>

#include "Sensor.h"
#include "ShieldEvent.h"
#include "VirtualShield.h"

class Media : public Sensor
{
public:
	Media(const VirtualShield &shield);

	int play(const char * url, long length = 0);

	inline int playVideo(const char * url, long length = 0) {
        const long full_length = length + 7;
        char *full_url = new char[full_length];
        int return_value = 0;

        snprintf(full_url, full_length, "VIDEOS:%s", url);
		return_value = play(full_url, full_length);
        delete[] full_url;
        return return_value;
	}

	inline int playAudio(const char * url, long length = 0) {
		return playVideo(url, length);
	}

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif
