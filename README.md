# Spark Control pedal emulation for MVave Chocolate <br/>
This program runs on ESP32 C3 SuperMini, just flash and use it like the Spark Control pedal.<br/>
You can use you device MAC address editing this line<br/>
```#define MVAVE_MAC "XX:XX:XX:XX:XX:XX" ```<br/>
or it will search for a device named "FootCtrl" (default name of mvave)<br/>

Led blinking is set to: <br/>
- 500 ms if no device is connected <br/>
- 1000 ms if only Spark App is connected <br/>
- 2000 ms if only Mvave is connected  <br/>
- No BLinking if Spark App and Mvave are connected <br/>


This project is based on amazing work of Paul Hamshere (paulhamsh)  <br/>
Paul Hamshere https://github.com/paulhamsh/SparkControl <br/>
 <br/>
Thanks Paul :) <br/>
