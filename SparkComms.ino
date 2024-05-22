#include "SparkComms.h"

const uint8_t notifyOn[] = {0x1, 0x0};
NimBLEAddress pedal_addr;

BLEUUID PedalServiceUuid(PEDAL_SERVICE);


class PedalClientCallback : public BLEClientCallbacks
{
  void onConnect(BLEClient *pclient)
  {
    DEBUG("callback: pedal connected");
    connected_pedal=true;
  }

  void onDisconnect(BLEClient *pclient)
  {
        connected_pedal = false;         
    DEBUG("callback: pedal disconnected");   
  }
};



void notifyCB_pedal(BLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify){

  int i;
  byte b;

  for (i = 0; i < length; i++) {
    b = pData[i];
    midi_in.add(b);
  }
  midi_in.commit();

  set_conn_received(BLE_MIDI);  
}


bool connect_pedal() {
Serial.println("Connecting pedal ...");

#ifdef  MVAVE_MAC
pedal_addr=   NimBLEAddress(MVAVE_MAC); 
#endif

if(!pedal_addr){
   Serial.println(" Scanning device with 'FootCtrl' name...");
    NimBLEScan *pScan = NimBLEDevice::getScan();
    pScan->setActiveScan(true);
    NimBLEScanResults results = pScan->start(10);

    for(int i = 0; i < results.getCount(); i++) {
        NimBLEAdvertisedDevice advertisedDevice = results.getDevice(i);
        
        if(advertisedDevice.getName()=="FootCtrl"){
      Serial.println(" Device found");
      Serial.print(" Device address: ");
      std::string mvaveAddress=advertisedDevice.getAddress().toString();
      Serial.println(mvaveAddress.c_str());
      pedal_addr=  NimBLEAddress(mvaveAddress); 

     }
    }
pScan->stop();

if(!pedal_addr){
   Serial.print(" Device address not found ");
  return false;
    }    
}
    if (pClient_pedal->connect(pedal_addr, true)) {
       connected_pedal = true;

      pService_pedal = pClient_pedal->getService(PedalServiceUuid);
      if (pService_pedal != nullptr) {
        pReceiver_pedal = pService_pedal->getCharacteristic(PEDAL_CHAR);

        if (pReceiver_pedal && pReceiver_pedal->canNotify()) {

          if (!pReceiver_pedal->subscribe(true, notifyCB_pedal, true)) {
            connected_pedal = false;
            DEBUG("Pedal disconnected");
            NimBLEDevice::deleteClient(pClient_pedal);
          } 
        }
      }
      DEBUG("connect_pedal(): pedal connected");
      set_conn_status_connected(BLE_MIDI);

      return true;
    }else{
         DEBUG("cannot connect to pedal");
       return false;
    }
  
   DEBUG("cannot connect to pedal");
  return false;
}



bool connect_to_all() {
  int i, j;
  int counts;
  uint8_t b;
  unsigned long t;

  connected_pedal = false;
  found_pedal = false;

  // set up connection status tracking array
  t = millis();
  for (i = 0; i < NUM_CONNS; i++) {
    conn_status[i] = false;
    for (j = 0; j < 3; j++)
      conn_last_changed[j][i] = t;
  }

  is_ble = true;

  pClient_pedal = BLEDevice::createClient();
  pClient_pedal->setClientCallbacks(new PedalClientCallback());

    while (!connect_pedal()) {  
    DEBUG("No pedal found , turn on pedal and set in pairing mode");
  delay(3000);                     
  }

  return true;
}




void set_conn_received(int connection) {
  conn_last_changed[FROM][connection] = millis();
}

void set_conn_sent(int connection) {
  conn_last_changed[TO][connection] = millis();
}

void set_conn_status_connected(int connection) {
  if (conn_status[connection] == false) {
    conn_status[connection] = true;
    conn_last_changed[STATUS][connection] = millis();
  }
}

void set_conn_status_disconnected(int connection) {
  if (conn_status[connection] == true) {
    conn_status[connection] = false;
    conn_last_changed[STATUS][connection] = millis();
  }
}

void TaskBlink(void *pvParameters){  
  Serial.println("START blinking LED");

  uint32_t blink_delay = 1000;

  while (!connected_pedal || !connected_app ){ 
    if(!connected_pedal && !connected_app)
    blink_delay=500;    //delay not connected_app and pedal
    else if(!connected_app )
    blink_delay=2000; //delay not connected_app only
    else
    blink_delay=1000; //delay not connected_pedal only


    digitalWrite(PIN_NEOPIXEL, HIGH);   
    delay(blink_delay);
    digitalWrite(PIN_NEOPIXEL, LOW);    
    delay(blink_delay);
  }
   Serial.print("STOP blinking LED");
  blinking=false;

  vTaskDelete(NULL); // Delete task
    
}

void launch_blink_thread() {
      if(!blinking){
        blinking=true;
      
     xTaskCreatePinnedToCore(
    TaskBlink
    ,  "Task Blink" 
    ,  2048        
    ,  NULL
    ,  NULL 
    ,  &blink_task_handle  
    , 2 
    );
    }
}


