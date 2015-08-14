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
    About the Graphics Shield:
    
    Graphics includes, but goes beyond Text to add truetype font (drawAt),
	images, buttons, rectangles, lines, colors and more.
    
*/

// Include the ArduinoJson library, a dependency.
#include <ArduinoJson.h>

// VirtualShield is the core of all shields.
#include <VirtualShield.h>

// Graphics is to display text onscreen.
#include <Graphics.h>

// Colors brings in commonly named colors.
#include <Colors.h>

// Instantiate the shields.
VirtualShield shield;
Graphics screen = Graphics(shield);

int buttonId, rectangleId;

void screenEvent(ShieldEvent* shieldEvent) 
{
  if (screen.isPressed(rectangleId)) 
  {
    screen.change(buttonId, ARGB(255,0,0));
    screen.change(rectangleId, ARGB(255,0,0));
  }
  
  if (screen.isReleased(rectangleId)) 
  {
    screen.change(buttonId, ARGB(0,255,0));
    screen.change(rectangleId, ARGB(0,255,0));
  }
}

// Callback for startup, reconnection, and when the pushing 'Refresh' button
void refresh(ShieldEvent* shieldEvent)
{
  // clear the screen to RED
  screen.clear(ARGB(215,48,34));
  
  screen.drawAt(0,0, "Basic Graphics");
  screen.drawImage(0,50, "https://blog.arduino.cc/wp-content/uploads/2015/04/windowsarduino.jpg", "", 300, 100);
  buttonId = screen.addButton(0, 175, "Button here");
  screen.drawAt(0,225, "Green Rectangle, Yellow Line");
  rectangleId = screen.fillRectangle(0, 265, 100, 50, GREEN);
  screen.line(105,265,300,305, YELLOW);
}

void setup()
{
  // Call 'refresh' on startup, on reconnection, and when the pushing 'Refresh' button
  shield.setOnRefresh(refresh);

  // Add callback for screen events (buttons, images being pressed). Otherwise, those calls are blocked.
  screen.setOnEvent(screenEvent);
  
  // Begin the shield communication
  shield.begin();
}

void loop()
{
  // checkSensors() checks for return events and handles them (calling callbacks). This is VirtualShield's single loop() method.
  shield.checkSensors();
}		 
