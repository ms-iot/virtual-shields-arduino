
#include "Sensor.h"
#include "Camera.h"
#include "SensorModels.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

const PROGMEM char SERVICE_CAMERA[] = "CAMERA";
const PROGMEM char PREVIEW[] = "PREVIEW";

/// <summary>
/// Initializes a new instance of the <see cref="Camera"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Camera::Camera(const VirtualShield &shield) : Sensor(shield, 'Z') {
}

/// <summary>
/// Enables the camera preview.
/// </summary>
/// <param name="enable">true to enable, false to disable.</param>
/// <returns>int.</returns>
int Camera::enablePreview(bool enable)
{
	EPtr eptrs[] = { EPtr(ACTION, enable ? ENABLE : DISABLE), EPtr(MESSAGE, PREVIEW) };
	return writeAll(SERVICE_CAMERA, eptrs, 2);
}

/// <summary>
/// Captures a picture and optionally saves/sends it to the specified URL.
/// </summary>
/// <param name="url">The URL.</param>
/// <returns>int.</returns>
int Camera::capture(String url)
{
	EPtr eptrs[] = { EPtr(url ? MemPtr : None, URL, url.c_str()) };
	return shield.block(writeAll(SERVICE_CAMERA, eptrs, 1), onEvent == 0);
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Camera::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent)
{
	Sensor::onJsonReceived(root, shieldEvent);
}