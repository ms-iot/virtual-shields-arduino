/*
    Copyright(c) Microsoft Open Technologies, Inc. All rights reserved.

    The MIT License(MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files(the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions :

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

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
