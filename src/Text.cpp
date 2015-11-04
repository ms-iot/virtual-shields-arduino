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

#include "Text.h"

#include <stdint.h>

const PROGMEM char SERVICE_NAME_LCDTEXT[] = "LCDT";

/// <summary>
/// Initializes a new instance of the <see cref="Screen"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Text::Text(const VirtualShield &shield) : Sensor(shield, 'S') {
}

/// <summary>
/// Clears the screen of all elements.
/// </summary>
/// <returns>The id of the message. Negative if an error.</returns>
int Text::clear(ARGB argb)
{
	EPtr eptrs[] = { EPtr(ACTION, CLEAR), EPtr(RGBAKEY, (uint32_t)argb.color, (uint32_t) argb.color ? Uint : None) };
	return writeAll(SERVICE_NAME_LCDTEXT, eptrs, 2);
}

/// <summary>
/// Clears the line.
/// </summary>
/// <param name="line">The line.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Text::clearLine(unsigned int line)
{
	EPtr eptrs[] = { EPtr(ACTION, CLEAR), EPtr(Y, (uint32_t) line) };
    return writeAll(SERVICE_NAME_LCDTEXT, eptrs, 2);
}

/// <summary>
/// Clears the element associated with the id.
/// </summary>
/// <param name="id">The id.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Text::clearId(unsigned int id)
{
	EPtr eptrs[] = { EPtr(ACTION, CLEAR), EPtr(PID, (uint32_t) id) };
    return writeAll(SERVICE_NAME_LCDTEXT, eptrs, 2);
}

/// <summary>
/// Prints the specified text.
/// </summary>
/// <param name="text">The text.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Text::print(const char * text, ARGB argb)
{
	EPtr eptrs[] = { EPtr(MemPtr, MESSAGE, text), EPtr(RGBAKEY, (uint32_t)argb.color, (uint32_t)argb.color ? Uint : None) };
	return writeAll(SERVICE_NAME_LCDTEXT, eptrs, 2);
}

/// <summary>
/// Prints the specified double at the specified line.
/// </summary>
/// <param name="line">The line.</param>
/// <param name="value">The value.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Text::printAt(unsigned int line, double value, ARGB argb)
{
	EPtr eptrs[] = { EPtr(Y, (uint32_t)line), EPtr(MESSAGE, value), EPtr(RGBAKEY, (uint32_t)argb.color, (uint32_t)argb.color ? Uint : None) };
	return writeAll(SERVICE_NAME_LCDTEXT, eptrs, 3);
}

/// <summary>
/// Prints the specified double at the specified line.
/// </summary>
/// <param name="line">The line.</param>
/// <param name="value">The value.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Text::printAt(unsigned int line, const char * text, ARGB argb)
{
	EPtr eptrs[] = { EPtr(Y, (uint32_t)line), EPtr(MemPtr, MESSAGE, text), EPtr(RGBAKEY, (uint32_t)argb.color, (uint32_t)argb.color ? Uint : None) };
	return writeAll(SERVICE_NAME_LCDTEXT, eptrs, 3);
}

/// <summary>
/// Prints the specified text at the specified line.
/// </summary>
/// <param name="line">The line.</param>
/// <param name="text">The text.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Text::printAt(unsigned int line, const char * text, Attr extraAttributes[], int extraAttributeCount) {
	return printAt(line, EPtr(MemPtr, MESSAGE, text), extraAttributes, extraAttributeCount);
}

/// <summary>
/// Prints the specified text at the specified line.
/// </summary>
/// <param name="line">The line.</param>
/// <param name="text">The text.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Text::printAt(unsigned int line, EPtr text, Attr extraAttributes[], int extraAttributeCount) {
	EPtr eptrs[] = { EPtr(Y, (uint32_t) line), text };
	return writeAll(SERVICE_NAME_LCDTEXT, eptrs, 2, extraAttributes, extraAttributeCount);
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Text::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent)
{
	Sensor::onJsonReceived(root, shieldEvent);
}