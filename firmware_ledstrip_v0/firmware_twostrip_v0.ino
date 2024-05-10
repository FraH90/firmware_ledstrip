// By limiting the number and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!
// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

#include <Adafruit_DotStar.h>
#include <SPI.h>

// Number of LEDs in strip
#define NUMPIXELS 72

// Here's how to control the LEDs from any two pins:
#define DATAPIN_STRIP_A 4
#define CLOCKPIN_STRIP_A 5

#define DATAPIN_STRIP_B 6
#define CLOCKPIN_STRIP_B 7

// Declaration of DotStar objects. If you want to use the hardware SPI you must use another declaration for this object:
// You need to exclude from object istantiation the DATAPIN and CLOCKPIN values (it automatically uses SPI ones)
// Basically it becomes: Adafruit_DotStar Strip(NUMPIXELS, DOTSTAR_BRG);
// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
Adafruit_DotStar StripA(NUMPIXELS, DATAPIN_STRIP_A, CLOCKPIN_STRIP_A, DOTSTAR_BRG);
Adafruit_DotStar StripB(NUMPIXELS, DATAPIN_STRIP_B, CLOCKPIN_STRIP_B, DOTSTAR_BRG);

// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

void setup() {
  StripA.begin();  // Initialize pins for output
  StripA.show();   // Turn all LEDs off ASAP

  StripB.begin();  // Initialize pins for output
  StripB.show();   // Turn all LEDs off ASAP
}

// Index of first 'on' and 'off' pixels
int head = 0;
int width = 5;
int tail = head - width;

// 'On' color (starts red). G, R, B
// Each pixel color is a 32 bit unsigned integer, that is three 8 bit groups (only the last 24 bits are used, 3x8=24)
uint32_t init_color = StripA.Color(0, 255, 0);
uint32_t color = init_color;


void loop() {

  StripA.setPixelColor(head, color);  // 'On' pixel at head
  StripA.setPixelColor(tail, 0);      // 'Off' pixel at tail
  StripA.show();                      // Refresh strip

  StripA.setPixelColor(head, color);  // 'On' pixel at head
  StripA.setPixelColor(tail, 0);      // 'Off' pixel at tail
  StripA.show();                      // Refresh strip

  delay(1);  // Pause 20 milliseconds (~50 FPS)

  // If reach the end of the strip (head>=NUMPIXELS), reset the head counter (so animation restart from the beginning of the strip)
  // and change color by performing a binary shift of 8 positions; if color is equal to zero
  if (++head >= NUMPIXELS) {  // Increment head index.  Off end of strip?
    head = 0;                 //  Yes, reset head index to start
    if ((color >>= 8) == 0)   //  Next color (R->G->B) ... past blue now?
      color = 0xFF0000;       //   Yes, reset to red
  }

  if (++tail >= NUMPIXELS) {
    tail = 0;  // Increment, reset tail index
  }
}
