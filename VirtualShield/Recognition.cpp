#include "Recognition.h"
#include "Sensor.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

const PROGMEM char SERVICE_NAME_RECOGNIZE[] = "RECOGNIZE";
const PROGMEM char SPEECH_UI[] = "UI";
const PROGMEM char CONFIDENCE[] = "CONFIDENCE";

/// <summary>
/// Initializes a new instance of the <see cref="Speech"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Recognition::Recognition(const VirtualShield &shield) : Sensor(shield, 'R')
{
}

/// <summary>
/// Recognizes open text / not constricted without a UI. 
/// </summary>
/// <returns>The id of the message. Negative if an error.</returns>
int Recognition::listen(char* openTextBuffer, int* length, bool useUI, long timeout)
{
	this->openTextBuffer = openTextBuffer;
	this->length = length;
	this->openTextBuffer[0] = 0;
	this->recognizedIndex = 0;
	
	EPtr eptrs[] = { EPtr(SPEECH_UI, useUI), EPtr(MS, timeout) };
    return shield.block(writeAll(SERVICE_NAME_RECOGNIZE, eptrs, 2), onEvent == 0);
}

int Recognition::stop()
{
	return Sensor::sendStop(SERVICE_NAME_RECOGNIZE);
}

/// <summary>
/// Recognizes the specified constricted recognition text without a UI.
/// </summary>
/// <param name="recognitionText">The recognition text (words or groups).</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Recognition::listenFor(String recognitionText, bool useUI, int expectedConfidence, long timeout)
{
    return listenFor(recognitionText.c_str(), useUI, expectedConfidence, timeout);
}

/// <summary>
/// Recognizes the specified constricted recognition text without a UI.
/// </summary>
/// <param name="recognitionText">The recognition text (words or groups).</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Recognition::listenFor(const char* recognitionText, bool useUI, int expectedConfidence, long timeout)
{
    return listenFor(EPtr(MemPtr, MESSAGE, recognitionText), useUI, expectedConfidence, timeout);
}

/// <summary>
/// Recognizes the specified constricted recognition text without a UI.
/// </summary>
/// <param name="recognitionText">The recognition text (words or groups).</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Recognition::listenFor(EPtr constraint, bool useUI, int expectedConfidence, long timeout)
{
	this->openTextBuffer = 0;
	this->recognizedIndex = 0;

	EPtr eptrs[] = { constraint, EPtr(CONFIDENCE, expectedConfidence), EPtr(SPEECH_UI, useUI), EPtr(MS, timeout) };
	return shield.block(writeAll(SERVICE_NAME_RECOGNIZE, eptrs, 4), onEvent == 0);
}

/// <summary>
/// Verifies if the recognized spoken text matches the text parameter.
/// </summary>
/// <param name="text">The text to verify.</param>
/// <returns>true if the text matches.</returns>
bool Recognition::heard(String text)
{
	const char* recognizedText = (const char*) this->openTextBuffer;

	if (recognizedText) {
		int length = this->length[0];
		//recognized open text often ends with a period - do a comparison without.
		const char* ctext = text.c_str();
        return length > 0 && strncmp(ctext, recognizedText, length - (recognizedText[length - 1] == '.')) == 0;
	}

    return false;
}

/// <summary>
/// Verifies if the recognized spoken number matches the number parameter.
/// </summary>
/// <param name="text">The text to verify.</param>
/// <returns>true if the text matches.</returns>
bool Recognition::heard(int spokenNumber) {
	return heard(String(spokenNumber));
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Recognition::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent)
{
	recognizedIndex = shieldEvent->resultId;
	confidence = shieldEvent->value;

	if (this->openTextBuffer)
	{
		int index = 0;
		int max = this->length[0];
		while (shieldEvent->result[index] && index < max)
		{
			this->openTextBuffer[index] = shieldEvent->result[index];
			index++;
		}

		this->length[0] = index;
	}

	Sensor::onJsonReceived(root, shieldEvent);
}
