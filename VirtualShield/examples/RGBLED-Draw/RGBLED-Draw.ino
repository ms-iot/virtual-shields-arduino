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

ARGB colors[gridcolumns][gridrows];

const int colorCount = 8;
ARGB selectableColors[colorCount] = {BLACK, RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, WHITE};
ARGB lightBlack = ARGB(0x222222);
ARGB selectedColor = lightBlack;

const int colorRowHeight = (height / colorCount) * 0.9;
const int colorCellHeight = colorRowHeight * 0.6;
const int selectedColorCellHeight = colorRowHeight * 0.8;
const int tappedHash = 38322;
const int pressedHash = 56990; 
const int releasedHash = 13117;
const int enteredHash = 54231;

int ids[gridcolumns][gridrows];
int touchX, touchY;
int pressedX, pressedY, releasedX, releasedY;
int clearId = 0;

ARGB visibleColor(ARGB color)
{
  return color.color == BLACK ? lightBlack : color;
}

void setColor(int x, int y, ARGB color, bool shift = true) 
{
    colors[x][y] = color;
    strip.setPixel(x,y,color);
    if (shift) {
      strip.ShiftAllPixels();
    }
    
    if (false) { //ids[x][y] > 0) {
      screen.change(ids[x][y], visibleColor(colors[x][y].color));
    } else {
      char tag[3] = {48+x, 48+y, 0};
      ids[x][y] = screen.fillRectangle(cwidth * x + (cwidth/2 - cellwidth/2), rheight * y + (rheight/2 - cellheight/2),
        cellwidth, cellheight, visibleColor(colors[x][y].color), String(tag), true);
    }
}

void drawGrid() 
{
  screen.clear();
  
  // draw colors
  for (int i=0; i<colorCount; i++) {
    screen.fillRectangle(cwidth * (totalcolumns-2) + (cwidth/2 - cellwidth/2), colorRowHeight*(i+.5) - colorCellHeight/2,
      cellwidth, colorCellHeight, visibleColor(selectableColors[i]), String(i));
  }

  for (int x=0; x<gridcolumns; x++) {
    for (int y=0; y<gridrows; y++) {
      setColor(x,y,colors[x][y],false);
    }
  }
  
  strip.ShiftAllPixels();

  clearId = screen.fillRectangle(cwidth * (totalcolumns-1), 0, cwidth * totalcolumns, height, visibleColor(selectedColor), String("8"));
}

void clearGrid(bool firstTime = false) 
{
  strip.clear();
  
  for (int x=0; x<gridcolumns; x++) {
    for (int y=0; y<gridrows; y++) {
      if (firstTime) {
        ids[x][y] = 0;
      }
      colors[x][y] = selectedColor;
    }
  }
  
  drawGrid();
}

void screenEvent(ShieldEvent* shieldEvent)
{
  int x,y;
  char tag[3];
  strncpy(tag, shieldEvent->tag, 3);

  if (tag[0] != 0 && tag[1] != 0 && tag[2] == 0) {
      //grid
      x = tag[0]-48;
      y = tag[1]-48;
  }
  
  if (shieldEvent->actionHash == tappedHash || shieldEvent->actionHash == enteredHash) {
    if (tag[1] == 0 && tag[0] < 48+(colorCount+1) && tag[0] >= 48) {
      //color
      if (tag[0] == 48+(colorCount)) {
        //clear all
        clearGrid();
      } else {
        selectedColor = ARGB(selectableColors[tag[0]-48]);
        screen.clearId(clearId);
        clearId = screen.fillRectangle(cwidth * (totalcolumns-1), 0, cwidth * totalcolumns, height, visibleColor(selectedColor), String("8"));
      }
    } else if (tag[0] != 0 && tag[2] == 0) {
      //grid
      setColor(x,y,selectedColor);
    }
  }
}

void refresh(ShieldEvent* shieldEvent)
{
  screen.orientation(Orientation_Landscape);
  drawGrid();
}

void setup()
{
  for (int x=0; x<gridcolumns; x++) {
    for (int y=0; y<gridrows; y++) {
      ids[x][y] = 0;
    }
  }
  
  shield.setOnRefresh(refresh);
  screen.setOnEvent(screenEvent);

  selectedColor = BLACK;
  clearGrid(true);

  strip.begin();
  shield.begin();
}


void loop()
{
  shield.checkSensors();
}		 