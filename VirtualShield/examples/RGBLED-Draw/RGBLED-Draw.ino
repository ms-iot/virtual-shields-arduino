#include <ArduinoJson.h>

#include <VirtualShield.h>
#include <Graphics.h>

#include "RGBStrip.h"

VirtualShield shield;
Graphics screen = Graphics(shield);

RGBStrip strip;

void drawGrid() 
{
  for (int x=0; x<
}

void refresh(ShieldEvent* shieldEvent)
{
  screen.orientation(Orientation_Landscape);
  screen.clear();
  drawGrid();
}

void setup()
{
  shield.setOnRefresh(refresh);

  strip.begin();
  shield.begin();
}


void loop()
{
  shield.checkSensors();
}		 
