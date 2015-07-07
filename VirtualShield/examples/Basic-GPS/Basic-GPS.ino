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

// Include the VirtualShield and used shields (Text, Geolocator)
#include <VirtualShield.h>
#include <Text.h>
#include <Geolocator.h>

// Instantiate the shields.
VirtualShield shield;
Text screen = Text(shield);
Geolocator gps = Geolocator(shield);

// Callback event for screen commands
void screenEvent(ShieldEvent* shieldEvent) 
{
}

// Callback event for gps events
void gpsEvent(ShieldEvent* shieldEvent)
{
  String lat = String("Lat: ") + String(gps.Latitude);
  String lon = String("Lon: ") + String(gps.Longitude);
  
  screen.printAt(3, lat);
  screen.printAt(4, lon);
}

// Callback event for pushing 'Refresh', or starting up, or reconnecting
void refresh(ShieldEvent* shieldEvent)
{
  screen.clear();
  screen.printAt(1, "Basic GPS Lookup");
}

void setup()
{
  // Connect the events to the shields.
  shield.setOnRefresh(refresh);
  gps.setOnEvent(gpsEvent);
  screen.setOnEvent(screenEvent);

  // Begin the shield communication
  shield.begin();
}

const long interval = 1000 * 15; //15 seconds;
long nextGPS = 0;

void loop()
{
  // Check GPS coordinates once per 'interval'
  if (millis() > nextGPS) {
    nextGPS = millis() + interval;
    gps.get();
  }

  // Allow for shield communications.
  shield.checkSensors();
}		 
