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
int selectedColorIndex = 0;

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
int visibleHeight = height * 0.85;
int text1, text2, text3, text4, clearId2;

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


void drawClear() 
{
  if (clearId > 0) {
    screen.clearId(clearId);
    screen.clearId(text1);
    screen.clearId(text2);
    
  }
  
    ARGB drawColor = ARGB(selectableColors[selectedColorIndex]);
    clearId = screen.fillRectangle(cwidth * (totalcolumns-1), 0, cwidth * totalcolumns, visibleHeight/2-10, visibleColor(drawColor), String("8"));
    text1 = screen.drawAt(cwidth * (totalcolumns-0.75), 5, "Set", String("8"));
    text2 = screen.drawAt(cwidth * (totalcolumns-0.75), 25, "All", String("8"));
  

  if (clearId2 == 0) {
    clearId2 = screen.fillRectangle(cwidth * (totalcolumns-1), visibleHeight/2+10, cwidth * totalcolumns, visibleHeight, visibleColor(lightBlack), String("9"));
    text3 = screen.drawAt(cwidth * (totalcolumns-0.8), 250, "CLR", String("9"));
  }  
}

void drawGrid() 
{
  screen.clear();
  clearId = 0;
  clearId2 = 0;
  
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
  drawClear();
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
  
  int id = shieldEvent->id;
  
  if (shieldEvent->actionHash == tappedHash || shieldEvent->actionHash == enteredHash || shieldEvent->actionHash == pressedHash) {
    if ((tag[1] == 0 && tag[0] < 48+(colorCount+2) && tag[0] >= 48)) {
      if (tag[0] == 48+(colorCount) || tag[0] == 48+(colorCount+1)) {
        if (shieldEvent->actionHash == tappedHash || shieldEvent->actionHash == pressedHash) {
          if ( tag[0] == 48+(colorCount+1)) {
              selectedColor = BLACK;
          }
          clearGrid();
          selectedColor = ARGB(selectableColors[selectedColorIndex]);
        }
      } else {
        selectedColorIndex = tag[0]-48;
        selectedColor = ARGB(selectableColors[selectedColorIndex]);
        drawClear();
      }
    } else if (tag[0] != 0 && tag[2] == 0) {
      //grid
      if (shieldEvent->actionHash == enteredHash) {
        setColor(x,y,selectedColor);
      }
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
  selectedColorIndex = 0;
  clearGrid(true);

  strip.begin();
  shield.begin();
}


void loop()
{
  shield.checkSensors();
}		 
