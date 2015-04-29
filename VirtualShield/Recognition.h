#ifndef Recognition_h
#define Recognition_h

#include "Sensor.h"
#include "ShieldEvent.h"
#include <ArduinoJson.h>

#include "Arduino.h"

const int Confidence_High = 0;
const int Confidence_Medium = 1;
const int Confidence_Low = 2;
const int Confidence_Rejected = 3;
const int Confidence_Error = 5;

class Recognition : public Sensor {
public:
	int recognizedIndex = 0;
	int confidence;

	Recognition(const VirtualShield &shield);

    int listen(char* openTextBuffer, int* length, bool useUI = true, long timeout = 0);

    int listenFor(String constraint, bool useUI = true, int expectedConfidence = Confidence_Medium, long timeout = 0);
    int listenFor(const char* constraint, bool useUI = true, int expectedConfidence = Confidence_Medium, long timeout = 0);
    int listenFor(EPtr constraint, bool useUI = true, int expectedConfidence = Confidence_Medium, long timeout = 0);

	int stop() override;

	bool heard(String text);
	bool heard(int spokenNumber);

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
private:
	char* openTextBuffer;
	int* length;
};

#endif