/* VFO
 *
 * library by Daniel Hjort, 2014.
 *
 * Very slightly modified by George Smart, M1GEO - 12 Feb 2012.
 * http://www,george-smart.co.uk/
 *
 * Originally by Peter Marks http://marxy.org
 * Fully. here: http://blog.marxy.org/2008/05/controlling-ad9851-dds-with-arduino.html
 */

#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include "VFO.h"

// DDS Reference Oscilliator Frequency, in Hz. (Remember, the PLL).
#define DDS_REF 180000000

// DDS/Arduino Connections
#define DDS_LOAD  8
#define DDS_CLOCK 9
#define DDS_DATA  10

void VFO::begin(unsigned long frequency)
{
  // Set all pins to output states
  pinMode (DDS_DATA,  OUTPUT);
  pinMode (DDS_CLOCK, OUTPUT);
  pinMode (DDS_LOAD,  OUTPUT);
  
  setFrequency(frequency);
}

void VFO::setFrequency(unsigned long frequency) {
  unsigned long tuning_word = (frequency * pow(2, 32)) / DDS_REF;
  digitalWrite (DDS_LOAD, LOW); // take load pin low

  for(int i = 0; i < 32; i++) {
    if ((tuning_word & 1) == 1)
      outOne();
    else
      outZero();
    tuning_word = tuning_word >> 1;
  }
  byte_out(0x09);
  digitalWrite (DDS_LOAD, HIGH); // Take load pin high again
}

void VFO::byte_out(unsigned char byte) {
  int i;

  for (i = 0; i < 8; i++) {
    if ((byte & 1) == 1)
      outOne();
    else
      outZero();
    byte = byte >> 1;
  }
}

void VFO::outOne() {
  digitalWrite(DDS_CLOCK, LOW);
  digitalWrite(DDS_DATA, HIGH);
  digitalWrite(DDS_CLOCK, HIGH);
  digitalWrite(DDS_DATA, LOW);
}

void VFO::outZero() {
  digitalWrite(DDS_CLOCK, LOW);
  digitalWrite(DDS_DATA, LOW);
  digitalWrite(DDS_CLOCK, HIGH);
}
