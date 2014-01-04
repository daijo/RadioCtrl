/* Radio MMI
 *
 * by Daniel Hjort, 2014.
 */

#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include "RadioMMI.h"
#include <EEPROM.h>
#include "EEPROMAnything.h"

#define BUTTON_PIN 5

void RadioMMI::begin(LCD* lcd, Encoder* encoder, int ditPin, int dahPin)
{
     mLcd = lcd;
     mEncoder = encoder;

     mDitPin = ditPin;
     mDahPin = dahPin;

     pinMode(ditPin, INPUT);      // set pin to input
     digitalWrite(ditPin, HIGH);  // turn on pullup resistors
     pinMode(dahPin, INPUT);      // set pin to input
     digitalWrite(dahPin, HIGH);  // turn on pullup resistors
     pinMode(BUTTON_PIN, INPUT);      // set pin to input
     digitalWrite(BUTTON_PIN, HIGH);  // turn on pullup resistors

     mIsPaddleConnected = digitalRead(ditPin);

     mUpdate = true;
     mMMIState = CHANGE_FREQ;

     mEncoderPosition = encoder->read();

     // Read presisted values
     EEPROM_readAnything(0, mStore);

     // Set defaults
     if (mStore.first != 666) {
         mStore.first = 666;
         mStore.freq = 7000000;
         mStore.step = 100;
     }
}

void RadioMMI::readInput()
{
  long newPos;
  newPos = mEncoder->read();
  if (newPos != mEncoderPosition) {
    if (0 == newPos % 4) {
      if (newPos > mEncoderPosition) {
        mEncoderPosition = newPos;
        onRotateRight();
      } else if(newPos < mEncoderPosition) {
        mEncoderPosition = newPos;
        onRotateLeft();
      }
    }
  }

  if (!buttonPressed()) {
    mButtonState = NOT_PRESSED;
  } else if (buttonPressed()
      && mButtonState == NOT_PRESSED) {
    mButtonState = PRESSED;
    mButtonDownSince = millis();
    onPressed();
  } else if (buttonPressed()
      && mButtonState == PRESSED
      && ((millis() - mButtonDownSince) > 1000)) {
    mButtonState = LONG_PRESSED;
    onLongPressed();
  }
}

void RadioMMI::setIsInTx(bool inTx)
{
    if (mIsInTx != inTx) {
        
        mIsInTx = inTx;
        mUpdate =  true;
    }
}

void RadioMMI::setFreq(long freq)
{
    if (mStore.freq != freq) {
        
        mStore.freq = freq;
        mUpdate =  true;
    }
}

long RadioMMI::getFreq()
{
    return mStore.freq;
}

void RadioMMI::setRit(int rit)
{
    if (mStore.rit != rit) {
        
        mStore.rit = rit;
        mUpdate =  true;
    }
}

int RadioMMI::getRit()
{
    return mStore.rit;
}

bool RadioMMI::isPaddleConnected()
{
    return mIsPaddleConnected;
}

bool RadioMMI::ditDown()
{
    return !digitalRead(mDitPin);
}

bool RadioMMI::dahDown()
{
    return !digitalRead(mDahPin);
}

bool RadioMMI::keyDown()
{
    return ditDown();
}

bool RadioMMI::buttonPressed()
{
    return !digitalRead(BUTTON_PIN);
}


void RadioMMI::updateUi()
{
  if (mUpdate) {
    mLcd->clear();

    if (mIsInTx) {
      mLcd->print("TX");
    } else {
      mLcd->print("RX");
    }

    printFreq(mStore.freq);
    printRit(mStore.rit);

    EEPROM_writeAnything(0, mStore);

    mUpdate = false;
  }
}

/*
 * Private - UI
 */

void RadioMMI::printFreq(long freq)
{
    int MHz = freq / 1000000;
    int kHz = freq / 1000 - MHz * 1000;
    int Hz = freq % 1000;

    mLcd->setCursor(3, 0);
    mLcd->print(MHz);
    mLcd->print(",");
    if (kHz <= 9) {
      mLcd->print("00");
    } else if (kHz <= 99) {
      mLcd->print("0");
    }
    mLcd->print(kHz);
    mLcd->print(".");
    if (Hz <= 9) {
      mLcd->print("00");
    } else if (Hz <= 99) {
      mLcd->print("0");
    }
    mLcd->print(Hz);
    mLcd->print(" kHz");
}

void RadioMMI::printRit(int rit)
{
    int kHz = rit / 1000;
    int Hz = rit % 1000;

    mLcd->setCursor(0, 1);
    mLcd->print("RIT ");

    if (rit >= 0) {
      mLcd->print("+");
    } else if (rit > -1000) {
      mLcd->print("-");
    }

    mLcd->print(kHz);
    mLcd->print(".");
    if (Hz <= 9 && Hz >= -9) {
      mLcd->print("00");
    } else if (Hz <= 99 && Hz >= -99) {
      mLcd->print("0");
    }
    
    if (Hz >= 0) {
      mLcd->print(Hz);
    } else {
      mLcd->print(-Hz);
    }
    mLcd->print(" kHz");
}

/*
 *  Private - Events
 */

void RadioMMI::onRotateLeft()
{
    if (mMMIState == CHANGE_FREQ) {
      mStore.freq -= mStore.step;
    } else if (mMMIState == CHANGE_RIT) {
      mStore.rit -= mStore.step;
    }
    mUpdate = true;
}

void RadioMMI::onRotateRight()
{
    if (mMMIState == CHANGE_FREQ) {
      mStore.freq += mStore.step;
    } else if (mMMIState == CHANGE_RIT) {
      mStore.rit += mStore.step;
    }
    mUpdate = true;
}

void RadioMMI::onPressed()
{
    if (mMMIState == CHANGE_FREQ) {
      mMMIState = CHANGE_RIT;
    } else if (mMMIState == CHANGE_RIT) {
      mMMIState = CHANGE_FREQ;
    }
}

void RadioMMI::onLongPressed()
{

}
