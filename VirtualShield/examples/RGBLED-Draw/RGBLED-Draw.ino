#include <ArduinoJson.h>

#include <VirtualShield.h>
#include <Graphics.h>
#include <Colors.h>

#include "RGBGrid.h"

VirtualShield shield;
Graphics screen = Graphics(shield);

const int gridcolumns = 8;
const int gridrows = 6;

RGBGrid strip = RGBGrid(gridcolumns, gridrows);

const int width = 480;
const int height = 340;

const int totalcolumns = 10;
const int totalrows = 7;

const int cwidth = width/totalcolumns;
const int rheight = height/totalrows;

const int cellwidth = cwidth * 0.8;
const int cellheight = rheight * 0.8;

int ids[gridcolumns][gridrows];
ARGB colors[gridcolumns][gridrows];

const int colorCount = 8;
ARGB selectableColors[colorCount] = {BLACK, RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, WHITE};
int colorIds[colorCount];
ARGB selectedColor = ARGB(0x222222);

const int colorRowHeight = (height / colorCount);
const int colorCellHeight = colorRowHeight * 0.6;
const int selectedColorCellHeight = colorRowHeight * 0.8;

void screenEvent(ShieldEvent* shieldEvent)
{
  char tag[3];
  strncpy(tag, shieldEvent->tag, 3);
  if (tag[1] == 0 && tag[0] < 48+colorCount && tag[0] >= 48) {
    //color
    selectedColor = ARGB(selectableColors[tag[0]-48]);
  } else if (tag[0] != 0 && tag[2] == 0) {
    //grid
    int x = tag[0]-48;
    int y = tag[1]-48;
    
    colors[x][y] = selectedColor;
    strip.setPixel(x,y,selectedColor);
  }
}

void drawGrid() 
{
  for (int x=0; x<gridcolumns; x++) {
    for (int y=0; y<gridrows; y++) {
      char tag[3] = {48+x, 48+y, 0};
      ids[x][y] = screen.fillRectangle(cwidth * x + (cwidth/2 - cellwidth/2), rheight * y + (rheight/2 - cellheight/2),
        cellwidth, cellheight, colors[x][y], String(tag));  
    }
  }
  
  for (int i=0; i<colorCount-1; i++) {
    colorIds[i] = screen.fillRectangle(cwidth * (totalcolumns-1) + (cwidth/2 - cellwidth/2), colorRowHeight*(i+.5) - colorCellHeight/2,
      cellwidth, colorCellHeight, selectableColors[i], String(i));
  }
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
  screen.setOnEvent(screenEvent);

  for (int x=0; x<gridcolumns; x++) {
    for (int y=0; y<gridrows; y++) {
      colors[x][y] = 0x222222;
    }
  }

  strip.begin();
  shield.begin();
}


void loop()
{
  shield.checkSensors();
}		 