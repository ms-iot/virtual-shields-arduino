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
    About the Accelerometer:
    
    An Accelerometer measures the acceleration (or gain of speed) of a device 
    in three axes: X (left-right), Y (up-down) and Z (back-forth).
    
    Direction is represented by the value being negative (i.e. -X = left) or 
    positive (i.e. +X = right).
    
    Due to gravity, accelerometers are often used to determine the orientation
    of a device.
    
    About Virtual Shield sensors:
    
    Virtual Shields uses three methods to track sensors:
    get: this is a one-time retrieval of values, starts and stops immediately.
        Example: accelerator.get()
    getOnChange: this is a triggered report. Only when any change over the 
        specified delta occurs, does a report get sent. It does not stop until 
        stop() is called.
        Example: accelerator.getOnChange(0.1); //report 0.1 or greater changes
    start: this starts and continues until stop() is called. Optionally, you
        can specify a delta to prevent smaller changes from being reported.
        Also optionally, you can specify a time interval to prevent too many
        reports too quickly.
        Example: accelerator.start();
               : accelerator.start(0.1); //report 0.1 or greater changes
               : accelerator.start(0.1, 100); //report 0.1 or greater changes,
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

// Geolocator is for Accelerometer use.
#include <Accelerometer.h>

// Instantiate the shields.
VirtualShield shield;
Text screen = Text(shield);
Accelerometer accelerometer = Accelerometer(shield);

// Callback event for Accelerometer events
void accelerometerEvent(ShieldEvent* shieldEvent)
{
  // If there is a sensor error (errors are negative)... display message
  if (shieldEvent->resultId < 0) {
    screen.printAt(3, "Sensor doesn't exist");
    screen.printAt(4, "or isn't turned on.");
    
    screen.printAt(6, "error: " + String(shieldEvent->resultId));
    return;
  }
  
  String x = String("X: ") + String(accelerometer.X);
  String y = String("Y: ") + String(accelerometer.Y);
  String z = String("Z: ") + String(accelerometer.Z);
  
  // Print the sensor values to the screen.
  screen.printAt(3, x);
  screen.printAt(4, y);
  screen.printAt(5, z);
}

// Callback for startup, reconnection, and when the pushing 'Refresh' button
void refresh(ShieldEvent* shieldEvent)
{
  screen.clear();
  screen.printAt(1, "Basic Accelerometer");

  // Send accelerometer changes no less than 0.1 delta changes, no less than every 100 milliseconds.
  accelerometer.start(0.1, 100);
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
