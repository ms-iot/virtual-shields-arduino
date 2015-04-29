#include "Text.h"
#include "SensorModels.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

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
int Text::clearLine(UINT line)
{
	EPtr eptrs[] = { EPtr(ACTION, CLEAR), EPtr(Y, (uint32_t) line) };
    return writeAll(SERVICE_NAME_LCDTEXT, eptrs, 2);
}

/// <summary>
/// Clears the element associated with the id.
/// </summary>
/// <param name="id">The id.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Text::clearId(UINT id)
{
	EPtr eptrs[] = { EPtr(ACTION, CLEAR), EPtr(PID, (uint32_t) id) };
    return writeAll(SERVICE_NAME_LCDTEXT, eptrs, 2);
}

/// <summary>
/// Prints the specified text.
/// </summary>
/// <param name="text">The text.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Text::print(String text, ARGB argb)
{
	EPtr eptrs[] = { EPtr(MemPtr, MESSAGE, text.c_str()), EPtr(RGBAKEY, (uint32_t)argb.color, (uint32_t)argb.color ? Uint : None) };
	return writeAll(SERVICE_NAME_LCDTEXT, eptrs, 2);
}

/// <summary>
/// Prints the specified double at the specified line.
/// </summary>
/// <param name="line">The line.</param>
/// <param name="value">The value.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Text::printAt(UINT line, double value, ARGB argb)
{
	EPtr eptrs[] = { EPtr(Y, (uint32_t)line), EPtr(MESSAGE, value), EPtr(RGBAKEY, (uint32_t)argb.color, (uint32_t)argb.color ? Uint : None) };
	return writeAll(SERVICE_NAME_LCDTEXT, eptrs, 3);
}

/// <summary>
/// Prints the specified text at the specified line.
/// </summary>
/// <param name="line">The line.</param>
/// <param name="text">The text.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Text::printAt(UINT line, String text, Attr extraAttributes[], int extraAttributeCount) {
	return printAt(line, EPtr(MemPtr, MESSAGE, text.c_str()), extraAttributes, extraAttributeCount);
}

/// <summary>
/// Prints the specified text at the specified line.
/// </summary>
/// <param name="line">The line.</param>
/// <param name="text">The text.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Text::printAt(UINT line, EPtr text, Attr extraAttributes[], int extraAttributeCount) {
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