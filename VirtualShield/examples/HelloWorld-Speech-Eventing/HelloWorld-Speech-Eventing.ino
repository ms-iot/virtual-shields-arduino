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

void recognitionEvent(ShieldEvent* event) 
{
  if (event->resultId > 0) {
	digitalWrite(LED_PIN, recognition.recognizedIndex == 1 ? HIGH : LOW);
	recognition.listenFor("on,off");	    // reset up the recognition after each event
  }
}

// when Bluetooth connects, or the 'Refresh' button is pressed
void refresh(ShieldEvent* event) 
{
        String message = "Hello Virtual Shields. Say the word 'on' or 'off' to affect the LED";
        
	screen.clear();
	screen.print(message);
        speech.speak(message); 

	recognition.listenFor("on,off");	// NON-blocking instruction to recognize speech
}

void setup()
{
	pinMode(LED_PIN, OUTPUT);
	pinMode(LED_PIN, LOW);

	recognition.setOnEvent(recognitionEvent);	// set up a function to handle recognition events (turns auto-blocking off)
        shield.setOnRefresh(refresh);

        // begin() communication - you may specify a baud rate here, default is 115200
	shield.begin();
}

void loop()
{
	shield.checkSensors();		    // handles Virtual Shield events.
}			
