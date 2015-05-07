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
#include <Text.h>

VirtualShield shield;
Text screen = Text(shield);

void refresh(ShieldEvent* shieldEvent)
{
   // put your refresh code here
   // this runs whenever Bluetooth connects, whenever the shield.begin() executes, or the 'refresh' button is pressed in the app:
   screen.clear();
   screen.printAt(1, "Hello Virtual Shields");
}

void setup() 
{
   // set up virtual shield events: 
   shield.setOnRefresh(refresh);
   
   // begin the shield communication (also calls refresh()).
   shield.begin(); //assumes 115200 Bluetooth baudrate

   // put your setup code here, to run once:
}

void loop() 
{
   shield.checkSensors();

   // put your main code here, to run repeatedly:
}
