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

// Include the ArduinoJson library, a dependency.
#include <ArduinoJson.h>

// VirtualShield is the core of all shields.
#include <VirtualShield.h>

// Text is to display text onscreen.
#include <Text.h>

// Geolocator is for GPS use.
#include <Geolocator.h>

// Instantiate the shields.
VirtualShield shield;
Text screen = Text(shield);
Geolocator gps = Geolocator(shield);

// Callback event for GPS events
void gpsEvent(ShieldEvent* shieldEvent)
{
  // If there is a sensor error (errors are negative)... display message
  if (shieldEvent->resultId < 0) {
    screen.printAt(3, "Sensor doesn't exist");
    screen.printAt(4, "or isn't turned on.");
    
    screen.printAt(6, "error: " + String(shieldEvent->resultId));
    return;
  }
  
  String lat = String("Lat: ") + String(gps.Latitude);
  String lon = String("Lon: ") + String(gps.Longitude);
  
  // Print the latitude and longitude to the screen.
  screen.printAt(3, lat);
  screen.printAt(4, lon);
}

void setup()
{
  // Begin the shield communication
  shield.begin();

  // Connect the events to the shields.
  screen.clear();
  screen.printAt(1, "Basic GPS Lookup");

  // When the GPS retrieves values, call this function: gpsEvent()
  gps.setOnEvent(gpsEvent);
}

const long interval = 1000 * 15; //15 seconds;
long nextGPS = 0;

void loop()
{
  // Check GPS coordinates once per 'interval'
  if (millis() > nextGPS) {
    nextGPS = millis() + interval;
    
    // Get the GPS coordinates (send the event to request GPS).
    gps.get();
  }

  // checkSensors() checks for return events and handles them (calling callbacks). This is VirtualShield's single loop() method.
  // If a gps result returns, the callback of gpsEvent() is executed where it displays the values to the screen.
  shield.checkSensors();
}		 
