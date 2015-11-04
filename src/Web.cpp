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

#include "Web.h"

const PROGMEM char SERVICE_WEB[] = "WEB";
const PROGMEM char GET[] = "Get";
const PROGMEM char POST[] = "Post";
const PROGMEM char DATA[] = "Data";
const PROGMEM char LEN[] = "Len";

/// <summary>
/// Initializes a new instance of the <see cref="Web"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Web::Web(const VirtualShield &shield) : Sensor(shield, 'W') {
}

/// <summary>
/// Performs a web Get, optionally returning a result.
/// </summary>
/// <param name="url">The url.</param>
/// <param name="parsingInstructions">The parsing instructions.</param>
/// <param name="maxLength">The maximum length of the result.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Web::get(const char * url, const char * parsingInstructions, int maxLength)
{
	return get(EPtr(MemPtr, URL, url), EPtr(parsingInstructions ? MemPtr : None, PARSE, parsingInstructions), maxLength);
}

/// <summary>
/// Performs a web Post, optionally returning a result.
/// </summary>
/// <param name="url">The url.</param>
/// <param name="data">The data.</param>
/// <param name="parsingInstructions">The parsing instructions.</param>
/// <param name="maxLength">The maximum length of the result.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Web::post(const char * url, const char * data, const char * parsingInstructions, int maxLength)
{
	return post(EPtr(MemPtr, URL, url), EPtr(MemPtr, DATA, data), EPtr(parsingInstructions ? MemPtr : None, PARSE, parsingInstructions), maxLength);
}

/// <summary>
/// Performs a web Get, optionally returning a result.
/// </summary>
/// <param name="url">The url.</param>
/// <param name="parsingInstructions">The parsing instructions.</param>
/// <param name="maxLength">The maximum length of the result.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Web::get(EPtr url, EPtr parsingInstructions, int maxLength)
{
	EPtr eptrs[] = { EPtr(ACTION, GET), url,
		EPtr(LEN, maxLength),
		parsingInstructions };
	return shield.block(writeAll(SERVICE_WEB, eptrs, 4), onEvent == NULL);
}

/// <summary>
/// Performs a web Post, optionally returning a result.
/// </summary>
/// <param name="url">The url.</param>
/// <param name="data">The data.</param>
/// <param name="parsingInstructions">The parsing instructions.</param>
/// <param name="maxLength">The maximum length of the result.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Web::post(EPtr url, EPtr data, EPtr parsingInstructions, int maxLength)
{
	EPtr eptrs[] = { EPtr(ACTION, POST), url, data,
		EPtr(LEN, maxLength),
		parsingInstructions };
	return shield.block(writeAll(SERVICE_WEB, eptrs, 5), onEvent == NULL);
}

/// <summary>
/// Copies the current web response (only valid before another web event) into a supplied buffer.
/// </summary>
/// <param name="responseBuffer">The buffer to place the response.</param>
/// <param name="length">The maximum length of the buffer.</param>
/// <param name="parts">Array of character pointers to populate after parsing.</param>
/// <param name="partCount">The part count.</param>
void Web::getResponse(char* responseBuffer, int length, char** parts, int partCount)
{
	strncpy(responseBuffer, response, length);
	if (parts && partCount > 0)
	{
		int count = 0;

		int index = 0;
		parts[count++] = &responseBuffer[index];

		while (count < partCount && ++index<length && responseBuffer[index])
		{
			if (responseBuffer[index] == '|')
			{
				parts[count++] = &responseBuffer[index + 1];
				responseBuffer[index] = '\0';
			}
		}
	}
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Web::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent)
{
	response = shieldEvent->result;
	resultId = shieldEvent->resultId;
	Sensor::onJsonReceived(root, shieldEvent);
}