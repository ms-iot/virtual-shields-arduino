#include "Vibrate.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

const PROGMEM char SERVICE_VIBRATE[] = "VIBRATE";

/// <summary>
/// Initializes a new instance of the <see cref="Vibrate"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Vibrate::Vibrate(const VirtualShield &shield) : Sensor(shield, 'V') {
}

/// <summary>
/// Vibrates the device for the specified milliseconds.
/// </summary>
/// <param name="milliseconds">The length to vibrate the device in milliseconds.</param>
/// <returns>int.</returns>
int Vibrate::vibrate(int milliseconds) {
	EPtr eptrs[] = { EPtr(MS, milliseconds) };
	return writeAll(SERVICE_VIBRATE, eptrs, 1);
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Vibrate::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) {
	Sensor::onJsonReceived(root, shieldEvent);
}
