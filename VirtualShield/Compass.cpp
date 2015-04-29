#include "Compass.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

/// <summary>
/// Initializes a new instance of the <see cref="Compass"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Compass::Compass(const VirtualShield &shield) : Sensor(shield, 'M') {
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Compass::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) {
	Heading = root["Mag"];

	Sensor::onJsonReceived(root, shieldEvent);
}
