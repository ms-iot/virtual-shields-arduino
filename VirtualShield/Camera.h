
#ifndef Camera_h
#define Camera_h

#include "Sensor.h"

class Camera : public Sensor
{
public:
	Camera(const VirtualShield &shield);

	int enablePreview(bool enable);
	int capture(String url = (const char*) 0);

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif