/* Radio MMI
 *
 * by Daniel Hjort, 2014.
 */

#ifndef RadioMMI_h
#define RadioMMI_h

#include <inttypes.h>

#include <Encoder.h>
#include <LCD.h>

typedef enum {
  NOT_PRESSED,
  PRESSED,
  LONG_PRESSED
} button_state_t;

typedef enum {
  CHANGE_FREQ,
  CHANGE_RIT,
  MENU
} mmi_state_t;

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

  bool buttonPressed();

  void updateUi();
  
private:
   
   void printFreq(long freq);

   void printRit(int rit);

   void onRotateLeft();

   void onRotateRight();

   void onPressed();

   void onLongPressed();

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

   button_state_t mButtonState;
   long mButtonDownSince;

   mmi_state_t mMMIState;
};

#endif
