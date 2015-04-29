
#include "LightSensor.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

/// <summary>
/// Initializes a new instance of the <see cref="LightSensor"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
LightSensor::LightSensor(const VirtualShield &shield) : Sensor(shield, 'P') {
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void LightSensor::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) {
	Lux = root["Lux"];

	Sensor::onJsonReceived(root, shieldEvent);
}
