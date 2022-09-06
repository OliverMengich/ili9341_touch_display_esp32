#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library
// Calibration data is stored in SPIFFS so we need to include it
#define BLACK_SPOT

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
#define CF_OL24 &Orbitron_Light_24
#define CF_RT24 &Roboto_Thin_24
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
  Serial.begin(9600);
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
  bool hereH1=true;
  tft.setFreeFont(CF_RT24); 
  while(hereH1==true){
    uint16_t x, y;
    // See if there's any touch data for us
  if (tft.getTouch(&x, &y)){
      Serial.println("I was touched !!!!");
      Serial.println(x);
      Serial.println(y);
     if(x<=23 && y<=14){
        Serial.println("Home Button was Pressed");
        hereH1=false;
        Serial.print("HereH now is: " );
        Serial.println(hereH1);
      }
      header("Readings Page", TFT_BLACK);
      drawDatumMarker(10, 10);
      tft.setFreeFont(CF_OL24);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
    
      tft.setCursor(10,60);
      tft.print("WaterLevel 1: ");
      tft.setCursor(220,60);
      tft.print(9440);
    
      tft.setCursor(10,95);
      tft.print("WaterLevel 2: ");
      tft.setCursor(220,95);
      tft.print(9440);
    
      tft.setCursor(10,130);
      tft.print("WaterLevel 3: ");
      tft.setCursor(220,130);
      tft.print(9440);
    
      tft.setCursor(10,160);
      tft.print("Board temp: ");
      tft.setCursor(200,160);
      tft.print(9440);
    
      tft.setCursor(10,190);
      tft.print("Pump is: ");
      tft.setCursor(150,190);
      tft.print(digitalRead(33));
    
      tft.setCursor(10,225);
      tft.print("Power: ");
      tft.setCursor(120,225);
      tft.print(9440);
    }
  }
  Serial.println("I'm in Readings page and I'm returning to homePage");
  return;
}
void powerReadingsPage(){
  bool hereH=true;
  tft.setFreeFont(CF_OL24);
  while(hereH==true){
    uint16_t x, y;
    // See if there's any touch data for us
  if (tft.getTouch(&x, &y)){
      Serial.println("I was touched !!!!");
      Serial.println(x);
      Serial.println(y);
     if(x<=23 && y<=14){
        Serial.println("Home Button was Pressed");
        hereH=false;
        Serial.print("HereH now is: " );
        Serial.println(hereH);
      }
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
  }
  Serial.println("I'm in Power page and I'm returning to homePage");
  return;
}
void configurationPage(){
  header("Configuration Page", TFT_BLACK);
  drawDatumMarker(10, 10);
  tft.setFreeFont(CF_OL24);
  redBtn();
  bool hereH=true;
  while(hereH==true){
  uint16_t x, y;
    // See if there's any touch data for us
  if (tft.getTouch(&x, &y)){
    Serial.println("I was touched !!!!");
    Serial.println(x);
    Serial.println(y);
     if(x<=23 && y<=14){
        Serial.println("Home Button was Pressed");
        hereH=false;
        Serial.print("HereH now is: ");
        Serial.println(hereH);
      }
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
//      Serial.println(SwitchOn);
    }
  }
  Serial.println("Returning to homePage");
  return;
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
  uint16_t x1, y1;
  if (tft.getTouch(&x1, &y1)){
    Serial.println("Nav was touched");
    Serial.println(x1);
    Serial.println(y1);
    if(x1<=35 && y1 >60 && y1<=75){
      Serial.println("Navigating to configurations page");
      configurationPage();
    }else if(x1<=35 && y1 >100 && y1 <= 125){
      Serial.println("Navigating to ReadingsPage");
      ReadingsPage();
    }
    else if(x1<=35 && y1 >130 && y1 <= 175){
      Serial.println("Navigating to Power ReadingsPage");
      powerReadingsPage();
    }
    else{
      homePage();
    }
  }
}
