# relay4esp
Simple relay control using an ESP8266 and mqtt protocol

This program was written for my home and it is a simple relay control.
I use it for control the main heater in my home.

The AP mode is disabled for security reason, so if you want to change something you need to flash the firmware again. I know that it is not confortable, but how many time you need to reprogram a relay control during the years.

Anyway I would like to implement the possibility to change some of the parameters using some mqtt command. 


#Parameters
The first block contains the wifi configuration  and the access data to the mosquitto server


const char* ssid = "your_wifi";

const char* wifiPass = "your_wifipasswoed";

const char* mqttServer = "your_mqttserver";

const char* mqttUser = "your_mqttuser";

const char* mqttPass = "your_mqttpassword";

const int mqttPort = 1883;

#Mosquitto messages

The name of the device for subscription is defined here

String mqttDevice = "nameofdevice";

The messages topics are:

nameofdevice/connectionStatus  that will publish a paylod "connected!" when the ESP8266 connects succesfully at the wifi network

nameofdevice/relayCommand that will accept as paylod  0, 1 and 2 (invert the actual state)

nameofdevice/realyStatus that will publish a payload 0, 1 



