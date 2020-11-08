#ifndef sevSeg_h
#define sevSeg_h

#include "Arduino.h"

class sevSeg
{
public:
  sevSeg();
  void print(char * Print);
  void SegWrite(int digit, int SegNumber);
  void init();
  void clear();
  void multiplex();
};
#endif
