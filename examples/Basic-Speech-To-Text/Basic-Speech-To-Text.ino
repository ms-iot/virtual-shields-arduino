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

/*
	The Recognition shield listens for specific words or phrases.
*/

// Include the ArduinoJson library, a dependency.
#include <ArduinoJson.h>

// VirtualShield is the core of all shields.
#include <VirtualShield.h>

// Text is to display text onscreen.
#include <Text.h>

// Recognition is the speech recognition shield.
#include <Recognition.h>

// Instantiate the shields.
VirtualShield shield;
Text screen = Text(shield);
Recognition recognition = Recognition(shield);

void recognitionEvent(ShieldEvent* shieldEvent)
{
  // the shieldEvent->resultId has the index of the recognized word. for "yes,no" then yes=1 and no=2.
  // for groupings such as "yes=yes,yeah,sure,ok;no=no,no way,nah,nope" then any yes word (sure) =1 and any no word (no way) =2
  
  if (shieldEvent->resultId < 0) {
    screen.printAt(1, "Unknown error-please refresh");
    return;
  }
  
  screen.printAt(1, "Heard:");
  if (shieldEvent->resultId == 1) {
    screen.printAt(2, "yes");
  } else if (shieldEvent->resultId == 2) {
    screen.printAt(2, "no");
  } else if (shieldEvent->resultId == 0) {
    screen.printAt(2, "(nothing)");
  }
  
  screen.printAt(4, "refresh to try again");
}

// Callback for startup, reconnection, and when the pushing 'Refresh' button
void refresh(ShieldEvent* shieldEvent)
{
  // clear the screen to a color
  screen.clear();

  screen.printAt(1, "Say Yes or No");

  // for single words, separate with commas: "yes,no" 
  // for groupings use semicolons: "yes=yes,yeah,sure,ok;no=no,no way,nah,nope" 
  // see the recognitionEvent for resultId details
  recognition.listenFor("yes,no");
}

void setup()
{
  // Call 'refresh' on startup, on reconnection, and when the pushing 'Refresh' button
  shield.setOnRefresh(refresh);

  // When recognition has a result, call the recognitionEvent method
  recognition.setOnEvent(recognitionEvent);

  // Begin the shield communication
  shield.begin();
}

void loop()
{
  // checkSensors() checks for return events and handles them (calling callbacks). This is VirtualShield's single loop() method.
  shield.checkSensors();
}		 
