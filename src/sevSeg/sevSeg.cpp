#include "Arduino.h"
#include "sevSeg.h"

/////  Variables defined as private in sevSeg.h
//
// unsigned long PreviousMillis = 0;
// const long Interval = 2;
// int Index = 0;
// const char *Number;
// int NumberWthoutDot;
// int DotPos;
// bool ErrorChar = 0;

// int a0;
// int a1;
// int a2;
// int a3;
// int a;
// int b;
// int c;
// int d;
// int dp;

// //NUMBER:
// const uint8_t SEG[12] = {
//     B00000, // 0   A B C D DP
//     B00010, // 1
//     B00100, // 2
//     B00110, // 3
//     B01000, // 4
//     B01010, // 5
//     B01100, // 6
//     B01110, // 7
//     B10000, // 8
//     B10010, // 9
//     B11111, // .
//     B11110  // nothing
// };

// //DIGIT:
// const uint8_t DIGIT[8] = {
//     B0000, // 0.   A3 A2 A1 A0
//     B0001, // 1.
//     B0010, // 2.
//     B0011, // 3.
//     B0100, // 4.
//     B0101, // 5.
//     B0110, // 6.
//     B1111  // nothing
// };

//--------------------------------------------------//
//////////////////////////////////////////////////////
//--------------------------------------------------//

sevSeg::sevSeg(int A0, int A1, int A2, int A3, int A, int B, int C, int D, int DP)
{
  // PINS:
 a0 = A0;
 a1 = A1;
 a2 = A2;
 a3 = A3;

 a = A;
 b = B;
 c = C;
 d = D;
 dp = DP;
}

//--------------------------------------------------//

void sevSeg::init()
{
  // HEF4028 --> digit
  pinMode(a3, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(a0, OUTPUT);

  // CD4511 --> number
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(dp, OUTPUT);
}

//--------------------------------------------------//

void sevSeg::clear()
{
  Number = "";
}

//--------------------------------------------------//

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
      digitalWrite(a3, bitRead(DIGIT[digit], i));
      break;
    case 2:
      digitalWrite(a2, bitRead(DIGIT[digit], i));
      break;
    case 1:
      digitalWrite(a1, bitRead(DIGIT[digit], i));
      break;
    case 0:
      digitalWrite(a0, bitRead(DIGIT[digit], i));
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
      digitalWrite(d, bitRead(SEG[SegNumber], i));
      break;
    case 3:
      digitalWrite(c, bitRead(SEG[SegNumber], i));
      break;
    case 2:
      digitalWrite(b, bitRead(SEG[SegNumber], i));
      break;
    case 1:
      digitalWrite(a, bitRead(SEG[SegNumber], i));
      break;
    case 0:
      digitalWrite(dp, bitRead(SEG[SegNumber], i));
      break;
    }
  }
}

//--------------------------------------------------//
//--------------------------------------------------//
//--------------------------------------------------//

/* !!! --> it's way more stable when using printChar(char) 
          (and converting the number to char in the loop), 
          instead of using printNum() <-- !!! */

void sevSeg::printNum(float Num)
{
  char Tmp[100];
  String TmpStr = String(Num);
  TmpStr.toCharArray(Tmp, 100);
  sevSeg::printChar(Tmp);
}

//--------------------------------------------------//

void sevSeg::printNum(long Num)
{
  char Tmp[100];
  String TmpStr = String(Num);
  TmpStr.toCharArray(Tmp, 100);
  sevSeg::printChar(Tmp);
}

//--------------------------------------------------//

void sevSeg::printNum(unsigned long Num)
{
  char Tmp[100];
  String TmpStr = String(Num);
  TmpStr.toCharArray(Tmp, 100);
  sevSeg::printChar(Tmp);
}

//--------------------------------------------------//

void sevSeg::printNum(short Num)
{
  char Tmp[100];
  String TmpStr = String(Num);
  TmpStr.toCharArray(Tmp, 100);
  sevSeg::printChar(Tmp);
}

//--------------------------------------------------//

void sevSeg::printNum(double Num)
{
  char Tmp[100];
  String TmpStr = String(Num);
  TmpStr.toCharArray(Tmp, 100);
  sevSeg::printChar(Tmp);
}

//--------------------------------------------------//
// only on point allowed
void sevSeg::printNum(int Num)
{
  char Tmp[100];
  String TmpStr = String(Num);
  TmpStr.toCharArray(Tmp, 100);
  sevSeg::printChar(Tmp);
}

//--------------------------------------------------//

void sevSeg::printNum(unsigned int Num)
{
  char Tmp[100];
  String TmpStr = String(Num);
  TmpStr.toCharArray(Tmp, 100);
  sevSeg::printChar(Tmp);
}

//--------------------------------------------------//

