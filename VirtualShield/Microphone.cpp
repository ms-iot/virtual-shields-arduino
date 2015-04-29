
#include "Microphone.h"
#include "SensorModels.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

const PROGMEM char SERVICE_MICROPHONE[] = "MICROPHONE";
const PROGMEM char AUTOPLAY[] = "Autoplay";
const PROGMEM char KEEP[] = "Keep";

/// <summary>
/// Initializes a new instance of the <see cref="Microphone"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Microphone::Microphone(const VirtualShield &shield) : Sensor(shield, 'U') {
}

/// <summary>
/// Records audio for the specified milliseconds and optionally saves/sends it to a url.
/// </summary>
/// <param name="milliseconds">The length in milliseconds.</param>
/// <param name="url">The URL.</param>
/// <returns>int.</returns>
int Microphone::record(long milliseconds, String url, bool keepLocal, bool autoplay)
{
	EPtr eptrs[] = { EPtr(MS, milliseconds), EPtr(url ? MemPtr : None, URL, url.c_str()),
		EPtr(KEEP, keepLocal, keepLocal ? Bool : None),
		EPtr(AUTOPLAY, autoplay, autoplay ? Bool : None) };
	return shield.block(writeAll(SERVICE_MICROPHONE, eptrs, 4), onEvent == 0);
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Microphone::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent)
{
	Sensor::onJsonReceived(root, shieldEvent);
}