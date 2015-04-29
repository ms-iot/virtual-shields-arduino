#include <ArduinoJson.h>

#include <VirtualShield.h>
#include <Graphics.h>
#include <Recognition.h>
#include <Colors.h>

VirtualShield shield;
Graphics screen = Graphics(shield);
Recognition speech = Recognition(shield);

int redPin = 10;
int yellowPin = 9;
int greenPin = 8;

int redButtonId, yellowButtonId, greenButtonId;

void checkLed(int id, ShieldEvent* event, int pin)
{
	if (screen.isPressed(id, event))
	{
		digitalWrite(pin, HIGH);
	}

	if (screen.isReleased(id, event))
	{
		digitalWrite(pin, LOW);
	}
}

// A screen event occurred (rectangle was pressed)
void screenEvent(ShieldEvent* event)
{
	checkLed(redButtonId, event, redPin);
	checkLed(yellowButtonId, event, yellowPin);
	checkLed(greenButtonId, event, greenPin);
}

// Refresh event callback
void refresh(ShieldEvent* event)
{
	screen.clear();

	digitalWrite(redPin, LOW);
	digitalWrite(yellowPin, LOW);
	digitalWrite(greenPin, LOW);

        // Create screen buttons, saving the ids for checking later
	redButtonId = screen.fillRectangle(120, 0, 70, 70, ARGB(RED));
	yellowButtonId = screen.fillRectangle(120, 80, 70, 70, ARGB(YELLOW));
	greenButtonId = screen.fillRectangle(120, 160, 70, 70, ARGB(GREEN));

        // Listen for 
	speech.listenFor("green,yellow,red,off", false);
}

void speechEvent(ShieldEvent* event)
{
	digitalWrite(redPin, speech.recognizedIndex == 3 ? HIGH : LOW);
	digitalWrite(yellowPin, speech.recognizedIndex == 2 ? HIGH : LOW);
	digitalWrite(greenPin, speech.recognizedIndex == 1 ? HIGH : LOW);

	speech.listenFor("green,yellow,red,off", false);
}

void setup()
{
	// set an event handler for screen events (turns off blocking for all screen functions)
	screen.setOnEvent(screenEvent);

	// set an event handler for speech events (turns off blocking for all speech functions)
	speech.setOnEvent(speechEvent);

	// set a 'Refresh' event handler for when any of these events occur:
	// (1) connect/reconnection occurs with Bluetooth, 
	// (2) the app resumes, 
	// or (3) the user clicks the 'Refresh' toolbar button.
	shield.setOnRefresh(refresh);

	pinMode(redPin, OUTPUT);
	pinMode(yellowPin, OUTPUT);
	pinMode(greenPin, OUTPUT);

    // begins Virtual Shield communication
	shield.begin();
}

void loop()
{
	shield.checkSensors();
}
