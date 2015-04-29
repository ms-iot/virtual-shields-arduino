#include <ArduinoJson.h>

#include <VirtualShield.h>
#include <Text.h>

VirtualShield shield;	          // identify the shield
Text screen = Text(shield);	  // connect the screen

void setup()
{
    // begin Virtual Shields for Arduino communication. You may specify a baud rate as a parameter, default is 115200
	shield.begin();

    // clears the screen
	screen.clear();

	screen.printAt(0, "Hello Virtual Shields");	 
}

void loop()
{
}								
