#include "Orientation.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

/// <summary>
/// Initializes a new instance of the <see cref="Orientation"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Orientation::Orientation(const VirtualShield &shield) : Sensor(shield, 'Q') {
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Orientation::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) {
	X = root["X"];
	Y = root["Y"];
	Z = root["Z"];
	W = root["W"];

	Sensor::onJsonReceived(root, shieldEvent);
}
