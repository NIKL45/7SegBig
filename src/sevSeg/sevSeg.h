#ifndef sevSeg_h
#define sevSeg_h

#include "Arduino.h"

/* !!! --> it's way more stable when using printChar(char) 
          (and converting the number to char in the loop), 
          instead of using printNum() <-- !!! */

class sevSeg
{
public:
  sevSeg();
  void printChar(char *Print);
  void printNum(float Num);
  void printNum(long Num);
  void printNum(unsigned long Num);
  void printNum(short Num);
  void printNum(double Num);
  void printNum(int Num);
  void printNum(unsigned int Num);
  void SegWrite(int digit, int SegNumber);
  void init();
  void clear();
  void multiplex();
};
#endif
