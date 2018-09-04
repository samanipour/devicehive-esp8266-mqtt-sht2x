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
## Modify PubSubClient.h
![alt text](https://github.com/samanipour/devicehive-esp8266-mqtt-sht2x/blob/master/screenshots/Modify%20PubSubClient.h.png)
## Modify Sodaq_SHT2x.cpp
![alt text](https://github.com/samanipour/devicehive-esp8266-mqtt-sht2x/blob/master/screenshots/Modify%20Sodaq_SHT2x.cpp.png)
# Check Data in DeviceHive admin panel
## check if device Register Successfully
![alt text](https://github.com/samanipour/devicehive-esp8266-mqtt-sht2x/blob/master/screenshots/admin%20panel1.PNG)
## see Data in notification tab
![alt text](https://github.com/samanipour/devicehive-esp8266-mqtt-sht2x/blob/master/screenshots/admin%20panel2.PNG)
# Create Grafana Dashboard for visualization
## Grafana Data source configuration
![alt text](https://github.com/samanipour/devicehive-esp8266-mqtt-sht2x/blob/master/screenshots/Data%20source%20configuration.PNG)
## Grafana Temperature graph
![alt text](https://github.com/samanipour/devicehive-esp8266-mqtt-sht2x/blob/master/screenshots/Grafana%20Temperature%20graph.PNG)
## Grafana Humidity graph
![alt text](https://github.com/samanipour/devicehive-esp8266-mqtt-sht2x/blob/master/screenshots/Grafana%20Humidity%20graph.PNG)
## Grafana Temperature 3DGauge
![alt text](https://github.com/samanipour/devicehive-esp8266-mqtt-sht2x/blob/master/screenshots/Grafana%20Temperature%20gauge.PNG)
## Grafana Humidity 3DGauge
![alt text](https://github.com/samanipour/devicehive-esp8266-mqtt-sht2x/blob/master/screenshots/Grafana%20Himidity%20gauge.PNG)
## Your Final Grafana Dashboard
![alt text](https://github.com/samanipour/devicehive-esp8266-mqtt-sht2x/blob/master/screenshots/Grafana%20dashboard.PNG)


