#include "Accelerometer.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

/// <summary>
/// Initializes a new instance of the <see cref="Accelerometer"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Accelerometer::Accelerometer(const VirtualShield &shield) : Sensor(shield, 'A') {
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Accelerometer::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) {
	X = root["X"];
	Y = root["Y"];
	Z = root["Z"];

	Sensor::onJsonReceived(root, shieldEvent);
}
