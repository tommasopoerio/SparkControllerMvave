#include "MIDI.h"

// MIDI State processing for BLE streams

void MIDIState::initialise(RingBuffer *rb)
{
  midi_stream = rb;
  midi_data_count = 0;
  midi_cmd_count = 0;
  midi_status = 0;
}

bool MIDIState::process(byte mi_data[3]) 
{
  byte b;
  bool got_data;

  got_data = false;
  
  if (!midi_stream->is_empty()) {
    midi_stream->get(&b);
    
    if (b <= 0x7F) {
      midi_data[midi_data_count] = b;
      midi_data_count++;
      midi_cmd_count = 0;
      if ((midi_status == 0xC0 || midi_status == 0xD0) || midi_data_count >= 2) {
        mi_data[0] = midi_status;
        mi_data[1] = midi_data[0];
        if (midi_data_count == 2)
          mi_data[2] = midi_data[1];
        else
          mi_data[2] = 0;
        midi_data_count = 0;
        got_data = true;
      }
    } 
    else {
      midi_cmd_count++;
      if (midi_cmd_count > 1) {
        midi_status = b;            // the last byte before a non-cmd byte will always be status unless it was a single timestamp
      }
    }
  } 
  return got_data;
}

MIDIState ble_app;
MIDIState ble_midi;

void setup_midi() {
  byte b;

#ifdef USB_HOST
  if (Usb.Init() == -1) {
    DEBUG("USB host init failed");
    usb_connected = false;
  }
  else {
    DEBUG("USB host running");
    usb_connected = true;   
  }
#endif
  

#ifdef USB_TRINKET
  ser2 = new HardwareSerial(2); 
  ser2->begin(115200, SERIAL_8N1, SER2_RX, SER2_TX);
  while (ser2->available())    
    b = ser2->read();
#endif

  
  ble_midi.initialise(&midi_in);
}


bool update_midi(byte *mid) {

  bool got_midi;
  byte b;
  
  got_midi = false;


  if (ble_midi.process(mid)) got_midi = true;


  if (got_midi) {
    Serial.print("MIDI ");
    Serial.print(mid[0], HEX);
    Serial.print(" ");
    Serial.print(mid[1]);
    Serial.print(" ");   
    Serial.println(mid[2]);
  }
  return got_midi;
}
