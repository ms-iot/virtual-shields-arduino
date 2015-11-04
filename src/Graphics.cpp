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

#include "Graphics.h"

#include <stdint.h>
#include <string.h>

#include "SensorModels.h"

const PROGMEM char SERVICE_NAME_GRAPHICS[] = "LCDG";
const PROGMEM char X[] = "X";
const PROGMEM char Y2[] = "Y2";
const PROGMEM char X2[] = "X2"; 
const PROGMEM char WIDTH[] = "Width";
const PROGMEM char HEIGHT[] = "Height"; 
const PROGMEM char BUTTON[] = "BUTTON";
const PROGMEM char RECTANGLE[] = "RECTANGLE";
const PROGMEM char LINE[] = "LINE";
const PROGMEM char TEXT[] = "TEXT";
const PROGMEM char PATH[] = "Path";
const PROGMEM char TOUCH[] = "TOUCH";
const PROGMEM char ORIENTATION[] = "ORIENTATION";
const PROGMEM char VALUE[] = "VALUE";
const PROGMEM char INPUTTXT[] = "INPUT";
const PROGMEM char MULTI[] = "MULTI";
const PROGMEM char EVENTS[] = "EVENTS";
const PROGMEM char CHANGEACTION[] = "CHANGE";

/// <summary>
/// Initializes a new instance of the <see cref="Screen"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Graphics::Graphics(const VirtualShield &shield) : Text(shield) {
}

int Graphics::line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, ARGB argb, unsigned int weight)
{
	EPtr eptrs[] = { EPtr(ACTION, LINE), EPtr(Y, (uint32_t)y1), EPtr(X, (uint32_t)x1),
		EPtr(X2, (uint32_t)x2), EPtr(Y2, (uint32_t)y2),
		EPtr(RGBAKEY, (uint32_t)argb.color, argb.color ? Uint : None),
		EPtr(WIDTH, (uint32_t) weight, weight == 1 ? None : Uint) };

	return shield.block(writeAll(SERVICE_NAME_GRAPHICS, eptrs, 7), onEvent == NULL);
}

/// <summary>
/// Draws graphical text at a location.
/// </summary>
/// <param name="x">The x.</param>
/// <param name="y">The y.</param>
/// <param name="text">The text.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Graphics::drawAt(unsigned int x, unsigned int y, const char * text, const char * tag, ARGB argb)
{
	EPtr eptrs[] = { EPtr(ACTION, TEXT), EPtr(Y, (uint32_t)y), 
		EPtr(X, (uint32_t)x), EPtr(MemPtr, MESSAGE, text), 
		EPtr(RGBAKEY, (uint32_t)argb.color, (uint32_t)argb.color ? Uint : None),
		EPtr(tag ? MemPtr : None, TAG, tag) };
	return writeAll(SERVICE_NAME_GRAPHICS, eptrs, 6);
}

/// <summary>
/// Draws the image at a location.
/// </summary>
/// <param name="x">The x.</param>
/// <param name="y">The y.</param>
/// <param name="url">The url (local or remote) of the image to draw.</param>
/// <param name="tag">The tag. Returned back for event recognition.</param>
/// <param name="width">The width.</param>
/// <param name="height">The height.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Graphics::drawImage(unsigned int x, unsigned int y, const char * url, const char * tag, unsigned int width, unsigned int height)
{
	EPtr eptrs[] = { EPtr(ACTION, IMAGE), EPtr(Y, (uint32_t)y), EPtr(X, (uint32_t)x),
		EPtr(WIDTH, (uint32_t)width, width ? Uint : None), EPtr(HEIGHT, (uint32_t)height, height ? Uint : None),
		EPtr(MemPtr, PATH, url), EPtr(tag ? MemPtr : None, TAG, tag) };

	return writeAll(SERVICE_NAME_GRAPHICS, eptrs, 7);
}

