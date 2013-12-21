/* Radio Controller
 *
 * by Daniel Hjort, 2013.
 */

#include <EEPROM.h>
#include "EEPROMAnything.h"
#include <Encoder.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

bool mUpdate = false;
bool mInTx = false;

struct vfo_t
{
  int first;
  long freq;
  int rit;
  int step;
} vfo;

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder knob(3, 4);

#define I2C_ADDR    0x27  // Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

#define DIT_PIN 6
#define DAH_PIN 7

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

struct keyer_t
{
  bool isPaddle;
  int speed;
} keyer;

long position;

bool ditDown();
bool dahDown();
bool keyDown();

void printFreq(long freq);

void setup() {

  // Setup keyer
  pinMode(DIT_PIN, INPUT);      // set pin to input
  digitalWrite(DIT_PIN, HIGH);  // turn on pullup resistors
  pinMode(DAH_PIN, INPUT);      // set pin to input
  digitalWrite(DAH_PIN, HIGH);  // turn on pullup resistors

  keyer.isPaddle = digitalRead(DIT_PIN);

  // Setup LCD
  lcd.begin (16,2);
  
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home();                 // go home

  lcd.print("RadioCtrl v0.1");
  lcd.setCursor(0, 1);        // go to the 2nd line
  lcd.print("by Daniel Hjort");

  Serial.begin(9600);
  Serial.println("RadioCtrl v1.0");
  Serial.println("by Daniel Hjort");

  // Read presisted values
  EEPROM_readAnything(0, vfo);

  // Set defaults
  if (vfo.first != 666) {
    vfo.first = 666;
    vfo.freq = 7000000;
    vfo.step = 100;
  }

  position = knob.read();
}

void loop() {

  long newPos;
  newPos = knob.read();
  if (newPos != position) {
    if (0 == newPos % 4) {
      if (newPos > position) {
        vfo.freq += vfo.step;
        position = newPos;
        mUpdate = true;
      } else if(newPos < position) {
        vfo.freq -= vfo.step;
        position = newPos;
        mUpdate = true;
      }
    }
  }

  if (Serial.available()) {

    // Read command
    //Serial.read();
  }

  if (mUpdate) {
    lcd.clear();

    if (mInTx) {
      lcd.print("TX");
    } else {
      lcd.print("RX");
    }

    printFreq(vfo.freq);
    lcd.setCursor(0, 1);
    lcd.print("RIT +0.625 kHz");

    EEPROM_writeAnything(0, vfo);
    mUpdate = false;

    Serial.print("freq: ");
    Serial.print(vfo.freq);
    Serial.println(" Hz");
  }

  // Test paddle
  //Serial.print(" ");
  //Serial.print(ditDown());
  //Serial.print(dahDown());

  if (keyer.isPaddle) {
    // Update keyer state
  } else {
    if (ditDown()) {
      // key down
      mInTx = true;
      mUpdate = true;
    } else {
      // key up
      mInTx = false;
      mUpdate = true;
    }
  }
}

/* Keyer */

bool ditDown()
{
  return !digitalRead(DIT_PIN);
}

bool dahDown()
{
  return !digitalRead(DAH_PIN);
}

/* Sidetone */

/* UI */

void printFreq(long freq)
{
    int MHz = vfo.freq / 1000000;
    int kHz = vfo.freq / 1000 - MHz * 1000;
    int Hz = vfo.freq % 1000;

    lcd.setCursor(3, 0);
    lcd.print(MHz);
    lcd.print(",");
    if (kHz <= 9) {
      lcd.print("00");
    } else if (kHz <= 99) {
      lcd.print("0");
    }
    lcd.print(kHz);
    lcd.print(".");
    if (Hz <= 9) {
      lcd.print("00");
    } else if (Hz <= 99) {
      lcd.print("0");
    }
    lcd.print(Hz);
    lcd.print(" kHz");
}

/* VFO */
