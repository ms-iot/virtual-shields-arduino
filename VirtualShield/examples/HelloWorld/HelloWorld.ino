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

      int y = 0;
      while(1)
      {
	screen.printAt(y, "Hello Virtual Shields");	 
        y++;
        if (y > 10)
        {
          y = 0;
        }
        delay(1000);
      }
}

void loop()
{
}								