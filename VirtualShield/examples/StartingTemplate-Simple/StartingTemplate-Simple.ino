#include <ArduinoJson.h>

#include <VirtualShield.h>
#include <Text.h>

VirtualShield shield;
Text screen = Text(shield);

void refresh(ShieldEvent* shieldEvent)
{
   // put your refresh code here, which runs when Bluetooth connects or the 'refresh' button is pressed in the app:
   screen.clear();
   screen.printAt(1, "Hello Virtual Shields");
}

void setup() 
{
   // set up virtual shield events: 
   shield.setOnRefresh(refresh);
   shield.begin(); //assumes 115200 Bluetooth baudrate

   // put your setup code here, to run once:
}

void loop() 
{
   shield.checkSensors();

   // put your main code here, to run repeatedly:
}
