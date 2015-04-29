#include <ArduinoJson.h>

#include <VirtualShield.h>
#include <Text.h>
#include <Speech.h>
#include <Recognition.h>

VirtualShield shield;	          // identify the shield
Text screen = Text(shield);	  // connect the screen
Speech speech = Speech(shield);	  // connect text to speech
Recognition recognition = Recognition(shield);	  // connect speech to text

int LED_PIN = 8;

void setup()
{
	pinMode(LED_PIN, OUTPUT);

	// begin Virtual Shields for Arduino communication. You may specify a baud rate as a parameter, default is 115200
	shield.begin();

	// clears the screen
	screen.clear();

        String message = "Hello Virtual Shields for Arduino. Say the word 'on' or 'off' to change the LED";
        
	screen.print(message);
        speech.speak(message);
        delay(3000);
}

void loop()
{
	// blocks until a word is recognized, or times out
	if (recognition.listenFor("on,off", false))	
	{
		digitalWrite(LED_PIN, recognition.recognizedIndex == 1 ? HIGH : LOW);
	}
}