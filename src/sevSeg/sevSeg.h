#ifndef sevSeg_h
#define sevSeg_h

#include "Arduino.h"

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
