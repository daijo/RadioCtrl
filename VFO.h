/* VFO
 *
 * by Daniel Hjort, 2014.
 */

#ifndef VFO_h
#define VFO_h

#include <inttypes.h>

class VFO
{
public:
   
   void begin(unsigned long frequency);
   
   void setFrequency(unsigned long frequency);
   
private:
   
   void byte_out(unsigned char byte);
   
   void outOne();
   
   void outZero();
};

#endif
