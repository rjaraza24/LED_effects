#include "FastLED.h"
#include "LEDMatrix.h"

#define DATA_PIN    15
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    224

#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)



int debounce = 50; // ms debounce period to prevent flickering when pressing or releasing the button
int DCgap = 250; // max ms between clicks for a double click event
int holdTime = 1500; // ms hold period: how long to wait for press+hold event
int longHoldTime = 3000; // ms long hold period: how long to wait for press+hold event

// Other button variables
boolean buttonVal = HIGH; // value read from button
boolean buttonLast = HIGH; // buffered value of the button's previous state
boolean DCwaiting = false; // whether we're waiting for a double click (down)
boolean DConUp = false; // whether to register a double click on next release, or whether to wait and click
boolean singleOK = true; // whether it's OK to do a single click
long downTime = -1; // time the button was pressed down
long upTime = -1; // time the button was released
boolean ignoreUp = false; // whether to ignore the button release because the click+hold was triggered
boolean waitForUp = false; // when held, whether to wait for the up event
boolean holdEventPast = false; // whether or not the hold event happened already
boolean longHoldEventPast = false;// whether or not the long hold event happened already

byte br = 100; //global LED brightness
byte buttonpin = 14;

int indiehue[7]; //for MSGEQ7 equalizer FX
int loophue; //for MSGEQ7 equalizer FX
int hue; //for MSGEQ7 equalizer FX
int WY; //for MSGEQ7 equalizer FX

uint8_t huEE; //for rainbowmirror FX
int16_t counter; //for rainbowmirror FX

unsigned int aminval = 200; //thresholdvalue for MSGEQ7 audio values - everything below (noise) will be cut off

const uint8_t kMatrixWidth = 32;
const uint8_t kMatrixHeight = 7;
const bool    kMatrixSerpentineLayout = true;

int analogPin = 18; // read from multiplexer using analog input - MSGEQ7
int strobePin = 19; // strobe is attached to digital pin - MSGEQ7
int resetPin = 17; // reset is attached to digital pin - MSGEQ7

int spectrumValue[7]; //array for MSGEQ7 audio values

boolean mode = 0; //button hold event - mode 0 = cycle effects - mode 1 = equalizer
boolean hold = 0; //button longhold event - holds the current pattern/effect forever

int raw;    //battery voltage raw ADC value
float sraw;    //sensitivity raw ADC value
float voltage; //actual battery voltage

int lowvoltage = 0; //low voltage flag when battery is too low


cLEDMatrix<32, 7, HORIZONTAL_MATRIX> matrix;  //rainbow mirror FX
uint16_t PlasmaTime, PlasmaShift; //for rainbow plasma FX

static uint16_t x; //for palette FX
static uint16_t y; //for palette FX
static uint16_t z; //for palette FX
uint16_t speed = 20;  //for palette FX
uint16_t scale = 30; //for palette FX
uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];  //for palette FX
CRGBPalette16 currentPalette( PartyColors_p );  //for palette FX
uint8_t       colorLoop = 1; //for palette FX


CRGB leds[ NUM_LEDS ];

#define LAST_VISIBLE_LED 224
uint16_t XY (uint16_t x, uint16_t y) {
  // any out of bounds address maps to the first hidden pixel
  if ( (x >= kMatrixWidth) || (y >= kMatrixHeight) ) {
    return (LAST_VISIBLE_LED + 1);
  }

  const uint16_t XWYTable[] = {
   192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
   191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160,
   128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
   127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100,  99,  98,  97,  96,
    64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
    63,  62,  61,  60,  59,  58,  57,  56,  55,  54,  53,  52,  51,  50,  49,  48,  47,  46,  45,  44,  43,  42,  41,  40,  39,  38,  37,  36,  35,  34,  33,  32,
     0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31
  };

  uint16_t i = (y * kMatrixWidth) + x;
  uint16_t j = XWYTable[i];
  return j;
}

