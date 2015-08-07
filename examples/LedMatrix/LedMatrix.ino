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
#include <Graphics.h>
#include <Accelerometer.h>
#include <Recognition.h>
#include <Web.h>

#include <SPI.h>
#include "RGBStripMatrix.h"

VirtualShield shield;
Accelerometer sensor = Accelerometer(shield);
Graphics screen = Graphics(shield);
Web web = Web(shield);
Recognition speech = Recognition(shield);

int goButton;
int stopButton;
int colorButton;

RGBStripMatrix ledMatrix;

// Color the dot will be drawn as
ARGB dotColor = ARGB("#FFFFFF");

// Delay in ms between sensor updates
int sensorUpdateDelay = 250;

void setup() {
  sensor.setOnEvent(onSensor);

  shield.setOnRefresh(refresh);
  web.setOnEvent(webEvent);
  speech.setOnEvent(onSpeech);
  screen.setOnEvent(screenEvent);

  shield.begin(115200);
}

void onSensor(ShieldEvent* shieldEvent) {
    ledMatrix.makeDot(dotColor, sensor.X, sensor.Y);
}

void refresh(ShieldEvent* shieldEvent) {  
    screen.clear();

    goButton = screen.addButton(0,300,"Go");
    stopButton = screen.addButton(100,300,"Stop");
    colorButton = screen.addButton(200,300,"Color");
    
    screen.printAt(2, "Looking up colors on wikipedia...");
    String parse = "i:.//table[1]/tr|k:td[2]|v:td[3]|table:name=colors";
    web.get("http://en.wikipedia.org/wiki/List_of_Crayola_crayon_colors", parse);

    ledMatrix.begin();
}

void webEvent(ShieldEvent* shieldEvent) {
  if (shieldEvent->resultId >= 0) {
    screen.printAt(4, "...parsed " + String(shieldEvent->resultId) + " colors!");
  } else {
    screen.printAt(2, "web error: " + String(shieldEvent->resultId));
  }
}

void onSpeech(ShieldEvent* shieldEvent) {
  screen.printAt(5, shieldEvent->resultId);
  screen.printAt(6, shieldEvent->result);
  screen.printAt(7, shieldEvent->action);

  dotColor = ARGB(shieldEvent->action);
}

void screenEvent(ShieldEvent* shieldEvent) {
    if (shieldEvent->id == goButton) {
        dotColor = ARGB("#FFFFFF");
        sensor.start(0, sensorUpdateDelay);
    } else if (shieldEvent->id == stopButton) {
        dotColor = ARGB("#000000");
        sensor.stop();
    } else if (screen.isPressed(colorButton, shieldEvent)) {
        speech.listenFor(String("{table:colors}"), true);
    }
}

void loop() {
  shield.checkSensors();
}
