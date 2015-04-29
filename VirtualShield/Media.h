
#ifndef Media_h
#define Media_h

#include "Sensor.h"

class Media : public Sensor
{
public:
	Media(const VirtualShield &shield);

	int play(String url, long length = 0);

	int playVideo(String url, long length = 0) {
		return play(String("VIDEOS:" + url), length);
	}

	int playAudio(String url, long length = 0) {
		return playVideo(url, length);
	}

	void onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) override;
};

#endif