void setup() {

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  pinMode(buttonpin, INPUT_PULLUP); //declare button pin internal pullup

  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);

  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
  FastLED.setBrightness(br);
  x = random16(); //for palette FX
  y = random16(); //for palette FX
  z = random16(); //for palette FX

  PlasmaShift = (random8(0, 5) * 64) + 64;
  PlasmaTime = 0;
  hue = 0;
  counter = 0;
  //Serial.begin(500);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {plasmaFX, mirrorFX, noiseFX};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

unsigned int lastpattern;
unsigned int currentpattern;

void loop()
{

//EVERY_N_MILLISECONDS(1000){
//  sraw = analogRead(23);
//  sraw = mapf(sraw, 50, 1023, 0.25, 5.0);
//  sraw = constrain(sraw, 0.25, 5.0);
//}

  
  unsigned int currentpattern = millis();

  gHue++;

  if (currentpattern - lastpattern >= 10000) {

    if (hold == 0) {
      nextPattern();
      lastpattern = currentpattern;
    }

    raw = analogRead(17);
    voltage = raw * 0.00660;

    if (voltage <= 2.6 && voltage >= 0.5) {
      lowvoltage = 1;
    }

    PlasmaShift = (random8(0, 5) * 64) + 64;
    PlasmaTime = 0;
  }

  int b = checkButton();

  //Serial.println(voltage);

  if (b == 1) nextPattern();  //clickEvent();
  if (b == 2) brightness(); //doubleClickEvent();
  if (b == 3) mode = !mode; //holdEvent();
  if (b == 4) hold = !hold; //longHoldEvent();


  if (mode == 0 && lowvoltage == 0) {
    gPatterns[gCurrentPatternNumber]();
  }
  else if (mode == 1 && lowvoltage == 0) {
    EQ();
  }
  else {
    fadeToBlackBy( leds, NUM_LEDS, 1);
  }


  FastLED.setBrightness(br);
  FastLED.show();
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  lastpattern = millis();
}



void EQ()
{

  FastLED.clear();
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // to allow the output to settle
    spectrumValue[i] = analogRead(analogPin);
    digitalWrite(strobePin, HIGH);
  }



  loophue++;

  indiehue[0] = loophue;
  indiehue[1] = loophue + 60;
  indiehue[2] = loophue + 90;
  indiehue[3] = loophue + 180;
  indiehue[4] = loophue + 300;
  indiehue[5] = loophue + 30;
  indiehue[6] = loophue + 270;

  for (int XX = 0; XX < kMatrixWidth; XX++) {

    int l;

    switch (XX) {
      case 0:
        hue = indiehue[0];
        WY = map(spectrumValue[0], aminval, 1023, 0, kMatrixHeight);
        break;
      case 5:
        hue = indiehue[1];
        WY = map(spectrumValue[1], aminval, 1023, 0, kMatrixHeight);
        break;
      case 10:
        hue = indiehue[2];
        WY = map(spectrumValue[2], aminval, 1023, 0, kMatrixHeight);
        break;
      case 15:
        hue = indiehue[3];
        WY = map(spectrumValue[3], aminval, 1023, 0, kMatrixHeight);
        break;
      case 20:
        hue = indiehue[4];
        WY = map(spectrumValue[4], aminval, 1023, 0, kMatrixHeight);
        break;
      case 24:
        hue = indiehue[5];
        WY = map(spectrumValue[5], aminval, 1023, 0, kMatrixHeight);
        break;
      case 28:
        hue = indiehue[6];
        WY = map(spectrumValue[6], aminval, 1023, 0, kMatrixHeight);
        break;
    }

    for (l = 0; l <= WY; l++) {
      leds[XY(XX, l)] = CHSV(hue, 255, 255);
    }

  }
}


void brightness() {
  switch (br) {
    case 255:
      br = 30;
      break;

    case 30:
      br = 100;
      break;

    case 100:
      br = 255;
      break;

  }

}



