# devicehive-esp8266-mqtt-sht2x

 Author : Ali Samanipour
 email :alisamanipour.offical@gmail.com
 Licence:GNU GENERAL PUBLIC LICENSE v3
 whit this code you can get temperature and humidtiy  from sht2x sensore series and 
 send these data to devicehive server over mqtt protocol and probably if you want, visualize them with grafana plugin
 you should modify Sodaq_SHT2x and PubSubClient libraries befor run the code.please review documentation for more details
# Overview
this code shows , how you can connect your ESP8266ardiuno board to DeviceHive server and send temperature and humidity data over MQTT protocol.I use Wemos D1 Board with ESP8266 Chipset and SHT21 sensor to read temperature and humidity.
# How to connect sht21 sensor to Wemos D1 board
![alt text](https://github.com/samanipour/devicehive-esp8266-mqtt-sht2x/blob/master/screenshots/How%20To%20connect%20sht21%20to%20wemos%20D1.jpg)
# Modify Libraries
#  Modify PubSubClient.h
## Modify S
