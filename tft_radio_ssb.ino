#include <Elegoo_GFX.h>     // Core graphics library
#include <Elegoo_TFTLCD.h>  // Hardware-specific library
#include <TouchScreen.h>

#include <math.h>

#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINY 120
#define TS_MAXY 920

#define TS_MINX 70
#define TS_MAXX 900

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
int x;
int y;

int selection;
String selection2 = "";


long freq = 7000000;
double freq_to_add = 0;

int array_freq[8];


class Rect {
public:
  int16_t x;
  int16_t y;
  int16_t width;
  int16_t height;
};

Rect selezioni[8];

void fillSelezioni() {

  selezioni[0].x = 1;
  selezioni[0].y = 107;
  selezioni[0].width = 35;
  selezioni[0].height = 35;

  selezioni[1].x = 36;
  selezioni[1].y = 107;
  selezioni[1].width = 35;
  selezioni[1].height = 35;

  selezioni[2].x = 72;
  selezioni[2].y = 107;
  selezioni[2].width = 35;
  selezioni[2].height = 35;

  selezioni[3].x = 108;
  selezioni[3].y = 107;
  selezioni[3].width = 35;
  selezioni[3].height = 35;

  selezioni[4].x = 144;
  selezioni[4].y = 107;
  selezioni[4].width = 35;
  selezioni[4].height = 35;

  selezioni[5].x = 180;
  selezioni[5].y = 107;
  selezioni[5].width = 35;
  selezioni[5].height = 35;

  selezioni[6].x = 216;
  selezioni[6].y = 107;
  selezioni[6].width = 35;
  selezioni[6].height = 35;

  selezioni[7].x = 252;
  selezioni[7].y = 107;
  selezioni[7].width = 35;
  selezioni[7].height = 35;
}

int convert_array(void) {

  for (int i = 0; i < 8; i++) {
    freq *= 10;
    freq += array_freq[i];
  }
  return freq;
}

int countDigit(long n) {
  if (n == 0)
    return 1;
  int count = 0;
  while (n != 0) {
    n = n / 10;
    ++count;
  }
  return count;
}

void fill_array(long number) {
  /*Serial.print("fill_array: ");
  Serial.println(number);*/
  

  for (int i = 7; i >= 0; i--) {
    /*Serial.print("i dentro ciclo: ");
    Serial.println(i);
    Serial.print("array_freq[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(array_freq[i]);*/
    array_freq[i] = number % 10;
    /*Serial.print("array_freq[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(array_freq[i]);*/
    number /= 10;
    /*Serial.print("n dentro ciclo: ");
    Serial.println(number);*/
  
  }

}

void fill_array_zero(void) {
  for (int i = 7; i >= 0; i--) {
    array_freq[i] = 0;
  }
}

void resertSelection(void) {
  for (int i = 0; i < 8; i++) {
    tft.drawRect(selezioni[i].x, selezioni[i].y, selezioni[i].width, selezioni[i].height, BLACK);
  }
}

int whatDigit(TSPoint punto) {

  for (int i = 0; i < 8; i++) {
    int pointX = map(punto.y, TS_MINX, TS_MAXX, tft.width(), 0);
    int pointY = map(punto.x, TS_MINY, TS_MAXY, tft.height(), 0);
    if (pointX >= selezioni[i].x && pointX <= (selezioni[i].x + selezioni[i].width) && pointY >= selezioni[i].y && pointY <= (selezioni[i].y + selezioni[i].height)) {
      tft.drawRect(selezioni[i].x, selezioni[i].y, selezioni[i].width, selezioni[i].height, RED);
      freq_to_add = round(pow(static_cast<double>(10), static_cast<double>(static_cast<double>(7) - static_cast<double>(i))));
      Serial.println(freq_to_add);
      return 0;
    }
  }
  freq_to_add = 0;
  return 0;
}

void refreshDigits(void) {
  tft.drawChar(11, 114, array_freq[0] + '0', WHITE, 2, 3);
  tft.drawChar(46, 114, array_freq[1] + '0', WHITE, 2, 3);
  tft.drawChar(82, 114, array_freq[2] + '0', WHITE, 2, 3);
  tft.drawChar(118, 114, array_freq[3] + '0', WHITE, 2, 3);
  tft.drawChar(154, 114, array_freq[4] + '0', WHITE, 2, 3);
  tft.drawChar(190, 114, array_freq[5] + '0', WHITE, 2, 3);
  tft.drawChar(226, 114, array_freq[6] + '0', WHITE, 2, 3);
  tft.drawChar(262, 114, array_freq[7] + '0', WHITE, 2, 3);
  tft.drawChar(290, 120, 'h', WHITE, 2, 2);
  tft.drawChar(310, 120, 'z', WHITE, 2, 2);
}

void setup(void) {

  fillSelezioni();
  Serial.begin(9600);
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(0x9341);
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  tft.drawLine(1, 30, 320, 30, WHITE);
  fill_array_zero();
  fill_array(freq);
  resertSelection();
  refreshDigits();

  // bottone push

  tft.drawRect(100, 150, 100, 50, WHITE);
  tft.drawChar(120, 160, 'P', WHITE, 2, 3);
  tft.drawChar(135, 160, 'U', WHITE, 2, 3);
  tft.drawChar(150, 160, 'S', WHITE, 2, 3);
  tft.drawChar(165, 160, 'H', WHITE, 2, 3);

  pinMode(13, OUTPUT);
}

bool isUp(TSPoint punto) {

  int pointX = map(punto.y, TS_MINX, TS_MAXX, tft.width(), 0);
  int pointY = map(punto.x, TS_MINY, TS_MAXY, tft.height(), 0);

  if (pointX >= 100 & pointX <= 200 && pointY >= 150 && pointY <= 200) {
    return true;
  } else {
    resertSelection();
    return false;
  }
}

void loop(void) {

  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {

    if (isUp(p)) {
      freq = freq + freq_to_add;
      fill_array_zero();
      fill_array(freq);
      refreshDigits();
    } else {
      whatDigit(p);
    }
    Serial.print("X = ");
    Serial.print(x);
    Serial.print("\tY = ");
    Serial.print(y);
    Serial.print("\tPressure = ");
    Serial.println(p.z);
    Serial.println(freq_to_add);
    delay(100);

    Serial.print("p.x = ");
    Serial.print(p.x);
    Serial.print("\tp.y = ");
    Serial.print(p.y);
  }
}