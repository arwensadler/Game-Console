
#include <ILI9488_t3.h>
#include <ILI9488_t3_font_Arial.h>
#include <ILI9488_t3_font_ArialBold.h>
//#define TEENSY64

#if defined(__MK66FX1M0__) && !defined(TEENSY64)
#define TFT_RST 255
#define TFT_DC 20
#define TFT_CS 21
ILI9488_t3 tft = ILI9488_t3(&SPI, TFT_CS, TFT_DC, TFT_RST);
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__)
// On Teensy 4 beta with Paul's breakout out:
// Using pins (MOSI, MISO, SCK which are labeled on Audio board breakout location
// which are not in the Normal processor positions
// Also DC=10(CS), CS=9(BCLK) and RST 23(MCLK)
#define TFT_RST 23
#define TFT_DC 9
#define TFT_CS 10
ILI9488_t3 tft = ILI9488_t3(&SPI, TFT_CS, TFT_DC, TFT_RST);
#elif defined(TEENSY64)
#define TFT_RST 255
#define TFT_DC 20
#define TFT_CS 21
#define TFT_SCK 14
#define TFT_MISO 39
#define TFT_MOSI 28



ILI9488_t3 tft = ILI9488_t3(&SPI, TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCK, TFT_MISO);
#else
#error "This example App will only work with Teensy 3.6 or Teensy 4."
#endif
//=============================================================================
// Setup
//=============================================================================
void setup()
{
  tft.begin();
  tft.setRotation(3); // 180
  delay(100);

  tft.fillScreen(ILI9488_BLACK);
  delay(250);

  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);

  delay(500);

}

void drawColor(uint16_t x, uint16_t y, const char *psz, uint16_t color)
  {
    tft.setFontAdafruit();
    tft.setTextColor(color);
    tft.setTextSize(2);
    tft.setCursor(x, y);
    tft.print(psz);
//    tft.drawRect(x + 100, y, 50, 50, color);
//    tft.fillRect(x + 110, y + 10, 30, 30, color);
//    tft.drawLine(x + 100, y + 70, x + 200, y + 70, color);
//    tft.drawLine(x + 220, y, x + 220, y + 70, color);
//    tft.drawLine(x + 100, y + 70, x + 220, y, color);
//    tft.drawCircle(x + 50, y + 50, 28, color);
//    tft.fillCircle(x + 50, y + 50, 20, color);
    tft.setFont(Arial_12_Bold);
    tft.setCursor(x + 160, y + 50);
    tft.print(psz);
  }

// Code for drawing fruit at a random location
void drawFruit(uint16_t x, uint16_t y, uint16_t color)
{
  tft.setCursor(x, y);
  tft.drawRect(x, y, 10, 10, color);
  tft.fillRect(x, y, 10, 10, color);
}

int fruitX(){
  int x;
  x = random(1, 30);
  return x*10;
}

int fruitY(){
  int y;
  y = random(1, 22);
  return y*10;
}


void updateSnake(int posx[], int posy[], int dir, int score) {
  tft.setCursor(posx[score - 1], posy[score - 1]);
  tft.drawRect(posx[score - 1], posy[score - 1], 10, 10, ILI9488_BLACK);
  tft.fillRect(posx[score - 1], posy[score - 1], 10, 10, ILI9488_BLACK);
  for (int i = score - 2; i >= 0; i--){
    posx[i + 1] = posx[i];
    posy[i + 1] = posy[i];
  }

  if (dir == 1) {
    posx[0] -= 10;
    posy[0] += 0;
  } else if (dir == 2) {
    posx[0] += 10;
    posy[0] += 0;
  } else if (dir == 3) {
    posx[0] -= 0;
    posy[0] -= 10;
  } else if (dir == 4) {
    posx[0] += 0;
    posy[0] += 10;
  }
  
}

// Code for moving snake
void drawSnake(int posx[], int posy[], uint16_t color, int score)
{
  for (int i = 0; i <= (score - 1); i++) {
      tft.setCursor(posx[i], posy[i]);
      tft.drawRect(posx[i], posy[i], 10, 10, color);
      tft.fillRect(posx[i], posy[i], 10, 10, color);
  }
}




int vertical, horizontal;

int x;
int y;

int savedx;
int savedy;


unsigned long starttime = 0;
unsigned long endtime;

// List of x positions of body
int posx[100] = {150, 150, 150};
// List of y positions of body
int posy[100] = {110, 120, 130};

int score = 3;

// Whether to draw a new fruit--starts as true
bool newfruit = true;

// Whether the game is currently playing
bool playing = false;

// Direction: 1: LEFT, 2: RIGHT, 3: DOWN, 4: UP
int dir = 4;

void loop(){
  
  int btnpress = digitalRead(7);
  int joybtnpress = digitalRead(8);
  
  if (playing == true) {
    // if we need to draw a new fruit, pick a new location
    // and set newfruit = false
    if (newfruit == true) {
      x = fruitX();
      y = fruitY();
      
      newfruit = false;
    }
  
    
    
    
    horizontal = analogRead(14); // will be 0-1023
    vertical = analogRead(15); // will be 0-1023
  
    
    // Check if player is moving
    if ((horizontal < 400) & (dir != 2)){
      dir = 1;
    } else if ((horizontal > 500) & (dir != 1)) {
      dir = 2;
    } else if ((vertical < 400) & (dir != 4)) {
      dir = 3;
    } else if ((vertical > 550) & (dir != 3)) {
      dir = 4;
    }

    
  
    // Check if the player collects the fruit
    if (posx[0] == x & posy[0] == y) {
      savedx = posx[score - 1];
      savedy = posy[score - 1];
      newfruit = true;
    }
  

  
    
  
    // Start redrawing the screen
  
    // draw the fruit
    drawFruit(x, y, ILI9488_PINK);

    // update the player
    updateSnake(posx, posy, dir, score);

    if (newfruit == true) {
      
      posx[score] = savedx;
      posy[score] = savedy;
      score += 1;
    }
    
    // draw the player
    drawSnake(posx, posy, ILI9488_GREEN, score);
  
      // Check if the player has run into the edge of the screen
    if (((posx[0] <= 0) & (dir == 1)) | ((posx[0] >= 310) & (dir == 2)) | ((posy[0] <= 0) & (dir == 3)) | ((posy[0] >= 230) & (dir == 4))) {
      tft.fillScreen(ILI9488_BLACK);
      playing = false;
    }
      // Check if the player has run into their own tail
    for (int i = 1; i <= (score - 1); i++) {
      if ((posx[0] == posx[i]) & (posy[0] == posy[i])) {
        tft.fillScreen(ILI9488_BLACK);
        playing = false;
      }
    }
    delay(200);
  } else {
//    tft.setRotation(1);
    tft.setTextColor(ILI9488_WHITE);
    tft.setCursor(40, 110);
    tft.setTextSize(2);
    tft.println("Press button to start");
    tft.setRotation(3);
    if (btnpress == LOW){
      endtime = millis();
      randomSeed(endtime - starttime);
      tft.fillScreen(ILI9488_BLACK);
      // Reset snake position:
      posx[0] = 150;
      posx[1] = 150;
      posx[2] = 150;
      posy[0] = 130;
      posy[1] = 120;
      posy[2] = 110;
      for (int i = 3; i <= (score); i++) {
        posx[i] = 0;
        posy[i] = 0;
      }
      dir = 4;
      score = 3;
      newfruit = true;
      playing = true;
    }
  }

  
}
