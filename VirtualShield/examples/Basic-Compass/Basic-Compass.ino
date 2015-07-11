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
    About the Compass sensor:
    
    This sensor returns a heading with respect to True North and, possibly, 
    Magnetic North. (The latter is dependent on the sensor capabilities.)
    
    About Virtual Shield sensors:
    
    Virtual Shields uses three methods to track sensors:
    get: this is a one-time retrieval of values, starts and stops immediately.
        Example: compass.get()
    getOnChange: this is a triggered report. Only when any change over the 
        specified delta occurs, does a report get sent. It does not stop until 
        stop() is called.
        Example: compass.getOnChange(0.1); //report 0.1 or greater changes
    start: this starts and continues until stop() is called. Optionally, you
        can specify a delta to prevent smaller changes from being reported.
        Also optionally, you can specify a time interval to prevent too many
        reports too quickly.
        Example: compass.start();
               : compass.start(0.1); //report 0.1 or greater changes
               : compass.start(0.1, 100); //report 0.1 or greater changes,
                 no less than 100 milliseconds apart.
                 
    The only difference between .getOnChange(0.1) and .start(0.1) is that 
    start reports immediately once and waits for changes while getOnChange
    waits for changes before reporting.    
*/

// Include the ArduinoJson library, a dependency.
#include <ArduinoJson.h>

// VirtualShield is the core of all shields.
#include <VirtualShield.h>

// Text is to display text onscreen.
#include <Text.h>

// Compass is for Compass use.
#include <Compass.h>

// Instantiate the shields.
VirtualShield shield;
Text screen = Text(shield);
Compass compass = Compass(shield);

// Callback event for Compass events
void compassEvent(ShieldEvent* shieldEvent)
{
  // If there is a sensor error (errors are negative)... display message
  if (shieldEvent->resultId < 0) {
    screen.printAt(3, "Sensor doesn't exist");
    screen.printAt(4, "or isn't turned on.");
    
    screen.printAt(6, "error: " + String(shieldEvent->resultId));
    return;
  }
  
  String heading = String("Heading: ") + String(compass.Heading);
  
  // Print the sensor values to the screen.
  screen.printAt(3, heading);
}

// Callback for startup, reconnection, and when the pushing 'Refresh' button
void refresh(ShieldEvent* shieldEvent)
{
  screen.clear();
  screen.printAt(1, "Basic Compass");

  // Send compass changes no less than 0.1 delta changes, no less than every 100 milliseconds.
  compass.start(0.1, 100);
}

void setup()
{
  // Call 'refresh' on startup, on reconnection, and when the pushing 'Refresh' button
  shield.setOnRefresh(refresh);
  
  // Begin the shield communication
  shield.begin();

  // When the compass sensor retrieves values, call this function: compassEvent()
  compass.setOnEvent(compassEvent);
}

void loop()
{
  // checkSensors() checks for return events and handles them (calling callbacks). This is VirtualShield's single loop() method.
  // If an compass result returns, the callback of compassEvent() is executed where it displays the values to the screen.
  shield.checkSensors();
}		 
