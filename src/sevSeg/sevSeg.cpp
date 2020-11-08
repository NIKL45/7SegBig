#include "Arduino.h"
#include "sevSeg.h"

unsigned long PreviousMillis = 0;
const long Interval = 2;
int Index = 0;
const char *Number;
int NumberOnlyLen;
int DotPos;

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

// ToDo: void sevSeg::print(double )
// char Tmp[18];
// String TmpStr = String(double );
// TmpStr.toCharArray(Tmp, 18);

void sevSeg::printNum(float Num)
{
  char Tmp[100];
  String TmpStr = String(Num);
  TmpStr.toCharArray(Tmp, 100);
  sevSeg::printChar(Tmp);
}

void sevSeg::printNum(long Num)
{
  char Tmp[100];
  String TmpStr = String(Num);
  TmpStr.toCharArray(Tmp, 100);
  sevSeg::printChar(Tmp);
}

void sevSeg::printNum(unsigned long Num)
{
  char Tmp[100];
  String TmpStr = String(Num);
  TmpStr.toCharArray(Tmp, 100);
  sevSeg::printChar(Tmp);
}

void sevSeg::printNum(short Num)
{
  char Tmp[100];
  String TmpStr = String(Num);
  TmpStr.toCharArray(Tmp, 100);
  sevSeg::printChar(Tmp);
}

void sevSeg::printNum(double Num)
{
  char Tmp[100];
  String TmpStr = String(Num);
  TmpStr.toCharArray(Tmp, 100);
  sevSeg::printChar(Tmp);
}

void sevSeg::printNum(int Num)
{
  char Tmp[100];
  String TmpStr = String(Num);
  TmpStr.toCharArray(Tmp, 100);
  sevSeg::printChar(Tmp);
}

void sevSeg::printNum(unsigned int Num)
{
  char Tmp[100];
  String TmpStr = String(Num);
  TmpStr.toCharArray(Tmp, 100);
  sevSeg::printChar(Tmp);
}


void sevSeg::printChar(char *Print)
{

  NumberOnlyLen = strlen(Print);
  for (int i = 0; i <= strlen(Print); i++)
  {
    if (Print[i] == '.')
    {
      NumberOnlyLen = NumberOnlyLen - 1;
      DotPos = i;
    }
  }

  if ((NumberOnlyLen > 6 && Print[0] != '-') || (NumberOnlyLen > 7 && Print[0] == '-') || (NumberOnlyLen < strlen(Print) - 1))
  {
    Number = "-......";
  }
  else
  {
    Number = Print;
  }

  // Serial.print("len: ");
  // Serial.println(strlen(Number));
}

void sevSeg::multiplex()
{

  if (millis() - PreviousMillis >= Interval)
  {
    if (Number[0] == '-')
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
                          // if (Number[Index] == ' ')
                          // {
                          //   SegWrite((strlen(Number) - Index) - 1, 11);
                          // }
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

    if (NumberOnlyLen > 6 && Number[0] != '-' && Index <= 1)
    {
      Index = 6;
    }
    else if (NumberOnlyLen > 7 && Number[0] == '-' && Index <= 1)
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
