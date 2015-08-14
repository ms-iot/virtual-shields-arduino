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

// Web is the web shield.
#include <Web.h>

// Instantiate the shields.
VirtualShield shield;
Text screen = Text(shield);
Web web = Web(shield);

void webEvent(ShieldEvent* shieldEvent)
{
  const int MAX_RESPONSE = 100;
  char response[MAX_RESPONSE];
  
  screen.printAt(3, "ResponseCode:");
  screen.printAt(4, shieldEvent->resultId);
  
  // Get the response phrase after the parsing instructions parsed
  web.getResponse(response, MAX_RESPONSE);
  
  // Further parse the result, separating from the vertical bar character (|)
  EPtr eptrs[3];
  int parts = EPtr::parse(response, eptrs, 3, '|', 0);
  
  screen.printAt(5, "The weather for:");
  
  String message = eptrs[0].value;
  screen.printAt(7, message.substring(0,eptrs[0].length));
  screen.printAt(8, "for");
  
  message = eptrs[1].value;
  screen.printAt(9, message.substring(0,eptrs[1].length));
  screen.printAt(10, "is");
  
  message = eptrs[2].value;
  screen.printAt(11, message.substring(0,eptrs[2].length));
}

// Callback for startup, reconnection, and when the pushing 'Refresh' button
void refresh(ShieldEvent* shieldEvent)
{
  // clear the screen to a color
  screen.clear();
  
  screen.printAt(1, "Retrieving the forecast");
  
  String url = "http://forecast.weather.gov/MapClick.php?lat=47.6694&lon=-122.1239&FcstType=json";
  String parsingInstructions = "J:location.areaDescription|&^J:time.startPeriodName[0]|&^J:data.weather[0]";
  
  web.get(url, parsingInstructions);
}

void setup()
{
  // Call 'refresh' on startup, on reconnection, and when the pushing 'Refresh' button
  shield.setOnRefresh(refresh);

  // When web has a result, call the webEvent method
  web.setOnEvent(webEvent);

  // Begin the shield communication
  shield.begin();
}

void loop()
{
  // checkSensors() checks for return events and handles them (calling callbacks). This is VirtualShield's single loop() method.
  shield.checkSensors();
}		 
