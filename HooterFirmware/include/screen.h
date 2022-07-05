#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

  /////Screen definitions
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BAT_PIN     A2
#define LOGO_HEIGHT   64
#define LOGO_WIDTH    128
String PatternsText[] = {"  Matrix", "  Glitter", " Dot Beat", "   Blur", "   Fill", " Blendwave", " Beatwave", "  Fade In", " Rainbow", " Confetti", "  Sinelon", "  Juggle", "   BPM" };

// Hootie V2 Logo
static const unsigned char PROGMEM logo_bmp[]={    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
    0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 
    0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
    0x01, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xfd, 0xff, 
    0xff, 0xbf, 0xff, 0xff, 0x7f, 0xff, 0xff, 0x80, 
    0x00, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xfb, 0xff, 
    0xff, 0xdf, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x1c, 
    0x1c, 0x7f, 0xff, 0xff, 0x3f, 0x87, 0xf3, 0xff, 
    0xff, 0xcf, 0xe1, 0xfc, 0xff, 0xff, 0xfe, 0x3f, 
    0xff, 0x7f, 0xff, 0xff, 0xbe, 0x01, 0xf7, 0xff, 
    0xff, 0xef, 0x80, 0x7d, 0xff, 0xff, 0xfe, 0x7f, 
    0xff, 0xbf, 0xff, 0xff, 0xcc, 0x01, 0xef, 0xff, 
    0xff, 0xf7, 0x00, 0x7b, 0xff, 0xff, 0xfc, 0xff, 
    0xff, 0xbf, 0xff, 0xff, 0xdf, 0x01, 0xcf, 0xff, 
    0xff, 0xf3, 0x80, 0x73, 0xff, 0xff, 0xfc, 0xff, 
    0xff, 0xdf, 0xff, 0xff, 0xe7, 0x03, 0x9f, 0xff, 
    0xff, 0xfb, 0xc0, 0xe7, 0xff, 0xff, 0xfc, 0xc3, 
    0xe1, 0xdf, 0xff, 0xff, 0xf7, 0x87, 0xbf, 0xff, 
    0xff, 0xfd, 0xe1, 0xef, 0xff, 0xff, 0xfc, 0xe1, 
    0xe3, 0xdf, 0xff, 0xff, 0xf3, 0x87, 0x3f, 0xff, 
    0xff, 0xfc, 0xe1, 0xcf, 0xff, 0xff, 0xfc, 0xf3, 
    0xc7, 0x9f, 0xff, 0xff, 0xfb, 0xcf, 0x3f, 0xff, 
    0xff, 0xfc, 0xf3, 0xcf, 0xff, 0xff, 0xfc, 0xfe, 
    0xff, 0xbf, 0xff, 0xff, 0xf9, 0xee, 0x7f, 0xff, 
    0xff, 0xfe, 0x7f, 0x9f, 0xff, 0xff, 0xfe, 0x7f, 
    0xff, 0x9f, 0xff, 0xff, 0xf9, 0xfe, 0x7f, 0xff, 
    0xff, 0xfe, 0x7f, 0x9f, 0xff, 0xff, 0xfe, 0x3f, 
    0x3f, 0x3f, 0xff, 0xff, 0xfc, 0xfc, 0xff, 0xff, 
    0xff, 0xff, 0x3f, 0x3f, 0xff, 0xff, 0xfe, 0x3f, 
    0x3e, 0x7f, 0xff, 0xff, 0xfc, 0xfc, 0xff, 0xff, 
    0xff, 0xff, 0x3f, 0x3f, 0xff, 0xff, 0xff, 0x0f, 
    0x7c, 0x7f, 0xff, 0xff, 0xfc, 0x7c, 0xff, 0xff, 
    0xff, 0xff, 0x9e, 0x3f, 0xff, 0xff, 0xff, 0x0f, 
    0xf9, 0xff, 0xff, 0xff, 0xfe, 0x79, 0xff, 0xff, 
    0xff, 0xff, 0x1e, 0x7f, 0xff, 0xff, 0xfe, 0x01, 
    0xe1, 0xff, 0xff, 0xff, 0xfe, 0x79, 0xff, 0xff, 
    0xff, 0xff, 0x9e, 0x7f, 0xff, 0xff, 0xfe, 0x00, 
    0x07, 0xff, 0xff, 0xff, 0xfe, 0x31, 0xff, 0xff, 
    0xff, 0xff, 0x8c, 0x7f, 0xff, 0xff, 0xfc, 0x00, 
    0xbf, 0xff, 0xff, 0xff, 0xfe, 0x39, 0xff, 0xff, 
    0xff, 0xff, 0x8e, 0x7f, 0xff, 0xff, 0xf8, 0x01, 
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x31, 0xff, 0xff, 
    0xff, 0xff, 0x9c, 0x7f, 0xff, 0xff, 0xf8, 0x00, 
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x71, 0xff, 0xff, 
    0xff, 0xff, 0x8c, 0x7f, 0xff, 0xff, 0xf8, 0x01, 
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x31, 0xff, 0xff, 
    0xff, 0xff, 0x8c, 0x7f, 0xff, 0xff, 0xf0, 0x01, 
    0xfe, 0xff, 0xff, 0xff, 0xfe, 0x39, 0xff, 0xff, 
    0xff, 0xff, 0x9e, 0x7f, 0xff, 0xff, 0xf0, 0x01, 
    0xfe, 0xff, 0xff, 0xff, 0xfe, 0x79, 0xff, 0xff, 
    0xff, 0xff, 0x8e, 0x7f, 0xff, 0xff, 0xf0, 0x01, 
    0xfe, 0x7f, 0xff, 0xff, 0xfe, 0x78, 0xff, 0xff, 
    0xff, 0xff, 0x9e, 0x3f, 0xff, 0xff, 0xe0, 0x03, 
    0xfe, 0x7f, 0xff, 0xff, 0xfc, 0x78, 0xff, 0xff, 
    0xff, 0xff, 0x1e, 0x3f, 0xff, 0xff, 0xf0, 0x03, 
    0xfc, 0x7f, 0xff, 0xff, 0xfc, 0x78, 0xff, 0xff, 
    0xff, 0xff, 0x1f, 0x3f, 0xff, 0xff, 0xe0, 0x07, 
    0xfe, 0x7f, 0xff, 0xff, 0xfc, 0xfc, 0xff, 0xff, 
    0xff, 0xff, 0x3f, 0x3f, 0xff, 0xff, 0xf0, 0x07, 
    0xfc, 0x7f, 0xff, 0xff, 0xfc, 0xfc, 0xff, 0xff, 
    0xff, 0xff, 0x3f, 0x1f, 0xff, 0xff, 0xe0, 0x07, 
    0xfc, 0x7f, 0xff, 0xff, 0xf9, 0xfe, 0x7f, 0xff, 
    0xff, 0xfe, 0x7f, 0x9f, 0xff, 0xff, 0xf0, 0x0f, 
    0xfc, 0x3f, 0xff, 0xff, 0xf9, 0xfe, 0x7f, 0xff, 
    0xff, 0xfe, 0x7f, 0x9f, 0xff, 0xff, 0xe0, 0x0f, 
    0xfc, 0x7f, 0xff, 0xff, 0xf9, 0xfe, 0x3f, 0xff, 
    0xff, 0xfe, 0x7f, 0x9f, 0xff, 0xff, 0xf0, 0x1f, 
    0xfc, 0x7f, 0xff, 0xff, 0xf3, 0xff, 0x3f, 0xff, 
    0xff, 0xfc, 0xff, 0xcf, 0xff, 0xff, 0xf0, 0x1f, 
    0xf8, 0x7f, 0xff, 0xff, 0xf3, 0xff, 0x3f, 0xff, 
    0xff, 0xfc, 0xff, 0xcf, 0xff, 0xff, 0xf0, 0x1f, 
    0xfc, 0x7f, 0xff, 0xff, 0xf3, 0xff, 0x9f, 0xff, 
    0xff, 0xfd, 0xff, 0xc7, 0xff, 0xff, 0xf0, 0x3f, 
    0xf8, 0xff, 0xff, 0xff, 0xe7, 0xff, 0x9f, 0xff, 
    0xff, 0xf9, 0xff, 0xe7, 0xff, 0xff, 0xf8, 0x3f, 
    0xf8, 0xff, 0xff, 0xff, 0xe7, 0xff, 0x9f, 0xff, 
    0xff, 0xf9, 0xff, 0xe7, 0xff, 0xff, 0xf8, 0x3f, 
    0xf0, 0xff, 0xff, 0xff, 0xef, 0xff, 0xcf, 0xff, 
    0xff, 0xfb, 0xff, 0xf7, 0xff, 0xff, 0xf8, 0x3f, 
    0xf1, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xcf, 0xff, 
    0xff, 0xf3, 0xff, 0xf3, 0xff, 0xff, 0xfc, 0x3f, 
    0xf3, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xef, 0xff, 
    0xff, 0xf7, 0xff, 0xfb, 0xff, 0xff, 0xfc, 0x3f, 
    0xf3, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xe7, 0xff, 
    0xff, 0xe7, 0xff, 0xf9, 0xff, 0xff, 0xfe, 0x7f, 
    0xe7, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xf7, 0xff, 
    0xff, 0xef, 0xff, 0xfd, 0xff, 0xff, 0xfe, 0x3f, 
    0xef, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xf7, 0xff, 
    0xff, 0xef, 0xff, 0xfc, 0xff, 0xff, 0xff, 0x3f, 
    0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xfb, 0xff, 
    0xff, 0xdf, 0xff, 0xfe, 0xff, 0xff, 0xff, 0x3f, 
    0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xfb, 0xff, 
    0xff, 0xdf, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xbf, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xef, 0x78, 0xfe, 0x3a, 
    0x4b, 0x93, 0xf7, 0xbd, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xcf, 0x30, 0x38, 0x08, 
    0x09, 0x01, 0xf3, 0x90, 0x7f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xef, 0x67, 0x99, 0xcf, 
    0x79, 0xbf, 0xf3, 0x37, 0x7f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xc4, 0x27, 0x93, 0xef, 
    0x39, 0x9f, 0xf3, 0x3b, 0x3f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xe0, 0x67, 0x93, 0xe7, 
    0x39, 0x81, 0xfb, 0x7e, 0x7f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xcf, 0x27, 0x9b, 0xcf, 
    0x79, 0x9f, 0xf9, 0x7c, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xe7, 0x67, 0x91, 0xcf, 
    0x39, 0xbf, 0xf8, 0x79, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xcf, 0x32, 0x39, 0x0f, 
    0x39, 0x85, 0xfc, 0xf1, 0x7f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xee, 0x78, 0x7c, 0x3f, 
    0x79, 0x81, 0xfc, 0xf0, 0x7f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

// display the logo on boot
void drawLogo(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}

// map float function 
double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// update the display
void updateDisplay(int mode) {
  int analog_reading = analogRead(BAT_PIN);
  float battery_voltage = mapf(analog_reading, 582, 845, 9, 13);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(" HOOTIE V2");
  display.print("BAT:");
  display.print(battery_voltage);
  display.println("V");
  display.println(PatternsText[mode]);
  display.println(" BRC  2022");
  display.display(); // actually display all of the above
}


// setup screen
void setupScreen(void) {
      // Clear the buffer
      display.clearDisplay();
      display.display();
      
      //display hootie logo
      drawLogo();
      updateDisplay(0);
}