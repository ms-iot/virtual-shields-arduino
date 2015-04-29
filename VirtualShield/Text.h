
#ifndef Text_h
#define Text_h

#include "Sensor.h"

const PROGMEM char Y[] = "Y";
const PROGMEM char CLEAR[] = "CLEAR";
const PROGMEM char RGBAKEY[] = "ARGB";
const PROGMEM char PID[] = "Pid";

class Text : public Sensor
{
public:
    Text(const VirtualShield &shield);

	int clear(ARGB argb = 0);
	int clearLine(UINT line);
	int clearId(UINT id);

	int print(String text, ARGB argb = 0);
	int printAt(UINT line, String text, Attr extraAttributes[] = 0, int extraAttributeCount = 0);
	int printAt(UINT line, EPtr text, Attr extraAttributes[] = 0, int extraAttributeCount = 0);
	int printAt(UINT line, double value, ARGB argb = 0);

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif