
#include "Notification.h"
#include "SensorModels.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

const PROGMEM char SERVICE_NOTIFICATION[] = "NOTIFY";
const PROGMEM char TOAST[] = "Toast";
const PROGMEM char TILE[] = "Tile";

/// <summary>
/// Initializes a new instance of the <see cref="Microphone"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Notification::Notification(const VirtualShield &shield) : Sensor(shield, 'N') {
}

/// <summary>
/// Send/Show a toast notification.
/// </summary>
/// <param name="milliseconds">The length in milliseconds.</param>
/// <param name="url">The URL.</param>
/// <returns>int.</returns>
int Notification::toast(String message, String tag, String image, String audio)
{
	EPtr eptrs[] = { EPtr(MemPtr, MESSAGE, message.c_str()), EPtr(tag ? MemPtr : None, TAG, tag.c_str()), 
		EPtr(image ? MemPtr : None, IMAGE, image.c_str()), EPtr(audio ? MemPtr : None, AUDIO, audio.c_str()), 
		EPtr(ACTION, TOAST) };
	return writeAll(SERVICE_NOTIFICATION, eptrs, 5);
}

/// <summary>
/// Send/Show a toast notification.
/// </summary>
/// <param name="milliseconds">The length in milliseconds.</param>
/// <param name="url">The URL.</param>
/// <returns>int.</returns>
int Notification::tile(String message, String tag, String image)
{
	EPtr eptrs[] = { EPtr(MemPtr, MESSAGE, message.c_str()), EPtr(tag ? MemPtr : None, TAG, tag.c_str()),
		EPtr(image ? MemPtr : None, IMAGE, image.c_str()), EPtr(ACTION, TILE) };
	return writeAll(SERVICE_NOTIFICATION, eptrs, 4);
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Notification::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent)
{
	Sensor::onJsonReceived(root, shieldEvent);
}