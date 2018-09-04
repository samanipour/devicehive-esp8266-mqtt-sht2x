/*
 * Author : Ali Samanipour
 * email :alisamanipour.offical@gmail.com
 * Licence:GNU GENERAL PUBLIC LICENSE v3
 * whit this code you can get temperature and humidtiy  from sht2x sensore series and 
 * send these data to devicehive server over mqtt protocol and probably if you want, visualize them with grafana plugin
 * you should modify Sodaq_SHT2x and PubSubClient libraries befor run the code.please review documentation for more details
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Sodaq_SHT2x.h>

WiFiClient espClient;
PubSubClient client(espClient);
String clientID;
String DeviceID;
char mqs[15];
boolean readyToSendNotifications=false;
boolean deviceRegistered=false;
//const char* mqtt_server;


void setup() {
  Wire.begin();
  Serial.begin(115200);
  setup_wifi();
  setup_mqttServer();
}

void loop() {
 
  if (!client.connected()) 
  {
    reconnect();
  }
    if(deviceRegistered && readyToSendNotifications)
  {
//    int sensorValue = analogRead(A0);
    String pars[] ={"temp","humidity"};
    String values[] ={String(SHT2x.GetTemperature()),String(SHT2x.GetHumidity())};
    sendNotification("SHT21",pars,values,2);
    delay(1000);
  }
  client.loop();
}


//--------------------------------------------****************--------------------------------------------//
//--------------------------------------------Setup Functions--------------------------------------------//

void setup_wifi() {

  delay(10);
  Serial.println();
  String SSIDIN = getInputString("Please Enter Wifi SSID : " );
  String PassIn = getInputString("Please Enter Wifi Password : ");

  const  char* ssid=SSIDIN.c_str();
  const  char* password=PassIn.c_str() ;
  Serial.print("Connecting to ");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
 void setup_mqttServer()
 {
  String MQTT_ServAdd = getInputString("Please Enter MQTT Server Address" );
  MQTT_ServAdd.toCharArray(mqs,MQTT_ServAdd.length()+1);
  Serial.println(MQTT_ServAdd.length());
  Serial.println("mqtt server adderss : ");
  Serial.println(mqs);
  client.setServer(mqs,1883);
  client.setCallback(callback);
 }


//--------------------------------------------*********************--------------------------------------------//
//--------------------------------------------Device Hive Functions--------------------------------------------//
void sendAuthenticateRequest()
{
  const int bufferSize = 401;
  StaticJsonBuffer<bufferSize> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  String token = getInputString("Please Enter Access Token :");
  JSONencoder["action"] = "authenticate";
  JSONencoder["token"] = token;
  char JSONmessageBuffer[bufferSize];

  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.print("Published message is: ");
  Serial.println(JSONmessageBuffer);
  client.publish("dh/request", JSONmessageBuffer,bufferSize);
  
}

void sendRegisterRequst()
{
  const int bufferSize = 401;
  StaticJsonBuffer<bufferSize> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();                                //  {
  JSONencoder["action"] = "device/save";                                              //            "action": "device/save",
  JSONencoder["deviceId"] = DeviceID;                                                 //            "deviceId": "mqtt-demo-device",
  JsonObject& device = JSONencoder.createNestedObject("device");                      //            "device": {
  device["name"] = DeviceID;                                                          //                       "name": "mqtt-demo-device"
  char JSONmessageBuffer[bufferSize];                                                 //                      }
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));            //  }
  Serial.print("Published message is: ");
  Serial.println(JSONmessageBuffer);
  client.publish("dh/request", JSONmessageBuffer);
}





void subscribeForInsertion()
{
  if (client.connect(clientID.c_str())) 
    {
      
      String subscribeTopic = "dh/response/notification/insert@"+clientID;
      client.subscribe(subscribeTopic.c_str());
      Serial.println("Subscribe For insertion :"+subscribeTopic);
      delay(100);
        
    }
    else
    {
      Serial.println("Device is not connected to Mqtt Server");
      Serial.println("Faild to subscribe for insertion data");
      
    }
}

void sendNotification(String notificationName,String parametersName[],String parametersValue[],int parametersNumber)
{
  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();                                //  {
  JSONencoder["action"] = "notification/insert";                                      //            "action": "notification/insert",
  JSONencoder["deviceId"] = DeviceID;                                                 //            "deviceId": "mqtt-demo-device",
  JsonObject& notification = JSONencoder.createNestedObject("notification");          //            "notification": {
  notification["notification"] = notificationName;                                    //                            "notification": "sht21"
  JsonObject& parameters = notification.createNestedObject("parameters");             //                             "parameters":
                                                                                      //                                          {
                                                                                      //                                          "parameter1" : "parametervalue1" ,
                                                                                      //                                           ...
  for(int i=0;i<parametersNumber;i++)                                                 //                                           }
  {
    parameters[parametersName[i]] = parametersValue[i];                                                                                                                                                             
  }
  
  char JSONmessageBuffer[300];                                                        //                            }
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));            // }
  Serial.print("Publish message: ");
  Serial.println(JSONmessageBuffer);
  client.publish("dh/request", JSONmessageBuffer);
}
//--------------------------------------------****************--------------------------------------------//
//--------------------------------------------Helper Functions--------------------------------------------//

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char jsonmsg[length];
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]);
    jsonmsg[i]=(char)payload[i];
  }
   Serial.println();
   StaticJsonBuffer<300> jsonBuffer;
   JsonObject& root = jsonBuffer.parseObject(jsonmsg);
   String stat = root["status"];
   String action = root["action"];
   Serial.println(stat);
   if(action.compareTo("authenticate")==0 )
   {
    if(stat.compareTo("success")==0)
    {
      registerDevice();
      delay(100);
      subscribeForInsertion();
      readyToSendNotifications = true;
      Serial.println("ready to send data to devicehive mqtt borker ");
    }
      else
    {
     sendAuthenticateRequest();
    } 
   }
   else if(action.compareTo("device/save")==0)
   {
    if(stat.compareTo("success")==0)
    {
      deviceRegistered=true;
      Serial.println("Device Register Successfully");
      readyToSendNotifications = true;
      Serial.println("ready to send data to devicehive mqtt borker ");
      subscribeForInsertion();
    }
    else
    {
      Serial.println("Device Registration Faild ");
      Serial.println("Retrying in 5 second");
      delay(5000);
      sendRegisterRequst();
    }
    
   }
   
   
}

void connectToMqttDHBroker()
{
  Serial.println("Connecting to Mqtt Broker ...");
  clientID = getInputString("Please Enter Client ID ");
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (client.connect(clientID.c_str())) 
    {
      Serial.println("MQTT Connection Stablished");
      delay(250);
      String subscribeTopic = "dh/response/authenticate@"+clientID;
      client.subscribe("dh/sbufortest");
      client.subscribe(subscribeTopic.c_str());
      Serial.println("Subscribe for :");
      Serial.println(subscribeTopic.c_str());
      delay(250);
      sendAuthenticateRequest();
    }
    else
    {
      
      Serial.println("Can not connect to MQQT Borker");
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
    
  }
}

void registerDevice()
{
  DeviceID = getInputString("Please Enter Device ID :");
  if (client.connect(clientID.c_str())) 
    {
//      do
//      {
        String subscribeTopic = "dh/response/device/save@"+clientID;
        client.subscribe(subscribeTopic.c_str());
        delay(250);
        sendRegisterRequst();
        
//      }
//      while(!deviceRegistered);
//      
    }
    else
    {
      Serial.println("Device is not connected to Mqtt Server");
      
    }
    
}

void reconnect() 
{
  connectToMqttDHBroker();
}


String getInputString(String promtMsg)
{
  Serial.println(promtMsg);
  String input;
  while(!Serial.available())
  {
    if(Serial.available())
    {
     break;
    }
  }
    
    input = Serial.readString();// read the incoming data as string
    input.trim();// Delete Enter Character in the end of Serial input String
    //Serial.println(input);
  return input;
}
//--------------------------------------------****************--------------------------------------------//
