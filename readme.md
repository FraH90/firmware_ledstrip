# Adafruit DotStar Project

## Links
- [Ultimate guide to addressable led strips](https://www.ledyilighting.com/it/the-ultimate-guide-to-addressable-led-strip/)
- [RS Online Order](https://it.rs-online.com/web/p/strisce-led/1245482?gb=s)
- [Official Adafruit Product page](https://www.adafruit.com/product/2328)
- [Official Adafruit DotStar Guide](https://www.ledyilighting.com/it/the-ultimate-guide-to-addressable-led-strip/)

## Connections
Strip: connect them to the 5V, 28A output
Arduino: 12V output via DC Jack (don't use othe voltage inputs)
Put together the GND of the Arduino, the external power supply, and the dotstar strips
Generally total power drawn is 0.5A, so you can just power everything with a +5V power supply.
Led strips require 5V voltage (no more); Arduino can be powered using 5V only via USB (this bypass the LDO)

Strip connections:
- Black: ground
- Red: +5V
- Yellow: Clock Input   (arduino pin 5 for stripA, pin 7 for stripB)
- Green: Data Input     (arduino pin 4 for stripA, pin 6 for stripB)


## Version description
- v0: simple effect, only one particle, one strip, no impact
- v1: two strip, one particle for each, different colors (red and green); added impact
- v2: deleted impact; still single particle, but only one color, orange, still uses two software SPI (separated channels for stripA, stripB)
- v3: now using only a single channel for the two strips (it speeds up running); implemented class particle; instantiated three particles on each strip.