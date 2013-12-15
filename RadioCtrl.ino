/* Radio Controller
 *
 * by Daniel Hjort, 2013.
 */

#include <Encoder.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

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

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

void setup() {

  lcd.begin (16,2);
  
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();                   // go home

  lcd.print("RadioCtrl v0.1");  
  lcd.setCursor ( 0, 1 );        // go to the 2nd line
  lcd.print("by Daniel Hjort");

  Serial.begin(9600);
  Serial.println("Radio Control");
}

long position  = -999;

void loop() {

  long newPos;
  newPos = knob.read();
  if (newPos != position) {
    lcd.print("Pos = ");
    lcd.print(newPos);
    position = newPos;
  }

  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset knob to zero");
    knob.write(0);
  }
}
