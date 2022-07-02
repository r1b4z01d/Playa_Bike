#include <FastLED.h>
#include "EEPROM.h"
#define buttonPin 10                                           // input pin to use as a digital input
#include "jsbutton.h"                                         // Nice button routine by Jeff Saltzman
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

FASTLED_USING_NAMESPACE

#define BAT_PIN     A1
#define DATA_PIN    3

#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    1200
CRGB leds[NUM_LEDS];
CRGB clr1;
CRGB clr2;
uint8_t speed;
uint8_t loc1;
uint8_t loc2;
uint8_t ran1;
uint8_t ran2;

uint8_t frequency = 50;                                       // controls the interval between strikes
uint8_t flashes = 8;                                          //the upper limit of flashes per strike
unsigned int dimmer = 1;

uint8_t ledstart;                                             // Starting location of a flash
uint8_t ledlen;                                               // Length of a flash

#define BRIGHTNESS       100
#define FRAMES_PER_SECOND  120

// Palette definitions
CRGBPalette16 currentPalette = PartyColors_p;
CRGBPalette16 targetPalette;
TBlendType    currentBlending = LINEARBLEND; 

// Define variables used by the sequences.
int   thisdelay =   10;                                       // A delay value for the sequence(s)
uint8_t   count =   0;                                        // Count up to 255 and then reverts to 0
uint8_t fadeval = 224;                                        // Trail behind the LED's. Lower => faster fade.

// Initialize global variables for sequences.
uint8_t    palIndex =  95;
uint8_t    thissat = 255;
int        thisdir =   0;
uint8_t thisbright = 255;
bool        huerot =   0;                                     // Does the hue rotate? 1 = yes
uint8_t      bgclr =   0;
uint8_t      bgbri =   0;
uint8_t bpm = 30;
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BAT_PIN     A2
#define LOGO_HEIGHT   64
#define LOGO_WIDTH    128
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


void setup() {                                     // Initialize serial port for debugging.
  //delay(1000);                                                // Soft startup to ease the flow of electrons.

  // tell FastLED about the LED strip configuration
  //FastLED.addLeds<LED_TYPE,4,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,3,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,7,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,9,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  pinMode(buttonPin, INPUT);                                  // Set button input pin
  digitalWrite(buttonPin, HIGH );

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
 // if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    //Serial.println(F("SSD1306 allocation failed"));
  //  for(;;); // Don't proceed, loop forever
  //}

  // Clear the buffer
  //display.clearDisplay();
  //display.display();
  
  //display hootie logo
  //drawLogo();

  //set mode to zero - TODO remove eprom
  gCurrentPatternNumber = 10;
  //updateDisplay(gCurrentPatternNumber);

}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { matrix,rainbowWithGlitter, fill_grad, blendwave, beatwave,fadein,rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm2 };
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
String PatternsText[] = { "  Matrix", "  Glitter", "   Fill", " Blendwave", " Beatwave", "  Fade In", " Rainbow", " Confetti", "  Sinelon", "  Juggle", "   BPM" };

void loop()
{
  // Call the current patt\ern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();
  readbutton();
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  //FastLED.delay(1000/FRAMES_PER_SECOND); 

 EVERY_N_MILLISECONDS(100) {                                                 // FastLED based non-blocking FIXED delay.
    uint8_t maxChanges = 24;
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);    // AWESOME palette blending capability.
  }

  EVERY_N_SECONDS(5) {                                                        // Change the target palette to a random one every 5 seconds.
    uint8_t baseC = random8(255);                                             // Use the built-in random number generator as we are re-initializing the FastLED one.
    targetPalette = CRGBPalette16(CHSV(baseC+random8(0,32), 255, random8(128, 255)), CHSV(baseC+random8(0,32), 255, random8(128, 255)), CHSV(baseC+random8(0,32), 192, random8(128, 255)), CHSV(baseC+random8(0,32), 255, random8(128, 255)));
    }
 // EVERY_N_SECONDS(10) {                                                        // Change the target palette to a random one every 5 seconds.
// updateDisplay(gCurrentPatternNumber);
 // }
  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { 
    gHue++;
  } // slowly cycle the "base color" through the rainbow

}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void readbutton() {                                           // Read the button and increase the mode

  uint8_t b = checkButton();

  if (b == 1) {                                               // Just a click event to advance to next pattern
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
    //Serial.println(gCurrentPatternNumber);
    // updateDisplay(gCurrentPatternNumber);
  }

  if (b == 2) {                                               // A double-click event to reset to 0 pattern
    gCurrentPatternNumber = 0;
    //Serial.println(gCurrentPatternNumber);
     //updateDisplay(gCurrentPatternNumber);
  }

} // readbutton()



void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}
void matrix() {                                               // One line matrix

  if (huerot) palIndex++;
  
  if (random8(90) > 80) {
    if (thisdir == 0)
      leds[0] = ColorFromPalette(currentPalette, palIndex, thisbright, currentBlending); 
    else
      leds[NUM_LEDS-1] = ColorFromPalette( currentPalette, palIndex, thisbright, currentBlending);
  } else {
    if (thisdir == 0)
      leds[0] = CHSV(bgclr, thissat, bgbri); 
    else
      leds[NUM_LEDS-1] = CHSV(bgclr, thissat, bgbri);
  }

  if (thisdir == 0) {
    for (int i = NUM_LEDS-1; i >0 ; i-- ) leds[i] = leds[i-1];
  } else {
    for (int i = 0; i < NUM_LEDS-1 ; i++ ) leds[i] = leds[i+1];
  }
  
} // matrix()