void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  byte lineStartHue = startHue8;
  for ( byte y = 0; y < kMatrixHeight; y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;
    for ( byte x = 0; x < kMatrixWidth; x++) {
      pixelHue += xHueDelta8;
      leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
    }
  }
}


void noiseFX() {
  ChangePaletteAndSettingsPeriodically();
  fillnoise8();
  mapNoiseToLEDsUsingPalette();
}



#define HOLD_PALETTES_X_TIMES_AS_LONG 1

void ChangePaletteAndSettingsPeriodically()
{
  uint8_t secondHand = ((millis() / 1000) / HOLD_PALETTES_X_TIMES_AS_LONG) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand ==  0)  {
      currentPalette = RainbowColors_p;
      speed = 20;
      scale = 30;
      colorLoop = 1;
    }
    if ( secondHand ==  5)  {
      SetupPurpleAndGreenPalette();
      speed = 10;
      scale = 40;
      colorLoop = 1;
    }
    if ( secondHand == 10)  {
      SetupBlackAndWhiteStripedPalette();
      speed = 20;
      scale = 30;
      colorLoop = 1;
    }
    if ( secondHand == 15)  {
      currentPalette = PartyColors_p;
      speed = 20;
      scale = 30;
      colorLoop = 1;
    }
    if ( secondHand == 20)  {
      SetupRandomPalette();
      speed = 20;
      scale = 50;
      colorLoop = 1;
    }
    if ( secondHand == 35)  {
      currentPalette = RainbowStripeColors_p;
      speed = 20;
      scale = 20;
      colorLoop = 1;
    }
  }
}

// This function generates a random palette that's a gradient
// between four different colors.  The first is a dim hue, the second is
// a bright hue, the third is a bright pastel, and the last is
// another bright hue.  This gives some visual bright/dark variation
// which is more interesting than just a gradient of different hues.
void SetupRandomPalette()
{
  currentPalette = CRGBPalette16(
                     CHSV( 0, 0, 0),
                     CHSV( 0, 0, 0),
                     CHSV( random8(), 128, 255),
                     CHSV( random8(), 255, 255));
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     green,  green,  black,  black,
                     purple, purple, black,  black,
                     green,  green,  black,  black,
                     purple, purple, black,  black );
}


void fillnoise8() {
  // If we're runing at a low "speed", some 8-bit artifacts become visible
  // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
  // The amount of data smoothing we're doing depends on "speed".
  uint8_t dataSmoothing = 0;
  if ( speed < 50) {
    dataSmoothing = 200 - (speed * 4);
  }

  for (int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for (int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;

      uint8_t data = inoise8(x + ioffset, y + joffset, z);

      // The range of the inoise8 function is roughly 16-238.
      // These two operations expand those values out to roughly 0..255
      // You can comment them out if you want the raw noise data.
      data = qsub8(data, 16);
      data = qadd8(data, scale8(data, 39));

      if ( dataSmoothing ) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }

      noise[i][j] = data;
    }
  }

  z += speed;

  // apply slow drift to X and Y, just for visual variation.
  x += speed / 8;
  y -= speed / 16;
}

void mapNoiseToLEDsUsingPalette()
{
  static uint8_t ihue = 0;

  for (int i = 0; i < kMatrixWidth; i++) {
    for (int j = 0; j < kMatrixHeight; j++) {
      // We use the value at the (i,j) coordinate in the noise
      // array for our brightness, and the flipped value from (j,i)
      // for our pixel's index into the color palette.

      uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];

      // if this palette is a 'loop', add a slowly-changing base value
      if ( colorLoop) {
        index += ihue;
      }

      // brighten up, as the color palette itself often contains the
      // light/dark dynamic range desired
      if ( bri > 127 ) {
        bri = 255;
      } else {
        bri = dim8_raw( bri * 2);
      }

      CRGB color = ColorFromPalette( currentPalette, index, bri);
      leds[XY(i, j)] = color;
    }
  }

  ihue += 1;
}


