//////////////////////////////////////////////////////////////
//
//
//      Big 7-Segment Display ( -.8.8.8.8.8.8 9 )
//
//
//////////////////////////////////////////////////////////////

#include <Arduino.h>
#include "sevSeg/sevSeg.h"

sevSeg Display;

void TaskWifi(void *parameters);
void TaskDisplay(void *parameters);

TaskHandle_t Task0, Task1;
SemaphoreHandle_t baton;

/***********************/

//DEBUG:
const bool debug = 0;

//TESTING:
float x;

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

  baton = xSemaphoreCreateMutex();

  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(
      TaskWifi, "TaskWifi" // A name just for humans
      ,
      1024 // This stack size can be checked & adjusted by reading the Stack Highwater
      ,
      NULL /* parameter of the task */
      ,
      1 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,
      &Task0 /* Task handle to keep track of created task */
      ,
      0); /* Core */

  delay(500); // needed to start-up task0

  xTaskCreatePinnedToCore(TaskDisplay, "TaskDisplay", 1024, NULL, 1, &Task1, 1);

  /***********************/

  Display.init();
}

//-----------------------------------------------------------//

void loop()
{

  // Empty. Things are done in Tasks.

  // moved to TaskDisplay:

  // char Tmp[100];
  // String TmpStr = String(1.1 * x);
  // x = x + 0.00001;
  // TmpStr.toCharArray(Tmp, 100);
  // Display.printChar(Tmp);
  // Display.multiplex();
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskDisplay(void *parameters) // This is a task.
{
  (void)parameters;
  //Code here runs once (like the normal setup)

  for (;;)
  {
    char Tmp[100];
    String TmpStr = String(1.1 * x);
    x = x + 0.00001;
    TmpStr.toCharArray(Tmp, 100);

    Display.printChar(Tmp);
    Display.multiplex();
    //vTaskDelay(10); // one tick delay (15ms) in between for stability
  }
}

/***********************/

void TaskWifi(void *parameters) // This is a task.
{
  (void)parameters;
  //Code here runs once (like the normal setup)

  for (;;) // A Task shall never return or exit.
  {
  }
}