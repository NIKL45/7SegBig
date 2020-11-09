/////////////////////////////////////
//                                 //
//       Big 7-Segment Display     //
//       _   _   _   _   _   _     //
//   _  |_| |_| |_| |_| |_| |_|    //
//     .|_|.|_|.|_|.|_|.|_|.|_|    //
//                                 //
/////////////////////////////////////

/* 
 Sources:

 - Multitasking: https://github.com/SensorsIot/ESP32-Dual-Core
 - https://github.com/sigvaldm/SevenSeg

 */

#include <Arduino.h>
#include "sevSeg/sevSeg.h" /* !!! --> it's way more stable when using printChar(char) 
                              (and converting the number to char in the loop), 
                              instead of using printNum() <-- !!! */

sevSeg Display;

void TaskWifi(void *parameters);
void TaskDisplay(void *parameters);

TaskHandle_t Task0, Task1;
//SemaphoreHandle_t baton; // for task synchronization

/***********************/

//DEBUG:
const bool debug = 0;

//Variable to pass between cores:
float pass = 1234.56;

//-----------------------------------------------------------//
///////////////////////////////////////////////////////////////
//-----------------------------------------------------------//

void setup()
{

  if (debug)
  {
    Serial.begin(115200);
  }

  /***********************/

  //baton = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(
      TaskWifi // fuction to run
      ,
      "TaskWifi" // A name just for humans
      ,
      1024 // This stack size can be checked & adjusted by reading the Stack Highwater
      ,
      NULL // parameter of the task
      ,
      1 // Priority, with 3 being the highest, and 0 being the lowest.
      ,
      &Task0 // Task handle to keep track of created task
      ,
      0); // Core

  delay(500); // needed to start-up task0

  xTaskCreatePinnedToCore(TaskDisplay, "TaskDisplay", 1024, NULL, 1, &Task1, 1);

  /***********************/

  Display.init();
}

//-----------------------------------------------------------//

void loop() // (CORE 1)
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskDisplay(void *parameters) // (CORE 1)
{
  (void)parameters;
  //Code here runs once (like the normal setup)

  //TESTING:
  //float x = -15;

  ////////////////////////

  for (;;) // loop
  {
    char Tmp[100];
    String TmpStr = String(pass);
    //x = x + 0.00001;
    TmpStr.toCharArray(Tmp, 100);

    Display.printChar(Tmp);
    //Display.printNum(pass);
    Display.multiplex();
  }
}

/***********************/

void TaskWifi(void *parameters) // (CORE 0)
{
  (void)parameters;
  //Code here runs once (like the normal setup)

  

  ////////////////////////

  for (;;) // loop (A Task shall never return or exit.)
  {
    delay(10);
  }
}