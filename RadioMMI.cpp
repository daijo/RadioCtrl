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

     mIsPaddleConnected = digitalRead(ditPin);

     mUpdate = true;

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
        mStore.freq += mStore.step;
        mEncoderPosition = newPos;
        mUpdate = true;
      } else if(newPos < mEncoderPosition) {
        mStore.freq -= mStore.step;
        mEncoderPosition = newPos;
        mUpdate = true;
      }
    }
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
    mLcd->setCursor(0, 1);
    mLcd->print("RIT +0.625 kHz"); //TODO: fix

    EEPROM_writeAnything(0, mStore);

    mUpdate = false;
  }
}

/*
 * Private
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