int checkButton() {
  int event = 0;
  buttonVal = digitalRead(buttonpin);
  // Button pressed down
  if (buttonVal == LOW && buttonLast == HIGH && (millis() - upTime) > debounce)
  {
    downTime = millis();
    ignoreUp = false;
    waitForUp = false;
    singleOK = true;
    holdEventPast = false;
    longHoldEventPast = false;
    if ((millis() - upTime) < DCgap && DConUp == false && DCwaiting == true)  DConUp = true;
    else  DConUp = false;
    DCwaiting = false;
  }
  // Button released
  else if (buttonVal == HIGH && buttonLast == LOW && (millis() - downTime) > debounce)
  {
    if (not ignoreUp)
    {
      upTime = millis();
      if (DConUp == false) DCwaiting = true;
      else
      {
        event = 2;
        DConUp = false;
        DCwaiting = false;
        singleOK = false;
      }
    }
  }
  // Test for normal click event: DCgap expired
  if ( buttonVal == HIGH && (millis() - upTime) >= DCgap && DCwaiting == true && DConUp == false && singleOK == true && event != 2)
  {
    event = 1;
    DCwaiting = false;
  }



  // Test for hold
  if (buttonVal == LOW && (millis() - downTime) >= holdTime) {
    // Trigger "normal" hold
    if (not holdEventPast)
    {
      event = 3;
      waitForUp = true;
      ignoreUp = true;
      DConUp = false;
      DCwaiting = false;
      //downTime = millis();
      holdEventPast = true;
    }

    if ((millis() - downTime) >= longHoldTime)
    {
      if (not longHoldEventPast)
      {
        event = 4;
        longHoldEventPast = true;
      }
    }


  }
  buttonLast = buttonVal;
  return event;
}




void plasmaFX() {
  uint16_t OldPlasmaTime;

  HuePlasmaFrame(PlasmaTime);
  OldPlasmaTime = PlasmaTime;
  PlasmaTime += PlasmaShift;
  if (OldPlasmaTime > PlasmaTime)
    PlasmaShift = (random8(0, 5) * 32) + 64;
}


void HuePlasmaFrame(uint16_t Time) {
#define PLASMA_X_FACTOR  24
#define PLASMA_Y_FACTOR  24
  int16_t r, h;
  int x, y;

  for (x = 0; x < kMatrixWidth; x++)
  {
    for (y = 0; y < kMatrixHeight; y++)
    {
      r = sin16(Time) / 256;
      h = sin16(x * r * PLASMA_X_FACTOR + Time) + cos16(y * (-r) * PLASMA_Y_FACTOR + Time) + sin16(y * x * (cos16(-Time) / 256) / 2);
      leds[XY(x, y)] = CHSV((h / 256) + 128, 255, 255);
    }
  }
}


void mirrorFX() {
  int16_t sx, sy, x, y;
  uint8_t h;
  FastLED.addLeds<WS2812B, 3, GRB>(matrix[0], matrix.Size());

  h = hue;
  if (counter < 500)
  {
    // ** Fill LED's with diagonal stripes
    for (x = 0; x < (matrix.Width() + matrix.Height()); ++x)
    {
      matrix.DrawLine(x - matrix.Height(), matrix.Height() - 1, x, 0, CHSV(h, 255, 255));
      h += 16;
    }
  }
  else
  {
    // ** Fill LED's with horizontal stripes
    for (y = 0; y < matrix.Height(); ++y)
    {
      matrix.DrawLine(0, y, matrix.Width() - 1, y, CHSV(h, 255, 255));
      h += 16;
    }
  }
  hue += 7;

  if (counter < 250)
    matrix.HorizontalMirror();
  else if (counter < 500)
    matrix.QuadrantMirror();

  else if (counter < 750)
    matrix.VerticalMirror();
  else if (counter < 1000)
    matrix.TriangleBottomMirror();
  else if (counter < 1250)
    matrix.QuadrantTopTriangleMirror();

  counter++;
  if (counter >= 1250)
    counter = 0;

}
