
#include "Sensor.h"
#include "Media.h"
#include "SensorModels.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

const PROGMEM char SERVICE_PLAY[] = "PLAY";

/// <summary>
/// Initializes a new instance of the <see cref="Camera"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Media::Media(const VirtualShield &shield) : Sensor(shield, 'E') {
}


/// <summary>
/// Plays the specified media specified by the URL.
/// </summary>
/// <param name="url">The URL.</param>
/// <returns>int.</returns>
int Media::play(String url, long length)
{
	EPtr eptrs[] = { EPtr(MemPtr, URL, url.c_str()), EPtr(MS, length, length ? Long : None) };
	return shield.block(writeAll(SERVICE_PLAY, eptrs, 2), onEvent == 0);
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Media::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent)
{
	Sensor::onJsonReceived(root, shieldEvent);
}