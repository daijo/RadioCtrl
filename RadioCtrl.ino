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

bool ditDown();
bool dahDown();
bool keyDown();

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
  Serial.println("Radio Control v1.0");

  // Read presisted values
  EEPROM_readAnything(0, vfo);

  // Set defaults
  if (vfo.freq == 0) {
    vfo.freq = 7000000;
  }

  if (vfo.step == 0) {
    vfo.step = 100;
  }
}

long position  = -999;

void loop() {

  long newPos;
  newPos = knob.read();
  if (newPos != position) {

Serial.print("new pos ");
Serial.println(newPos);

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

    lcd.setCursor(3, 0);
    lcd.print("7,020.225 kHz");
    lcd.setCursor(0, 1);
    lcd.print("RIT +0.625 kHz");

    EEPROM_writeAnything(0, vfo);
    mUpdate = false;
  }

  // Test paddle
  Serial.print(" ");
  Serial.print(ditDown());
  Serial.print(dahDown());
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

/* VFO */
