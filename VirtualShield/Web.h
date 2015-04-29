
#ifndef Web_h
#define Web_h

#include "Sensor.h"

const PROGMEM char PARSE[] = "Parse";

namespace ArduinoJson{
	class JsonObject;
}

const int maxResponseLength = 200;

class Web : public Sensor
{
public:
	int resultId;

	Web(const VirtualShield &shield);

	int get(String url, String parsingInstructions = (const char*) 0, int maxLength = 0);
	int post(String url, String data, String parsingInstructions = (const char*) 0, int maxLength = 0);

	int get(EPtr url, EPtr parsingInstructions, int maxLength = 0);
	int post(EPtr url, EPtr data, EPtr parsingInstructions, int maxLength = 0);

	void getResponse(char* responseBuffer, int length, char** parts = 0, int partCount = 0);

	void onJsonReceived(ArduinoJson::JsonObject& root, ShieldEvent* shieldEvent) override;
private:
	const char* response;
};

#endif