/// <summary>
/// Draws the image at a location.
/// </summary>
/// <param name="x">The x.</param>
/// <param name="y">The y.</param>
/// <param name="url">The url (local or remote) of the image to draw.</param>
/// <param name="tag">The tag. Returned back for event recognition.</param>
/// <param name="width">The width.</param>
/// <param name="height">The height.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Graphics::input(unsigned int x, unsigned int y, bool multiline, const char * text, unsigned int width, unsigned int height)
{
	EPtr eptrs[] = { EPtr(ACTION, INPUTTXT), EPtr(Y, (uint32_t)y), EPtr(X, (uint32_t)x),
		EPtr(WIDTH, (uint32_t)width, width ? Uint : None), EPtr(HEIGHT, (uint32_t)height, height ? Uint : None),
		EPtr(MULTI, multiline, multiline ? Bool : None),
		EPtr(text ? MemPtr : None, MESSAGE, text) };

	return writeAll(SERVICE_NAME_GRAPHICS, eptrs, 7);
}

int Graphics::change(unsigned int id, ARGB argb)
{
    EPtr eptrs[] = { EPtr(ACTION, CHANGEACTION), EPtr(PID, (uint32_t)id), EPtr(RGBAKEY, (uint32_t)argb.color, (uint32_t)argb.color ? Uint : None) };
    return writeAll(SERVICE_NAME_GRAPHICS, eptrs, 3);
}

/// <summary>
/// Fills a rectangle.
/// </summary>
/// <param name="x">The x.</param>
/// <param name="y">The y.</param>
/// <param name="width">The width.</param>
/// <param name="height">The height.</param>
/// <param name="rgba">The rgba.</param>
/// <param name="tag">The tag. Returned back for event recognition.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Graphics::fillRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, ARGB argb, const char * tag, bool enableExtendedEvents)
{
	EPtr eptrs[] = { EPtr(ACTION, RECTANGLE), EPtr(Y, (uint32_t)y), EPtr(X, (uint32_t)x),
		EPtr(WIDTH, (uint32_t)width), EPtr(HEIGHT, (uint32_t)height),
		EPtr(RGBAKEY, (uint32_t)argb.color, argb.color ? Uint : None),
		EPtr(tag ? MemPtr : None, TAG, tag),
        EPtr(EVENTS, enableExtendedEvents, enableExtendedEvents ? Bool : None)
    };

	return shield.block(writeAll(SERVICE_NAME_GRAPHICS, eptrs, 8), onEvent == NULL);
}

int Graphics::orientation(int autoRotationPreferences)
{
	EPtr eptrs[] = { EPtr(ACTION, ORIENTATION), EPtr(VALUE, autoRotationPreferences, autoRotationPreferences == -1 ? None : Int) };
	return shield.block(writeAll(SERVICE_NAME_GRAPHICS, eptrs, 2), onEvent == NULL);
}

/// <summary>
/// Adds a button.
/// </summary>
/// <param name="x">The x.</param>
/// <param name="y">The y.</param>
/// <param name="text">The text.</param>
/// <param name="tag">The tag. Returned back for event recognition.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Graphics::addButton(unsigned int x, unsigned int y, const char * text, const char * tag)
{
	EPtr eptrs[] = { EPtr(ACTION, BUTTON), EPtr(Y, (uint32_t)y), EPtr(X, (uint32_t)x), EPtr(MemPtr, MESSAGE, text), EPtr(MemPtr, TAG, tag ? tag : text) };
	return shield.block(writeAll(SERVICE_NAME_GRAPHICS, eptrs, 5), onEvent == NULL);
}

/// <summary>
/// Enables the touch input on the screen.
/// </summary>
/// <param name="enable">true to enable, false to disable.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Graphics::enableTouch(bool enable)
{
	EPtr eptrs[] = { EPtr(ACTION, enable ? ENABLE : DISABLE), EPtr(MESSAGE, TOUCH) };
	return writeAll(SERVICE_NAME_GRAPHICS, eptrs, 2);
}

