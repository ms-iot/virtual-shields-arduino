#include <ArduinoJson.h>

#include <VirtualShield.h>

#include <Text.h>
#include <Recognition.h>

VirtualShield shield;

Text screen = Text(shield);
Recognition speech = Recognition(shield);

int LOCK_PIN = 3;

void setup()
{
	// begin Virtual Shields for Arduino communication. You may specify a baud rate as a parameter, default is 115200
	shield.begin();

	pinMode(LOCK_PIN, OUTPUT);
	digitalWrite(LOCK_PIN, HIGH);
}

bool askToUnlock() {
	int num1 = random(5, 10);
	int num2 = random(5, 10);

	int answer = num1 * num2;

	String question = String(String(num1) + " x " + String(num2));

	screen.clear();
	screen.printAt(1, "Math Door Lock");

	screen.printAt(3, "To unlock the door, Answer: what is...");
	screen.printAt(6, question);

	char recognizedTextBuffer[10];
	int length = 10;
	
	// blocks until anything is recognized
	if (speech.listen(recognizedTextBuffer, &length, false)) {
		screen.printAt(8, String("You said:" + String(recognizedTextBuffer)));

		if (speech.heard(answer)) {
			digitalWrite(LOCK_PIN, LOW);
			screen.printAt(10, "DOOR UNLOCKED!");
			return true;
		}
		else 
		{
			screen.printAt(10, "oops, please try again in a moment");
		}
	}
	else
	{
		screen.printAt(10, "I didn't understand. Try again in a moment");
	}

	return false;
}

bool isLocked = true;

void loop()
{
	while (isLocked) {
		isLocked = !askToUnlock();
		delay(5000);
	}
}
