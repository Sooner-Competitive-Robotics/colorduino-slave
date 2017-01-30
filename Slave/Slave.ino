#include <Colorduino.h>
#include <Wire.h>

void setup() {
  /// Set up the Colorduino stuff
  Colorduino.Init();
  unsigned char whiteBalance[3] = {36, 63, 63}; // {R, G, B}
  Colorduino.SetWhiteBal(whiteBalance);
  /// Set up wiring stuff
  // Addresses 0-7 are reserved, so the first one we can use is 8 (https://www.arduino.cc/en/Reference/Wire)
  Wire.begin(8); // Join the I2C bus with address #8
  Wire.onReceive(handler);
  // Important: there are both 7- and 8-bit versions of I2C addresses, and Wire uses the 7-bit form.
  // Thus, drop the low bit (shift right one) if you're using 8 bits elsewhere.
}

void loop() {
  // Don't actually do anything
}

void handler(int numberOfBytes) {

  /// Let's document this here, I guess. The message we get over I2C is formatted thusly:
  //
  // | 1 | 2 | 3 | 4 | 5 |
  // |---|---|---|---|---|
  // | x | y | r | g | b |
  //
  // Where (x, y) is the coordinate of the pixel (measuring from the bottom left)
  // and (r, g, b) are the red, green, and blue components of the color.

  if(numberOfBytes != 5) { // This clearly isn't a message we can understand; ignore it.
    return;
  }
  int x = Wire.read();
  int y = Wire.read();
  int r = Wire.read();
  int g = Wire.read();
  int b = Wire.read();
  setPixel(x, y, r, g, b);
}

void setPixel(int x, int y, int r, int g, int b) {
   Colorduino.SetPixel(x, y, r, g, b); // This may seem silly, but it makes life easier for the wrapping
   Colorduino.FlipPage();
}