/// <summary>
/// Determines whether the specified identifier is pressed.
/// </summary>
/// <param name="id">The identifier.</param>
/// <param name="shieldEvent">The shield event.</param>
/// <returns>true if pressed or tapped</returns>
bool Graphics::isPressed(int id, ShieldEvent* shieldEvent)
{
	if (shieldEvent == NULL)
	{
		shieldEvent = recentEvent;
	}

	return Sensor::isEvent(id, "pressed", shieldEvent) || Sensor::isEvent(id, "click", shieldEvent);
}

/// <summary>
/// Determines whether the specified tag is pressed.
/// </summary>
/// <param name="tag">The tag.</param>
/// <param name="shieldEvent">The shield event.</param>
/// <returns>true if pressed or tapped</returns>
bool Graphics::isPressed(const char * tag, ShieldEvent* shieldEvent)
{
	if (shieldEvent == NULL)
	{
		shieldEvent = recentEvent;
	}

	return Sensor::isEvent(tag, "pressed", shieldEvent) || Sensor::isEvent(tag, "click", shieldEvent);
}

/// <summary>
/// Determines whether the specified identifier is released.
/// </summary>
/// <param name="id">The identifier.</param>
/// <param name="shieldEvent">The shield event.</param>
/// <returns>true if released or tapped</returns>
bool Graphics::isReleased(int id, ShieldEvent* shieldEvent)
{
	if (shieldEvent == NULL)
	{
		shieldEvent = recentEvent;
	}

	return Sensor::isEvent(id, "released", shieldEvent) || Sensor::isEvent(id, "click", shieldEvent);
}

/// <summary>
/// Determines whether the specified tag is released.
/// </summary>
/// <param name="tag">The tag.</param>
/// <param name="shieldEvent">The shield event.</param>
/// <returns>true if released or tapped</returns>
bool Graphics::isReleased(const char * tag, ShieldEvent* shieldEvent)
{
	if (shieldEvent == NULL)
	{
		shieldEvent = recentEvent;
	}

	return Sensor::isEvent(tag, "released", shieldEvent) || Sensor::isEvent(tag, "click", shieldEvent);
}

/// <summary>
/// Determines whether a tag was clicked or tapped.
/// </summary>
/// <param name="tag">The tag.</param>
/// <param name="shieldEvent">The shield event.</param>
/// <returns>true if clicked or tapped</returns>
bool Graphics::isButtonClicked(const char * tag, ShieldEvent* shieldEvent)
{
	if (shieldEvent == NULL)
	{
		shieldEvent = recentEvent;
	}

	return Sensor::isEvent(tag, "click", shieldEvent) || Sensor::isEvent(tag, "tapped", shieldEvent);
}

/// <summary>
/// Determines whether a tag was clicked or tapped.
/// </summary>
/// <param name="tag">The tag.</param>
/// <param name="shieldEvent">The shield event.</param>
/// <returns>true if clicked or tapped</returns>
bool Graphics::isButtonClicked(int id, ShieldEvent* shieldEvent)
{
    if (shieldEvent == NULL)
    {
        shieldEvent = recentEvent;
    }

    return Sensor::isEvent(id, "click", shieldEvent) || Sensor::isEvent(id, "tapped", shieldEvent);
}

/// <summary>
/// Determines whether the specified shield event is a touch input event.
/// </summary>
/// <param name="shieldEvent">The shield event.</param>
/// <returns>true if this event is a touch input event</returns>
bool Graphics::isTouchEvent(ShieldEvent* shieldEvent)
{
	if (shieldEvent == NULL)
	{
		shieldEvent = recentEvent;
	}

	return strcmp(area, "TOUCH") == 0;
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Graphics::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent)
{
	area = static_cast<const char *>(root["Area"]);
	Sensor::onJsonReceived(root, shieldEvent);
}