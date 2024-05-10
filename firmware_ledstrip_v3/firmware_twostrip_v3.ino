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

// Here's how to control the LEDs from any two pins (software SPI):
#define DATAPIN_STRIP_A 8
#define CLOCKPIN_STRIP_A 9

// Declaration of DotStar objects. If you want to use the hardware SPI you must use another declaration for this object:
// You need to exclude from object istantiation the DATAPIN and CLOCKPIN values (it automatically uses SPI ones)
// Basically it becomes: Adafruit_DotStar Strip(NUMPIXELS, DOTSTAR_BRG);
// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
Adafruit_DotStar StripA(NUMPIXELS, DATAPIN_STRIP_A, CLOCKPIN_STRIP_A, DOTSTAR_BRG);


class particle  {
  public: 
    int head, tail;
    int origin_head, origin_tail;
    uint32_t color;
    char* particle_name;

    particle(int head, int width, uint32_t color, char* particle_name)
    {
      this->head = head;
      this->tail = head - width;
      this->color = color;

      this->origin_head = head;
      this->origin_tail = tail;
      this->particle_name = particle_name;

      // The head sets the led which will be lighted on; as it progress toward the strip, all the subsequent leds will be turned on
      StripA.setPixelColor(this->head, color);
      // Without a tail, which turns off all the led that it encounter in its path, all the leds will remain turned on!
      // So this perform the job of turning off the led which are behind a certain number of leds (offset) from the head.
      StripA.setPixelColor(this->tail, 0);  // 'Off' pixel at tail
      StripA.show();                  // Refresh strip
    
      // Serial.print("Instantiated particle with head, tail = ");
      // Serial.print(this->head);
      // Serial.print(this->tail);

    }

  void update() {
    // If reach the end of the strip (head>=NUMPIXELS), reset the head counter (so animation restart from the beginning of the strip)
    if (++this->head >= NUMPIXELS) {  // Increment head index.  Off end of strip?
      this->head = 0;                 //  Yes, reset head index to start
      // impact(StripA, StripB);
    }

    if (++this->tail >= NUMPIXELS) {
      this->tail = 0;  // Increment, reset tail index
    }

    StripA.setPixelColor(this->head, color);
    StripA.setPixelColor(this->tail, 0);
    StripA.show();

    // Serial.print("Update from particle:");
    // Serial.println(particle_name);

    // Serial.print("head = ");
    // Serial.println(this->head);

    // Serial.print("tail = ");
    // Serial.println(this->tail);

    // Serial.println("---------------");
    // delay(2000);
  }

};


void setup() {
  StripA.begin();  // Initialize pins for output
  StripA.show();   // Turn all LEDs off ASAP

  Serial.begin(9600); // Initialize serial communication just for debug purposes for Serial.print

}


// Each pixel color is a 32 bit unsigned integer, that is three 8 bit groups (only the last 24 bits are used, 3x8=24)
uint32_t colorON = StripA.Color(80, 180, 0);

particle particle1(0, 3, colorON, "particle 1");
particle particle2(24, 3, colorON, "particle 2");
particle particle3(48, 3, colorON, "particle 3");


void loop() {
  particle1.update();
  particle2.update();
  particle3.update();
}


void turnOffSequence(Adafruit_DotStar &strip, int start, int end) {
  for(int i=start; i<end; i++){
    strip.setPixelColor(i, 0);  // Turn off i-th led
  }
  strip.show();
}
