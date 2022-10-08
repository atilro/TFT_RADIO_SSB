#include <Elegoo_GFX.h>     // Core graphics library
#include <Elegoo_TFTLCD.h>  // Hardware-specific library
#include <TouchScreen.h>

#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define MAXPRESSURE 1000
int pippo = 0;
int pippo2 = 0;
int x;
int y;


String selection = "1";
String selection2 = "";

#define BOXSIZE 40
#define PENRADIUS 3

void setup(void) {

  Serial.begin(9600);

  tft.reset();

  uint16_t identifier = tft.readID();

  tft.begin(0x9341);

  tft.setRotation(1);

  tft.fillScreen(BLACK);

  // primo numero

  tft.drawRect(1, 1, 50, 50, WHITE);
  tft.drawChar(15, 7, pippo + '0', WHITE, 2, 5);

  // seconodo numero

  tft.drawRect(51, 1, 50, 50, WHITE);
  tft.drawChar(66, 7, pippo2 + '0', WHITE, 2, 5);

  // bottone push

  tft.drawRect(100, 100, 100, 50, WHITE);
  tft.drawChar(120, 110, 'P', WHITE, 2, 3);
  tft.drawChar(135, 110, 'U', WHITE, 2, 3);
  tft.drawChar(150, 110, 'S', WHITE, 2, 3);
  tft.drawChar(165, 110, 'H', WHITE, 2, 3);


  pinMode(13, OUTPUT);
  Serial.print("Width : ");
  Serial.println(tft.width());

  Serial.print("Height : ");
  Serial.println(tft.height());
}

void loop(void) {

  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    y = map(p.x, TS_MINX, TS_MAXX, tft.width(), 1);
    x = map(p.y, TS_MINY, TS_MAXY, tft.height(), 1);

    if (x >= 1 & x <= 51 && y >= 1 && y <= 51) {
      selection = "1";
      selection2 = "null";
      tft.drawRect(1, 1, 50, 50, RED);
      tft.drawRect(51, 1, 50, 50, WHITE);
    } else if (x >= 51 & x <= 101 && y >= 1 && y <= 51) {
      selection = "2";
      selection2 = "null";
      tft.drawRect(1, 1, 50, 50, WHITE);
      tft.drawRect(51, 1, 50, 50, RED);
    } else if (x >= 100 & x <= 180 && y >= 100 && y <= 180) {
      selection2 = "button";
    } else {
      selection2 = "null";
    }


    if (selection2 == "button") {
      if (selection == "1") {
        pippo = pippo + 1;
        if (pippo == 10) {
          pippo = 0;
        }
      }
      if (selection == "2") {
        pippo2 = pippo2 + 1;
        if (pippo2 == 10) {
          pippo2 = 0;
        }
      }
    }

    tft.drawChar(15, 7, pippo + '0', WHITE, 2, 5);
    tft.drawChar(66, 7, pippo2 + '0', WHITE, 2, 5);
    Serial.print("X = ");
    Serial.print(x);
    Serial.print("\tY = ");
    Serial.print(y);
    Serial.print("\tPressure = ");
    Serial.println(p.z);
    Serial.println(selection);
    delay(100);
  }
}