void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
  
  uint8_t secondHand = (millis() / 1000) % 25;                // Change '25' to a different value to change length of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.

  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: thisdelay=50; palIndex=95; bgclr=140; bgbri=4; huerot=0; break;
      case  5: targetPalette = OceanColors_p; thisdir=1; bgbri=0; huerot=1; break;
      case 10: targetPalette = LavaColors_p; thisdelay=30; palIndex=0; bgclr=50; bgbri=8; huerot=0; break;
      case 15: targetPalette = ForestColors_p; thisdelay=80; bgbri = 16; bgclr=96; palIndex=random8(); break;
      case 20: palIndex=random8(); huerot=1; break;
      case 25: break;
    }
  }

} // ChangeMe()

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm2()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void fadein() {

  random16_set_seed(535);                                                           // The randomizer needs to be re-set each time through the loop in order for the 'random' numbers to be the same each time through.

  for (int i = 0; i<NUM_LEDS; i++) {
    uint8_t fader = sin8(millis()/random8(10,20));                                  // The random number for each 'i' will be the same every time.
    leds[i] = ColorFromPalette(currentPalette, i*20, fader, currentBlending);       // Now, let's run it through the palette lookup.
  }

  random16_set_seed(millis());                                                      // Re-randomizing the random number seed for other routines.

}// fadein()

void ease() {

  static uint8_t easeOutVal = 0;
  static uint8_t easeInVal  = 0;
  static uint8_t lerpVal    = 0;

  easeOutVal = ease8InOutQuad(easeInVal);                     // Start with easeInVal at 0 and then go to 255 for the full easing.
  easeInVal++;

  lerpVal = lerp8by8(0, NUM_LEDS, easeOutVal);                // Map it to the number of LED's you have.

  leds[lerpVal] = CRGB::Red;
  fadeToBlackBy(leds, NUM_LEDS, 16);                          // 8 bit, 1 = slow fade, 255 = fast fade
  
} // ease()

void beatwave() {
  
  uint8_t wave1 = beatsin8(9, 0, 255);                        // That's the same as beatsin8(9);
  uint8_t wave2 = beatsin8(8, 0, 255);
  uint8_t wave3 = beatsin8(7, 0, 255);
  uint8_t wave4 = beatsin8(6, 0, 255);

  for (int i=0; i<NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, i+wave1+wave2+wave3+wave4, 255, currentBlending); 
  }
  
} // beatwave()

void dot_beat() {

  uint8_t inner = beatsin8(bpm, NUM_LEDS/4, NUM_LEDS/4*3);    // Move 1/4 to 3/4
  uint8_t outer = beatsin8(bpm, 0, NUM_LEDS-1);               // Move entire length
  uint8_t middle = beatsin8(bpm, NUM_LEDS/3, NUM_LEDS/3*2);   // Move 1/3 to 2/3

  leds[middle] = CRGB::Purple;
  leds[inner] = CRGB::Blue;
  leds[outer] = CRGB::Aqua;

  nscale8(leds,NUM_LEDS,fadeval);                             // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadeval);

} // dot_beat()

void blendwave() {

  speed = beatsin8(6,0,255);

  clr1 = blend(CHSV(beatsin8(3,0,255),255,255), CHSV(beatsin8(4,0,255),255,255), speed);
  clr2 = blend(CHSV(beatsin8(4,0,255),255,255), CHSV(beatsin8(3,0,255),255,255), speed);

  loc1 = beatsin8(10,0,NUM_LEDS-1);
  
  fill_gradient_RGB(leds, 0, clr2, loc1, clr1);
  fill_gradient_RGB(leds, loc1, clr2, NUM_LEDS-1, clr1);

} // blendwave()

void black(){
  fill_solid(leds, NUM_LEDS, CRGB::Black);                    // Just to be sure, let's really make it BLACK.
  FastLED.show();                         // Power managed display  
}

void blur() {

  uint8_t blurAmount = dim8_raw( beatsin8(3,64, 192) );       // A sinewave at 3 Hz with values ranging from 64 to 192.
  blur1d( leds, NUM_LEDS, blurAmount);                        // Apply some blurring to whatever's already on the strip, which will eventually go black.
  
  uint8_t  i = beatsin8(  9, 0, NUM_LEDS);
  uint8_t  j = beatsin8( 7, 0, NUM_LEDS);
  uint8_t  k = beatsin8(  5, 0, NUM_LEDS);
  
  // The color of each point shifts over time, each at a different speed.
  uint16_t ms = millis();  
  leds[(i+j)/2] = CHSV( ms / 29, 200, 255);
  leds[(j+k)/2] = CHSV( ms / 41, 200, 255);
  leds[(k+i)/2] = CHSV( ms / 73, 200, 255);
  leds[(k+i+j)/3] = CHSV( ms / 53, 200, 255);
  
  FastLED.show();
  
} // blur()
 
void fill_grad() {
  
  uint8_t starthue = beatsin8(5, 0, 255);
  uint8_t endhue = beatsin8(7, 0, 255);
  
  if (starthue < endhue) {
    fill_gradient(leds, NUM_LEDS, CHSV(starthue,255,255), CHSV(endhue,255,255), FORWARD_HUES);    // If we don't have this, the colour fill will flip around. 
  } else {
    fill_gradient(leds, NUM_LEDS, CHSV(starthue,255,255), CHSV(endhue,255,255), BACKWARD_HUES);
  }
  
} // fill_grad()


void drawLogo(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(5000);
}

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


double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}