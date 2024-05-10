// By limiting the number and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!
// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

#include <Adafruit_DotStar.h>
#include <SPI.h>
#include <math.h>
#include <stdio.h>

// Number of LEDs in strip
#define NUMPIXELS 72

// Here's how to control the LEDs from any two pins:
#define DATAPIN_STRIP_A 8
#define CLOCKPIN_STRIP_A 9

#define DATAPIN_STRIP_B 10
#define CLOCKPIN_STRIP_B 11

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

  Serial.begin(9600); // Initialize serial communication just for debug purposes for Serial.print
}

// Index of first 'on' and 'off' pixels
int head = 0;
int width = 5;
int tail = head - width;

// 'On' color (starts red). G, R, B
// Each pixel color is a 32 bit unsigned integer, that is three 8 bit groups (only the last 24 bits are used, 3x8=24)
uint32_t colorA = StripA.Color(0, 255, 0);
uint32_t colorB = StripB.Color(255, 9, 9);

void loop() {

  // The head sets the led which will be lighted on; as it progress toward the strip, all the subsequent leds will be turned on
  StripA.setPixelColor(head, colorA);
  // Without a tail, which turns off all the led that it encounter in its path, all the leds will remain turned on!
  // So this perform the job of turning off the led which are behind a certain number of leds (offset) from the head.
  StripA.setPixelColor(tail, 0);  // 'Off' pixel at tail
  StripA.show();                  // Refresh strip

  StripB.setPixelColor(head, colorB);
  StripB.setPixelColor(tail, 0);
  StripB.show();

  // If reach the end of the strip (head>=NUMPIXELS), reset the head counter (so animation restart from the beginning of the strip)
  if (++head >= NUMPIXELS) {  // Increment head index.  Off end of strip?
    head = 0;                 //  Yes, reset head index to start
    impact(StripA, StripB);
  }

  if (++tail >= NUMPIXELS) {
    tail = 0;  // Increment, reset tail index
  }
}

void turnOffSequence(Adafruit_DotStar &strip, int start, int end) {
  for(int i=start; i<end; i++){
    strip.setPixelColor(i, 0);  // Turn off i-th led
  }
  strip.show();
}

void impact(Adafruit_DotStar &StripA, Adafruit_DotStar &StripB) {
  turnOffSequence(StripA, 0, 72);
  turnOffSequence(StripB, 0, 72);

  // Define the starting position of the white light at the intersection
  int intersection = NUMPIXELS - 1;

  // Define the maximum radius of the expanding white light
  int maxRadius = 14;

  // Loop to expand the white light from the intersection
  for (int radius = 0; radius < maxRadius; radius++) {
    // Loop through the LEDs around the intersection within the current radius
    for (int i = -radius; i <= radius; i++) {
      // Calculate the LED index on StripA
      int indexA = intersection + i;

      // Calculate the LED index on StripB
      int indexB = intersection - i;

      /*
      Serial.println("----------------------");
      Serial.print("Radius = ");
      Serial.println(radius);
      Serial.print("i = ");
      Serial.println(i);
      Serial.print("indexA = ");
      Serial.println(indexA);
      Serial.print("indexB = ");
      Serial.println(indexB);
      Serial.println();
      */

      // Ensure the LED index is within the range of the strips
      if (indexA >= 0 && indexA < NUMPIXELS) {
        // Set the LED color on StripA to white  
        StripA.setPixelColor(indexA, StripA.Color(80, 180, 0));
      }
      if (indexB >= 0 && indexB < NUMPIXELS) {
        // Set the LED color on StripB to white
        StripB.setPixelColor(indexB, StripB.Color(80, 180, 0));
      }
    }

    // Show the updated colors on both strips
    StripA.show();
    StripB.show();

    // Delay to control the expansion speed
    delay(30);
  }

  // After the expansion, clear the white light
  for (int i = 0; i < NUMPIXELS; i++) {
    StripA.setPixelColor(i, 0);
    StripB.setPixelColor(i, 0);
  }

  // Show the strips with cleared colors
  StripA.show();
  StripB.show();

  delay(50);

}