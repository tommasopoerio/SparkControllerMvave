// USES SPARK BOX TO EMULATE A SPARK CONTROL
// Library dependenct: NimBLE 1.4.1
// Edit NimBLEDevice.cpp
// Probably here: C:\Users\XXXXX\Documents\Arduino\libraries\NimBLE-Arduino\src\NimBLEDevice.cpp

/*
  #ifdef SPARK_CONTROL
  uint8_t                     NimBLEDevice::m_own_addr_type = BLE_OWN_ADDR_RANDOM;
  #else
  uint8_t                     NimBLEDevice::m_own_addr_type = BLE_OWN_ADDR_PUBLIC;
  #endif
*/



// this triggers the changes in NimBLEDevice.spp
#define SPARK_CONTROL

#define ESP_DEVKIT
//#define MVAVE_MAC "XX:XX:XX:XX:XX:XX"   //if not defined it will scan for a bluetooth device with 'FootCtrl' name



#ifdef HELTEC
#include "heltec.h"
#endif

#ifdef M5CORE2
#include <M5Core2.h>
#endif

#ifdef M5CORE
#include <M5Stack.h>
#endif

#ifdef M5STICK
#include <M5StickC.h>
#endif

#include "SparkComms.h"
#include "MIDI.h"


//Define some variables
int iCompressor = 1;
int iDrive = 1;
int iModulation = 1;
int iDelay = 1;
int iAmp = 1;
int EQLevel = 0; //range from 0 to 1
int EQ200 = 0; 
int EQ400 = 0;
int EQ800 = 0;
int EQ1600 = 0;
int  EQ3200 = 0;
int EQ32 = 0;



void SparkControlStart();
void SparkControlLoop();

TaskHandle_t blink_task_handle; 


void setup() {
// general setup
#ifdef HELTEC
  Heltec.begin(true , false , true );
  Heltec.display->setFont(ArialMT_Plain_24); // 10, 16 and 24
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Spk Control");
  Heltec.display->display();
#elif defined M5CORE2 || defined M5CORE || defined M5STICK
  M5.begin();
#ifdef M5CORE
  M5.Power.begin();
#endif
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.println("Spk Control");
#else
  Serial.begin(115200);
#endif

setup_midi();


  pinMode(PIN_NEOPIXEL, OUTPUT);

  launch_blink_thread();

  SparkControlStart();

  
}

void loop() {

    SparkControlLoop();
  
}
