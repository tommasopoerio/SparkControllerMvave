#ifndef SparkComms_h
#define SparkComms_h

#ifdef CLASSIC
#include "BluetoothSerial.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#else
#include "NimBLEDevice.h"
#endif

#include "RingBuffer.h"

// Functions for logging changes for any UI updates
// Spark amp
#define SPK 0  
// Spark app                 
#define APP 1
// Buetooth MIDI controller  
#define BLE_MIDI 2
// USB Host MIDI
#define USB_MIDI 3
// Serial / DIN MIDI
#define SER_MIDI 4


#define DEBUG(x) Serial.println(x)


#define NUM_CONNS 5

#define TO 0
#define FROM 1
#define STATUS 2

bool conn_status[NUM_CONNS];
unsigned long conn_last_changed[3][NUM_CONNS];

void set_conn_status_connected(int connection);
void set_conn_status_disconnected(int connection);
void set_conn_received(int connection);
void set_conn_sent(int connection);

void TaskBlink( void *pvParameters );


#define BLE_BUFSIZE 5000

#define C_SERVICE "ffc0"
#define C_CHAR1   "ffc1"
#define C_CHAR2   "ffc2"

#define S_SERVICE "ffc0"
#define S_CHAR1   "ffc1"
#define S_CHAR2   "ffc2"


#define PEDAL_SERVICE    "03b80e5a-ede8-4b33-a751-6ce34ec4c700"
#define PEDAL_CHAR       "7772e5db-3868-4112-a1a9-f2669d106bf3"


#define MAX_SCAN_COUNT 2

bool connect_to_all();
bool connect_pedal();

bool is_ble;

bool ble_app_connected;
bool classic_app_connected;

bool connected_pedal;
bool connected_app=false;
bool blinking=false;
bool found_pedal;


bool connected_sp;
bool found_sp;


BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic_receive;
BLECharacteristic *pCharacteristic_send;


BLEAdvertising *pAdvertising;

BLEScan *pScan;
BLEScanResults pResults;
BLEAdvertisedDevice device;

BLEClient *pClient_sp;
BLERemoteService *pService_sp;
BLERemoteCharacteristic *pReceiver_sp;
BLERemoteCharacteristic *pSender_sp;
BLERemoteDescriptor* p2902_sp;
BLEAdvertisedDevice *sp_device;

BLEClient *pClient_pedal;
BLERemoteService *pService_pedal;
BLERemoteCharacteristic *pReceiver_pedal;
BLERemoteCharacteristic *pSender_pedal;
BLERemoteDescriptor* p2902_pedal;
BLEAdvertisedDevice *pedal_device;


RingBuffer midi_in;


#endif
