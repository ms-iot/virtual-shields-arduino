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
    About the Media Shield:
    
    Media provides video or audio playback with the ability to start, stop,
    and receive notifications when playback has started or finished.
    
    Media provides these example methods:
    play("http://...something.mp4") - play a video
    play("http://...something.wav") - play audio
*/

// Include the ArduinoJson library, a dependency.
#include <ArduinoJson.h>

// VirtualShield is the core of all shields.
#include <VirtualShield.h>

// Text is to display text onscreen.
#include <Text.h>

// Media is to play video and audio
#include <Media.h>

// Instantiate the shields.
VirtualShield shield;
Text screen = Text(shield);
Media media = Media(shield);

void mediaEvent(ShieldEvent* shieldEvent) 
{
  if (shieldEvent->resultId < 0) 
  {
    screen.printAt(3, "a playback error occurred");
  }
}

// Callback for startup, reconnection, and when the pushing 'Refresh' button
void refresh(ShieldEvent* shieldEvent)
{
  // clear the screen to a color
  screen.clear();
  
  // draw text (not LCD looking)
  screen.printAt(1, "Basic Media");
  
  media.play("https://content4.ssl.catalog.video.msn.com/e2/ds/80c9258f-4f69-463a-9adc-1b95a094f8d1.mp4");
}

void setup()
{
  // Call 'refresh' on startup, on reconnection, and when the pushing 'Refresh' button
  shield.setOnRefresh(refresh);

  // Add callback for screen events (buttons, images being pressed). Otherwise, those calls are blocked.
  media.setOnEvent(mediaEvent);
  
  // Begin the shield communication
  shield.begin();
}

void loop()
{
  // checkSensors() checks for return events and handles them (calling callbacks). This is VirtualShield's single loop() method.
  shield.checkSensors();
}		 
