# relay4esp
Simple relay control using an ESP8266 and mqtt protocol

This program was written for my home and it is a simple relay control.
I use it for control the main heater in my home.

The AP mode is disabled for security reason, so if you want to change something you need to flash the firmware again. I know that it is not confortable, but how many time you need to reprogram a relay control during the years.

Anyway I would like to implement the possibility to change some of the parameters using some mqtt command. 

The first block contains the wifi configuration  and the access data to the mosquitto server

const char* ssid = "your_wifi";
const char* wifiPass = "your_wifipasswoed";
const char* mqttServer = "your_mqttserver";
const char* mqttUser = "your_mqttuser";
const char* mqttPass = "your_mqttpassword";
const int mqttPort = 1883;