// !!! only one point allowed !!!
void sevSeg::printChar(char *Print)
{

  NumberWthoutDot = strlen(Print);
  for (int i = 0; i < strlen(Print); i++)
  {
    if (Print[i] == '.')
    {
      NumberWthoutDot = NumberWthoutDot - 1;
      DotPos = i;
    }

    if (Print[i] == '0' || Print[i] == '1' || Print[i] == '2' || Print[i] == '3' || Print[i] == '4' || Print[i] == '5' || Print[i] == '6' || Print[i] == '7' || Print[i] == '8' || Print[i] == '9' || Print[i] == '-' || Print[i] == '.' || Print[i] == ' ')
    {
      ErrorChar = 0;
    }
    else
    {
      ErrorChar = 1;
    }
  }

  if ((NumberWthoutDot > 6 && Print[0] != '-') || (NumberWthoutDot > 7 && Print[0] == '-') || (NumberWthoutDot < strlen(Print) - 1))
  {
    Number = "-......"; //Overflow
  }
  else if (ErrorChar == 1)
  {
    Number = " ......"; //unknown character
  }
  else
  {
    Number = Print;
  }
}

//--------------------------------------------------//

void sevSeg::multiplex()
{

  if (millis() - PreviousMillis >= Interval)
  {
    if (Number == "")
    {
      SegWrite(0, 11);
    }
    else if (Number[0] == '-')
    {

      SegWrite(6, 8);
    }

    if (Number[Index] == '0')
    {
      if (Index <= DotPos)
      {
        SegWrite((strlen(Number) - Index - 1) - 1, 0);
      }
      else
      {
        SegWrite((strlen(Number) - Index) - 1, 0);
      }
    }
    else
    {
      if (Number[Index] == '1')
      {
        if (Index <= DotPos)
        {
          SegWrite((strlen(Number) - Index - 1) - 1, 1);
        }
        else
        {
          SegWrite((strlen(Number) - Index) - 1, 1);
        }
      }
      else
      {
        if (Number[Index] == '2')
        {
          if (Index <= DotPos)
          {
            SegWrite((strlen(Number) - Index - 1) - 1, 2);
          }
          else
          {
            SegWrite((strlen(Number) - Index) - 1, 2);
          }
        }
        else
        {
          if (Number[Index] == '3')
          {
            if (Index <= DotPos)
            {
              SegWrite((strlen(Number) - Index - 1) - 1, 3);
            }
            else
            {
              SegWrite((strlen(Number) - Index) - 1, 3);
            }
          }
          else
          {
            if (Number[Index] == '4')
            {
              if (Index <= DotPos)
              {
                SegWrite((strlen(Number) - Index - 1) - 1, 4);
              }
              else
              {
                SegWrite((strlen(Number) - Index) - 1, 4);
              }
            }
            else
            {
              if (Number[Index] == '5')
              {
                if (Index <= DotPos)
                {
                  SegWrite((strlen(Number) - Index - 1) - 1, 5);
                }
                else
                {
                  SegWrite((strlen(Number) - Index) - 1, 5);
                }
              }
              else
              {
                if (Number[Index] == '6')
                {
                  if (Index <= DotPos)
                  {
                    SegWrite((strlen(Number) - Index - 1) - 1, 6);
                  }
                  else
                  {
                    SegWrite((strlen(Number) - Index) - 1, 6);
                  }
                }
                else
                {
                  if (Number[Index] == '7')
                  {
                    if (Index <= DotPos)
                    {
                      SegWrite((strlen(Number) - Index - 1) - 1, 7);
                    }
                    else
                    {
                      SegWrite((strlen(Number) - Index) - 1, 7);
                    }
                  }
                  else
                  {
                    if (Number[Index] == '8')
                    {
                      if (Index <= DotPos)
                      {
                        SegWrite((strlen(Number) - Index - 1) - 1, 8);
                      }
                      else
                      {
                        SegWrite((strlen(Number) - Index) - 1, 8);
                      }
                    }
                    else
                    {
                      if (Number[Index] == '9')
                      {
                        if (Index <= DotPos)
                        {
                          SegWrite((strlen(Number) - Index - 1) - 1, 9);
                        }
                        else
                        {
                          SegWrite((strlen(Number) - Index) - 1, 9);
                        }
                      }
                      else
                      {
                        if (Number[Index] == '.')
                        {
                          SegWrite((strlen(Number) - Index) - 1, 10);
                        }
                        else
                        {
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    ////////////////////////////////

    if (NumberWthoutDot > 6 && Number[0] != '-' && Index <= 1)
    {
      Index = 6;
    }
    else if (NumberWthoutDot > 7 && Number[0] == '-' && Index <= 1)
    {
      Index = 7;
    }
    else if (Index <= 0)
    {
      Index = (strlen(Number) - 1);
    }
    else
    {
      Index--;
    }
    //Serial.println(Index);
    PreviousMillis = millis();
  }
}