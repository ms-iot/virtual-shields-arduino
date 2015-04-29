
#include "Geolocator.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

/// <summary>
/// Initializes a new instance of the <see cref="Geolocator"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Geolocator::Geolocator(const VirtualShield &shield) : Sensor(shield, 'L') {
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Geolocator::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) {
	Latitude = root["Lat"];
	Longitude = root["Lon"];
	Altitude = root["Alt"];

	Sensor::onJsonReceived(root, shieldEvent);
}
