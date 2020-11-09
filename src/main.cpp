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

#include <PubSubClient.h>
#include <WiFi.h>
#include <credentials.h>

#define WIFI_SSID mySSID_2
#define WIFI_PASSWORD myPASSWORD_2

char mqttServer[] = "192.168.2.205";
int mqttPort = 1883;
char deviceId[] = "Display";
char topic[] = "/Corona/Display";
char mqttUser[] = myMQTT;
char mqttPassword[] = myMQTTPASS;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastOperation;
const long MIN_OPERATION_INTERVAL = 600L;

/***********************/

#include "sevSeg/sevSeg.h" /* !!! --> it's way more stable when using printChar(char) 
                              (and converting the number to char in the loop), 
                              instead of using printNum() <-- !!! */

sevSeg Display(15, 14, 13, 12, 4, 5, 18, 19, 21);

/***********************/

void TaskWifi(void *parameters);
void TaskDisplay(void *parameters);

TaskHandle_t Task0, Task1;
SemaphoreHandle_t baton; // for task synchronization

/***********************/

//DEBUG:
const bool debug = 0;

//Variable to pass between cores:
String pass = ".      ";

//-----------------------------------------------------------//
///////////////////////////////////////////////////////////////
//-----------------------------------------------------------//

void callback(char *topic, byte *payload, unsigned int length)
{

  signed long now = millis();
  long deltaTime = now - lastOperation;

  if (deltaTime > MIN_OPERATION_INTERVAL)
  {
    String message = "";
    for (unsigned int i = 0; i < length; i++)
    {
      message = message + (char)payload[i];
    }

    //////////////////////////
    xSemaphoreTake(baton, portMAX_DELAY);
    pass = message;
    xSemaphoreGive(baton);
  }

  else
  {
    Serial.println("Operation denied right now(busy)");
  }
}

//-----------------------------------------------------------//

void connect()
{
  while (!client.connected())
  {
    status = WiFi.status();
    if (status != WL_CONNECTED)
    {
      WiFi.mode(WIFI_STA);
      //WiFi.config(ip, gateway, subnet);
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED)
      {
        delay(500);
        Serial.print(".");
      }
      Serial.print("Connected to ");
      Serial.println(WIFI_SSID);
    }
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    if (client.connect(deviceId, mqttUser, mqttPassword))
    {
      client.subscribe(topic);
      Serial.println("Connected to MQTT Server");
    }
    else
    {
      Serial.print("[FAILED] [ rc = ");
      Serial.print(client.state());
      Serial.println(" : retrying in 5 seconds]");
      delay(5000);
    }
  }
}

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

  xTaskCreatePinnedToCore(
      TaskWifi // fuction to run
      ,
      "TaskWifi" // A name just for humans
      ,
      5000 // This stack size can be checked & adjusted by reading the Stack Highwater
      ,
      NULL // parameter of the task
      ,
      1 // Priority, with 3 being the highest, and 0 being the lowest.
      ,
      &Task0 // Task handle to keep track of created task
      ,
      0); // Core

  delay(400); // needed to start-up task0

  xTaskCreatePinnedToCore(TaskDisplay, "TaskDisplay", 1024, NULL, 1, &Task1, 1);
}

//-----------------------------------------------------------//

void loop() // (CORE 1)
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskDisplay(void *parameters)
{
  (void)parameters;

  ////////-- SETUP --/////////
  Display.init();

  //Testing:
  //float x = -15;

  ////////-- LOOP --//////////
  for (;;)
  {
    char Tmp[100];
    //String TmpStr = String(x);
    //x = x + 0.00001;

    xSemaphoreTake(baton, portMAX_DELAY);
    pass.toCharArray(Tmp, 100);
    xSemaphoreGive(baton);

    Display.clear();
    Display.printChar(Tmp);
    Display.multiplex();
    //Serial.println(pass);
    vTaskDelay(1);
  }
}

/***********************/

void TaskWifi(void *parameters)
{
  (void)parameters;
  ////////-- SETUP --/////////

  ////////-- LOOP --//////////
  for (;;)
  {
    if (!client.connected())
    {
      connect();
    }
    client.loop();
    vTaskDelay(1000);
  }
}