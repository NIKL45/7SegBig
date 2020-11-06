//////////////////////////////////////////////////////////////
//
//
//      Big 7-Segment Display ( -.8.8.8.8.8.8 9 )
//
//
//////////////////////////////////////////////////////////////

#include <Arduino.h>
//#include <MsTimer2.h>

/***********************/

// PINS:
#define a0 15
#define a1 14
#define a2 13
#define a3 12

#define A 4
#define B 5
#define C 18
#define D 19
#define DP 21

//DEBUG:
const bool debug = 1;

//
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

//-----------------------------------------------------------//

void interrupt()
{
  //afficheur.multiplex();
}

//-----------------------------------------------------------//

void setup()
{
  //MsTimer2::set(5, interrupt); // 50Hz
  //MsTimer2::start();

  if (debug)
  {
    Serial.begin(115200);
  }

  // HEF4028 --> digit
  pinMode(a3, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(a0, OUTPUT);

  // CD4511 --> number
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(DP, OUTPUT);

  // static test digit
  digitalWrite(a3, LOW);
  digitalWrite(a2, LOW);
  digitalWrite(a1, LOW);
  digitalWrite(a0, HIGH);
}

//-----------------------------------------------------------//

void loop()
{
  for (int a = 0; a <= 11; a++)
  {
    Serial.print(a);
    Serial.print("\t");
    for (int i = 4; i >= 0; i--)
    {

      Serial.print(bitRead(SEG[a], i));

      switch (i)
      {
      case 4:
        digitalWrite(D, bitRead(SEG[a], i));
        break;

      case 3:
        digitalWrite(C, bitRead(SEG[a], i));
        break;

      case 2:
        digitalWrite(B, bitRead(SEG[a], i));
        break;

      case 1:
        digitalWrite(A, bitRead(SEG[a], i));
        break;

      case 0:
        digitalWrite(DP, bitRead(SEG[a], i));
        break;
      }
    }
    Serial.println("______________");
    delay(700);
  }
  delay(500);
}


//-----------------------------------------------------------//

//-----------------------------------------------------------//