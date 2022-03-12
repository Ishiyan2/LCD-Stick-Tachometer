volatile unsigned long timeA = 0;
volatile unsigned long timeB = 0;
volatile unsigned long timeC = 0;
volatile unsigned long rev = 0;
volatile unsigned long rev_old = 0;
volatile unsigned long hysteresis = 100;


#define LED 4                                 // LED Board Control 
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, LED, NEO_GRB + NEO_KHZ800);

#define MAX_VAL 15                            // 0 to 255 for brightness
#define WAN_VAL 40                            // Warnig for brightness

void setup(){
  pinMode(2,INPUT);
  attachInterrupt(0, calc, RISING);

  strip.begin();
  rainbowCycle();
  delay(1000);
  colorWipe(strip.Color(0, 0, 0)); // Black
}

void loop() {
  while ( micros() < timeC + 100000 ) { }
  timeC - micros();
  if ( rev > rev_old ) {
    rev_old = rev;
  } else if (rev  > rev_old - hysteresis ) {
    rev = rev_old;
  } else {
    rev_old = rev; 
  }
  flush_LED();
}

void calc() {
  long revA;
  long timeC;
  timeC=micros();
  boolean check;
  check=true;
  while (micros()<timeC+1000) {
    if ( digitalRead(2) == LOW ) { check = false; }
  }
  if ( check == true ) {
    timeB=micros();
    revA = 30000000/(timeB-timeA);
    timeA = timeB;
    if (revA > 500 & revA < 7000 ) {
        rev=revA;
    }
  }
}

void flush_LED() {
    if (rev > 7000 ) {
      //pass
    } else if (rev > 6750)  {
      colorWipe(strip.Color(WAN_VAL, 0, 0));        // Red Warning
      delay(100);
      colorWipe(strip.Color(0, 0, 0));              // Black
      delay(100);
    } else {
      for ( int i = 0 ; i<8 ; i++ ) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
      if (rev>=3000) { strip.setPixelColor(0, strip.Color(0, MAX_VAL, 0));  }
      if (rev>=3500) { strip.setPixelColor(1, strip.Color(0, MAX_VAL, 0));  }
      if (rev>=4000) { strip.setPixelColor(2, strip.Color(MAX_VAL, MAX_VAL, 0)); }
      if (rev>=4500) { strip.setPixelColor(3, strip.Color(MAX_VAL, MAX_VAL, 0)); }
      if (rev>=5000) { strip.setPixelColor(4, strip.Color(MAX_VAL, MAX_VAL/2, 0));  }
      if (rev>=5500) { strip.setPixelColor(5, strip.Color(MAX_VAL, MAX_VAL/2, 0));  }
      if (rev>=6000) { strip.setPixelColor(6, strip.Color(WAN_VAL, 0, 0));  }
      if (rev>=6500) { strip.setPixelColor(7, strip.Color(WAN_VAL, 0, 0));  }
      strip.show();
    }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
  }
}

void rainbow() {
  uint16_t i, j;
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle() {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color((255 - WheelPos * 3) * MAX_VAL/255, 0, (WheelPos * 3) * MAX_VAL/255);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3 * MAX_VAL/255, (255 - WheelPos * 3) * MAX_VAL/255);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3 * MAX_VAL/255, (255 - WheelPos * 3) * MAX_VAL/255 , 0);
}
