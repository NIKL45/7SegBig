#include "Arduino.h"
#include "sevSeg.h"

unsigned long PreviousMillis = 0;
const long Interval = 25;


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

sevSeg::sevSeg()
{
  // PINS:
#define A0 15
#define A1 14
#define A2 13
#define A3 12

#define A 4
#define B 5
#define C 18
#define D 19
#define DP 21
}

void sevSeg::init()
{
  // HEF4028 --> digit
  pinMode(A3, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A0, OUTPUT);

  // CD4511 --> number
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(DP, OUTPUT);
}

void sevSeg::clear()
{
}

void sevSeg::SegWrite(int digit, int SegNumber)
{
  ////////////////////////////
  //DIGIT:

  for (int i = 3; i >= 0; i--)
  {
    //Serial.print(bitRead(DIGIT[digit], i));
    switch (i)
    {
    case 3:
      digitalWrite(A3, bitRead(DIGIT[digit], i));
      break;
    case 2:
      digitalWrite(A2, bitRead(DIGIT[digit], i));
      break;
    case 1:
      digitalWrite(A1, bitRead(DIGIT[digit], i));
      break;
    case 0:
      digitalWrite(A0, bitRead(DIGIT[digit], i));
      break;
    }
  }

  ////////////////////////////
  //NUMBER:
  for (int i = 4; i >= 0; i--)
  {
    //Serial.print(bitRead(SEG[SegNumber], i));
    switch (i)
    {
    case 4:
      digitalWrite(D, bitRead(SEG[SegNumber], i));
      break;
    case 3:
      digitalWrite(C, bitRead(SEG[SegNumber], i));
      break;
    case 2:
      digitalWrite(B, bitRead(SEG[SegNumber], i));
      break;
    case 1:
      digitalWrite(A, bitRead(SEG[SegNumber], i));
      break;
    case 0:
      digitalWrite(DP, bitRead(SEG[SegNumber], i));
      break;
    }
  }
}

void sevSeg::print(char * Print)
{
  for (byte i = 0; i < sizeof(Print) - 1; i++)
  {
    Serial.print(Print[i]);
  }

  Serial.println(sizeof(Print));
}

void sevSeg::multiplex()
{

  if (millis() - PreviousMillis >= Interval)
  {

    PreviousMillis = millis();
  }
}

// void sevSeg::multiplex()
// {
//     if (digit != 0)
//     {
//         pinMode(Dig[digit - 1], INPUT);
//     }
//     else
//     {
//         pinMode(Dig[nbdigit - 1], INPUT);
//     }
//     if (disp)
//     {                                                             //si affichage valide
//         if (cursr && (digit == pos) && ((millis() % 1000) < 500)) //affichage du curseur
//         {
//             segwrite('_');
//         }
//         else
//         {
//             segwrite(disply[digit]);
//         } //ou du caractere
//         if (dot[digit])
//         {
//             digitalWrite(Seg[7], !common);
//         } //affichage du point
//         else
//         {
//             digitalWrite(Seg[7], common);
//         }
//     }
//     else
//     {
//         segwrite(' ');
//         digitalWrite(Seg[7], common);
//     }
//     if (driver)
//     {
//         digitalWrite(Dig[digit], !common);
//     }
//     else
//     {
//         digitalWrite(Dig[digit], common);
//     };
//     pinMode(Dig[digit], OUTPUT);
//     digit++;
//     if (digit == nbdigit)
//         digit = 0;
// }
