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
    About the Web:
   
*/

// Include the ArduinoJson library, a dependency.
#include <ArduinoJson.h>

// VirtualShield is the core of all shields.
#include <VirtualShield.h>

// Text is to display text onscreen.
#include <Text.h>

// Geolocator is for Accelerometer use.
#include <Web.h>

// Instantiate the shields.
VirtualShield shield;
Text screen = Text(shield);
Web web = Web(shield);

// Callback event for Accelerometer events
void webEvent(ShieldEvent* shieldEvent)
{
  // If there is a sensor error (errors are negative)... display message
  if (shieldEvent->resultId < 0) {
    screen.printAt(3, "Web error");
    
    screen.printAt(6, "error: " + String(shieldEvent->resultId));
    return;
  }

  screen.printAt(3, "success");
}

// Callback for startup, reconnection, and when the pushing 'Refresh' button
void refresh(ShieldEvent* shieldEvent)
{
  screen.clear();
  screen.printAt(1, "Basic Web");

  // Send accelerometer changes no less than 0.1 delta changes, no less than every 100 milliseconds.
  web.post("https://api.twilio.com/2010-04-01/Accounts/ACxxx/Messages.json", 
  "Body=TestMessage\r\nTo=+14255551212\r\nFrom=+14255551212\r\n");
}

void setup()
{
  // Call 'refresh' on startup, on reconnection, and when the pushing 'Refresh' button
  shield.setOnRefresh(refresh);
  
  // Begin the shield communication
  shield.begin();

  // When the GPS retrieves values, call this function: gpsEvent()
  accelerometer.setOnEvent(accelerometerEvent);
}

void loop()
{
  // checkSensors() checks for return events and handles them (calling callbacks). This is VirtualShield's single loop() method.
  // If an accelerometer result returns, the callback of accelerometerEvent() is executed where it displays the values to the screen.
  shield.checkSensors();
}		 
