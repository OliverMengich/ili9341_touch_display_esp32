#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
#define CF_OL24 &Orbitron_Light_24
#define GFXFF 1
#define FF18 &FreeSans12pt7b
#include "Free_Fonts.h" // Include the header file attached to this sketch
unsigned long drawTime = 0;
#define FRAME_X 100
#define FRAME_Y 64
#define FRAME_W 120
#define FRAME_H 50
bool SwitchOn = false;
bool returnHome=false;
// Red zone size
#define REDBUTTON_X FRAME_X
#define REDBUTTON_Y FRAME_Y
#define REDBUTTON_W (FRAME_W/2)
#define REDBUTTON_H FRAME_H

#define GREENBUTTON_X (REDBUTTON_X + REDBUTTON_W)
#define GREENBUTTON_Y FRAME_Y
#define GREENBUTTON_W (FRAME_W/2)
#define GREENBUTTON_H FRAME_H
void setup(void) {
  tft.init();
//  tft.begin();

  tft.setRotation(1);
  homePage();
}
void drawDatumMarker(int x, int y){
  tft.drawLine(x,y, x + 10, y, TFT_GREEN);
  tft.drawLine(x, y, x+5, y - 5, TFT_GREEN);
  tft.drawLine(x, y, x+5, y + 5, TFT_GREEN);
}
void header(const char *string, uint16_t color){
  tft.fillScreen(color);
  tft.setTextSize(1);
  tft.setTextColor(TFT_MAGENTA, TFT_BLUE);
  tft.fillRect(0, 0, 320, 30, TFT_BLUE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString(string, 160, 2, 4); // Font 4 for fast drawing with background
}
void homePage(){
  header("SMART ENERGY METER", TFT_BLACK); 
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setFreeFont(FF18);                 // Select the font
  tft.setFreeFont(CF_OL24);
  //Configuration Page
  tft.drawLine(10,50, 300, 50, TFT_GREEN);
  tft.drawLine(300,50, 300, 90, TFT_GREEN);
  tft.drawLine(300,90, 10, 90, TFT_GREEN);
  tft.drawLine(10,90, 10, 50, TFT_GREEN);
  tft.setCursor(15, 75);
  tft.print("Configurations Page");
  //Online Page
  tft.drawLine(10,100, 230, 100, TFT_GREEN);
  tft.drawLine(230,100, 230, 135, TFT_GREEN);
  tft.drawLine(230,135, 10, 135, TFT_GREEN);
  tft.drawLine(10,135, 10, 50, TFT_GREEN);
  tft.setCursor(15, 125);
  tft.print("Readings Page");
  // Infor Page
  tft.drawLine(10,150, 230, 150, TFT_GREEN);
  tft.drawLine(230,100, 230, 185, TFT_GREEN);
  tft.drawLine(230,185, 10, 185, TFT_GREEN);
  tft.drawLine(10,185, 10, 50, TFT_GREEN);
  tft.setCursor(15, 175);
  tft.print("Power Page");
}
void ReadingsPage(){
  uint16_t x1=10, y1=10;
  bool pressed1 = tft.getTouch(&x1,&y1);
  header("Readings Page", TFT_BLACK);
  drawDatumMarker(10, 10);
  tft.setFreeFont(CF_OL24);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10,60);
  tft.print("WaterLevel 1: ");
  tft.setCursor(10,95);
  tft.print("WaterLevel 2: ");
  tft.setCursor(10,130);
  tft.print("WaterLevel 3: ");
  tft.setCursor(10,160);
  tft.print("Board temp: ");
  tft.setCursor(10,190);
  tft.print("Pump is: ");
  tft.setCursor(10,225);
  tft.print("Power: ");
}
void powerReadingsPage(){
  uint16_t x1=10, y1=10;
  bool pressed1 = tft.getTouch(&x1,&y1);
  header("Power", TFT_BLACK);
  drawDatumMarker(10, 10);
  tft.setFreeFont(CF_OL24);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10,60);
  tft.print("Current: ");
  tft.setCursor(10,95);
  tft.print("Voltage: ");
  tft.setCursor(10,130);
}
void configurationPage(){
  header("Configuration Page", TFT_BLACK);
  drawDatumMarker(10, 10);
  redBtn();
  uint16_t x, y;
    // See if there's any touch data for us
  if (tft.getTouch(&x, &y)){
    // Draw a block spot to show where touch was calculated to be
    #ifdef BLACK_SPOT
      tft.fillCircle(x, y, 2, TFT_BLACK);
    #endif
    
    if (SwitchOn)
    {
      if ((x > REDBUTTON_X) && (x < (REDBUTTON_X + REDBUTTON_W))) {
        if ((y > REDBUTTON_Y) && (y <= (REDBUTTON_Y + REDBUTTON_H))) {
          Serial.println("Red btn hit");
          redBtn();
        }
      }
    }
    else //Record is off (SwitchOn == false)
    {
      if ((x > GREENBUTTON_X) && (x < (GREENBUTTON_X + GREENBUTTON_W))) {
        if ((y > GREENBUTTON_Y) && (y <= (GREENBUTTON_Y + GREENBUTTON_H))) {
          Serial.println("Green btn hit");
          greenBtn();
        }
      }
    }
    Serial.println(SwitchOn);
  }
}
void redBtn(){
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, TFT_RED);
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("OFF", GREENBUTTON_X + (GREENBUTTON_W / 2), GREENBUTTON_Y + (GREENBUTTON_H / 2));
  SwitchOn = false;
}

// Draw a green button
void greenBtn(){
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, TFT_GREEN);
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("ON", REDBUTTON_X + (REDBUTTON_W / 2) + 1, REDBUTTON_Y + (REDBUTTON_H / 2));
  SwitchOn = true;
}
void loop(){
  uint16_t x1=10, y1=10;
  bool pressed5 = tft.getTouch(&x1,&y1);
  if(pressed5){
    homePage();
  }else{
    uint16_t x1=15, y1=75;
    uint16_t x2=15, y2=125;
    uint16_t x3=15, y3=175;
    bool pressed1 = tft.getTouch(&x1,&y1);
    bool pressed2 = tft.getTouch(&x2,&y2);
    bool pressed3 = tft.getTouch(&x3,&y3);
    if (pressed1){
        configurationPage();
    }else if(pressed2){
        ReadingsPage();
    }else if(pressed3){
        powerReadingsPage();
    }
  }
}
