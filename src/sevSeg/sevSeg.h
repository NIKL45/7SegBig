#ifndef sevSeg_h
#define sevSeg_h

#include "Arduino.h"

/* !!! --> it's way more stable when using printChar(char) 
          (and converting the number to char in the loop), 
          instead of using printNum() <-- !!! */

class sevSeg
{
public:
  sevSeg(int A0, int A1, int A2, int A3, int A, int B, int C, int D, int DP);
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

private:
  unsigned long PreviousMillis = 0;
  const long Interval = 2;
  int Index = 0;
  const char *Number;
  int NumberWthoutDot;
  int DotPos;
  bool ErrorChar = 0;

  //PINS:
  int a0;
  int a1;
  int a2;
  int a3;
  int a;
  int b;
  int c;
  int d;
  int dp;

  //NUMBER:
  const uint8_t SEG[12] = {
      B00000, // 0   A B C D DP
      B00010, // 1
      B00100, // 2
      B00110, // 3
      B01000, // 4
      B01010, // 5
      B01100, // 6
      B01110, // 7
      B10000, // 8
      B10010, // 9
      B11111, // .
      B11110  // nothing
  };

  //DIGIT:
  const uint8_t DIGIT[8] = {
      B0000, // 0.   A3 A2 A1 A0
      B0001, // 1.
      B0010, // 2.
      B0011, // 3.
      B0100, // 4.
      B0101, // 5.
      B0110, // 6.
      B1111  // nothing
  };
};
#endif
