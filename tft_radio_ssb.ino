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

int array_freq[8];

String mode = "RX";
String oldMode = "";

class Rect {
public:
  int16_t x;
  int16_t y;
  int16_t width;
  int16_t height;
};

Rect selezioni[8];
int band = 20;

unsigned long freq;
unsigned long freq_to_add = 0;
unsigned long fmi_;
unsigned long fma_;
String bandMode;


void changeBand(TSPoint punto) {
  int pointX = map(punto.y, TS_MINX, TS_MAXX, tft.width(), 0);
  int pointY = map(punto.x, TS_MINY, TS_MAXY, tft.height(), 0);
  String change;

  if (pointX >= 25 && pointX <= 55 && pointY >= 50 && pointY <= 60) {
    band = 80;
    change = "Y";
  }

  if (pointX >= 70 && pointX <= 100 && pointY >= 50 && pointY <= 60) {
    band = 40;
    change = "Y";
  }

  if (pointX >= 115 && pointX <= 145 && pointY >= 50 && pointY <= 60) {
    band = 20;
    change = "Y";
  }

  if (pointX >= 170 && pointX <= 200 && pointY >= 50 && pointY <= 60) {
    band = 15;
    change = "Y";
  }

  if (pointX >= 225 && pointX <= 255 && pointY >= 50 && pointY <= 60) {
    band = 11;
    change = "Y";
  }

  if (pointX >= 280 && pointX <= 310 && pointY >= 50 && pointY <= 60) {
    band = 10;
    change = "Y";
  }


  if (change == "Y") {
    printBand();
    set_freq();
    check_freq();
    fill_array_zero();
    fill_array(freq);
    refreshDigits();
    printBandMode();

  }

    
}
void printBand() {
  tft.setCursor(25, 50);
  tft.setTextSize(2);
  if (band == 80) {
    tft.setTextColor(RED);
  } else {
    tft.setTextColor(WHITE);
  }
  tft.print("80m ");
  if (band == 40) {
    tft.setTextColor(RED);
  } else {
    tft.setTextColor(WHITE);
  }
  tft.print("40m ");
  if (band == 20) {
    tft.setTextColor(RED);
  } else {
    tft.setTextColor(WHITE);
  }
  tft.print("20m ");
  if (band == 15) {
    tft.setTextColor(RED);
  } else {
    tft.setTextColor(WHITE);
  }
  tft.print("15m ");

  if (band == 11) {
    tft.setTextColor(RED);
  } else {
    tft.setTextColor(WHITE);
  }
  tft.print("11m ");

  if (band == 10) {
    tft.setTextColor(RED);
  } else {
    tft.setTextColor(WHITE);
  }
  tft.print("10m ");
}

void printMode() {
  tft.setCursor(1, 10);
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.print("Mode:" + oldMode);

  tft.print("                ");
  tft.setCursor(1, 10);
  tft.setTextColor(WHITE);
  tft.print("Mode:" + mode);
}

void changeMode(TSPoint punto) {

  int pointX = map(punto.y, TS_MINX, TS_MAXX, tft.width(), 0);
  int pointY = map(punto.x, TS_MINY, TS_MAXY, tft.height(), 0);
  if (pointX >= 1 && pointX <= 40 && pointY >= 10 && pointY <= 20) {
    oldMode = mode;
    if (mode == "RX") {
      mode = "RX/TX";
    } else {
      mode = "RX";
    }
    printMode();
  }
}

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
  for (int i = 7; i >= 0; i--) {
    array_freq[i] = number % 10;
    number /= 10;
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


bool isUp(TSPoint punto) {

  int pointX = map(punto.y, TS_MINX, TS_MAXX, tft.width(), 0);
  int pointY = map(punto.x, TS_MINY, TS_MAXY, tft.height(), 0);

  if (pointX >= 50 & pointX <= 120 && pointY >= 165 && pointY <= 215) {
    return true;
  } else {
    return false;
  }
}

bool isDown(TSPoint punto) {

  int pointX = map(punto.y, TS_MINX, TS_MAXX, tft.width(), 0);
  int pointY = map(punto.x, TS_MINY, TS_MAXY, tft.height(), 0);

  if (pointX >= 150 & pointX <= 220 && pointY >= 165 && pointY <= 215) {
    return true;
  } else {
    return false;
  }
}


void set_freq() {
  if (band == 80) freq = 3500000;
  if (band == 40) freq = 7000000;
  if (band == 20) freq = 14000000;
  if (band == 15) freq = 21000000;
  if (band == 11) freq = 26500000;
  if (band == 10) freq = 28000000;
}


void check_freq() {

  if (band == 80) {
    fmi_ = 3500000;
    fma_ = 3580000;
    bandMode = "LSB";
  }
  if (band == 40) {
    fmi_ = 7000000;
    fma_ = 7350000;
    bandMode = "LSB";
  }
  if (band == 20) {
    fmi_ = 14000000;
    fma_ = 14380000;
    bandMode = "USB";
  }
  if (band == 15) {
    fmi_ = 21000000;
    fma_ = 21650000;
    bandMode = "USB";
  }
  if (band == 11) {
    fmi_ = 26500000;
    fma_ = 27500000;
    bandMode = "USB";
  }
  if (band == 10) {
    fmi_ = 28000000;
    fma_ = 28800000;
    bandMode = "USB";
  }
}

void printBandMode(){

  tft.setCursor(280, 10);
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.print("LSB");
  tft.setCursor(280, 10);
  tft.print("USB");
  tft.setCursor(280, 10);  
  tft.setTextColor(WHITE);
  tft.print(bandMode);

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
  check_freq();
  set_freq();
  fill_array_zero();
  fill_array(freq);
  resertSelection();
  refreshDigits();
  printMode();
  printBand();
  tft.drawLine(1, 90, 320, 90, WHITE);
  printBandMode();

  // bottone UP
  tft.drawRect(50, 165, 70, 50, WHITE);
  tft.drawChar(70, 180, 'U', WHITE, 2, 2);
  tft.drawChar(90, 180, 'P', WHITE, 2, 2);
  // bottone DOWN
  tft.drawRect(150, 165, 70, 50, WHITE);
  tft.drawChar(155, 180, 'D', WHITE, 2, 2);
  tft.drawChar(170, 180, 'O', WHITE, 2, 2);
  tft.drawChar(185, 180, 'W', WHITE, 2, 2);
  tft.drawChar(200, 180, 'N', WHITE, 2, 2);


  pinMode(13, OUTPUT);
}

void loop(void) {

  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    if (isUp(p)) {
      if ((freq + freq_to_add) <= fma_) {
        freq = freq + freq_to_add;
        fill_array_zero();
        fill_array(freq);
        refreshDigits();
      }
    } else if (isDown(p)) {
      if ((freq - freq_to_add) >= fmi_) {
        freq = freq - freq_to_add;
        fill_array_zero();
        fill_array(freq);
        refreshDigits();
      }
    } else {
      resertSelection();
      whatDigit(p);
      changeMode(p);
      changeBand(p);
    }
    delay(50);
  }
}