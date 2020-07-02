// ArrayOfLedArrays - see https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples for more info on
// using multiple controllers.  In this example, we're going to set up three NEOPIXEL strips on three
// different pins, each strip getting its own CRGB array to be played with, only this time they're going
// to be all parts of an array of arrays.

//#include "FastLED.h"

//#define NUM_STRIPS 3
//#define NUM_LEDS_PER_STRIP 30
////CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];
//
//// For mirroring strips, all the "special" stuff happens just in setup.  We
//// just addLeds multiple times, once for each strip
//void setup() {
//  // tell FastLED there's 60 NEOPIXEL leds on pin 10
//  FastLED.addLeds<NEOPIXEL, 3>(leds[0], NUM_LEDS_PER_STRIP);
//
//  // tell FastLED there's 60 NEOPIXEL leds on pin 11
//  FastLED.addLeds<NEOPIXEL, 7>(leds[1], NUM_LEDS_PER_STRIP);
//
//  // tell FastLED there's 60 NEOPIXEL leds on pin 12
//  FastLED.addLeds<NEOPIXEL, 12>(leds[2], NUM_LEDS_PER_STRIP);
//
//}
int ls = 15;
#include "FastLED.h"
#define NUM_LEDS 180 //milkLED
#define NUM_LEDS2 180 //barLED
  CRGB leds[NUM_LEDS]; //milk
  CRGB leds2[NUM_LEDS]; //milk2

#define PIN 14 //5 pcb
#define PIN2 17  //11 pcb

 

void setup()
{
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2811, PIN2, GRB>(leds2, NUM_LEDS).setCorrection( TypicalLEDStrip );

}

// *** REPLACE FROM HERE ***
void loop() {

  ///RED-ORANGE-YELLOW-BLUE-GREEN
  delay(500);
  colorWipe(0xdd,0xaa,0xdd,ls );//RED
  colorWipe(0x00,0x00,0x00, ls);
  colorWipe(0xdd,0xaa,0xdd,ls );//ORANGE
////  colorWipe(0x00,0x00,0x00, 15);
//  colorWipe(0x00,0xaa,0x00, ls);//BLUE
////  colorWipe(0x00,0x00,0x00, 15);
//  colorWipe(0x00,0x00,0xaa, ls);//GREEN
//  //  colorWipe(0x00,0x00,0x00, 15);
//    colorWipe(0xaa,0xaa,0x00, ls);//YELLOW

//  colorWipe(0x00,0x00,0x00, 15);
  delay(500);
}

// ---> here we define the effect function <---
// *** REPLACE TO HERE ***

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
   leds2[Pixel].r = red;
   leds2[Pixel].g = green;
   leds2[Pixel].b = blue;

 #endif
}

void setPixel2(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red - red;
   leds[Pixel].g = red - green;
   leds[Pixel].b = red - blue;
   leds2[Pixel].r = red - red;
   leds2[Pixel].g = red - green;
   leds2[Pixel].b = red - blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel2(i, red, green, blue); 
  }
  showStrip();
}



void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
      setPixel(i, red, green, blue);
      showStrip();
      delay(SpeedDelay);
  }
}
