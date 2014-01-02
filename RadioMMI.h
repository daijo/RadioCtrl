/* Radio MMI
 *
 * by Daniel Hjort, 2014.
 */

#ifndef RadioMMI_h
#define RadioMMI_h

#include <inttypes.h>

#include <Encoder.h>
#include <LCD.h>

class RadioMMI
{
public:
  
  void begin(LCD* lcd, Encoder* encoder, int ditPin, int dahPin);

  void readInput();

  void setIsInTx(bool inTx);
  
  void setFreq(long freq);

  long getFreq();

  void setRit(int rit);

  int getRit();

  bool isPaddleConnected();

  bool ditDown();

  bool dahDown();

  bool keyDown();

  void updateUi();
  
private:
   
   void printFreq(long freq);

   bool mUpdate;

   LCD* mLcd;
   Encoder* mEncoder;

   int mDitPin;
   int mDahPin;

   long mEncoderPosition;

   bool mIsInTx;
   bool mIsPaddleConnected;

   struct store_t
   {
       int first;
       long freq;
       int rit;
       int step;
   } mStore;

};

#endif
