
/*

  This is the starting point - will add documentation here

*/




// Include Network Controllers
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
//#include <WiFiServer.h>
//#include <ESP8266WiFi.h>
//#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <avr/wdt.h>



/************ network Setup Information (CHANGE THESE FOR YOUR SETUP) ******************/
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };
//byte ip[] = { 192, 168, 1, 232 }; // use DHCP as preference
byte server[] = { 64, 233, 187, 99 }; // Google - for testing network setup

/************ WIFI  Information (CHANGE THESE FOR YOUR SETUP) **************************/
char* ssid = "YourSSID"; //type your WIFI information inside the quotes
char* wifi_password = "YourWIFIpassword";
bool network_wifi = false;

/**************************** FOR OTA **************************************************/
#define OTApassword "yourOTApassword" //the password you will need to enter to upload remotely via the ArduinoIDE
int OTAport = 8266;


/*********************** MQTT setup Details ********************************************/
//const char* mqtt_server = "openHABianPine64.local";
const char* mqtt_server = "Pauls-MacBook-Pro.local";
const int mqtt_port = 1883;

const char mqtt_channel_pub[] = "/home/bus/action/HA-Controller1/";
const char mqtt_heartbeat[] = "/home/bus/state/heartbeat/HA-Controller1";
const char mqtt_channel_sub[] = "/home/bus/state/HA-Controller1/#";

//const String mqtt_channel_pub = "/home/bus/action/";
//const String mqtt_channel_sub = "/home/bus/state";
//const String mqtt_seperator = "/";
//const String mqtt_heartbeat = "/heartbeat";

const char* on_cmd = "ON";
const char* off_cmd = "OFF";
const char* toggle_cmd = "TOGGLE";


/****************************************FOR JSON***************************************/
const int BUFFER_SIZE = JSON_OBJECT_SIZE(10);
//#define MQTT_MAX_PACKET_SIZE 512 // already defined in the pubsubclient // only extend if needed


/**************************** define the clients ***************************************/
EthernetClient netclient;
WiFiClient wificlient;
//Client client;
//PubSubClient mqttclient(client);
PubSubClient mqttclient(netclient);
PubSubClient mqttwificlient(wificlient);

/**************************** Borad Details ********************************************/
const char* controllerName = "HA-Controller1";


/**************************** PIN Details **********************************************/
#define NUM_BUTTONS 8
const int button1 = 23;
const int button2 = 25;
const int button3 = 27;
const int button4 = 29;
const int button5 = 31;
const int button6 = 33;
const int button7 = 35;
const int button8 = 37;
int buttons[] = {button1,button2,button3,button4,button5,button6,button7,button8};
//int buttonStates[] = {};

const byte pushButton = 49;
const int pushButtonRelayMapping = 1;

// RELAY PIN NUMBERS
#define NUM_RELAYS 8
#define RELAY_ON 1
#define RELAY_OFF 0
const int relay1 = 22;
const int relay2 = 24;
const int relay3 = 26;
const int relay4 = 28;
const int relay5 = 30;
const int relay6 = 32;
const int relay7 = 34;
const int relay8 = 36;
int relays[] = {relay1,relay2,relay3,relay4,relay5,relay6,relay7,relay8};


/****************** State Register Details **********************************************/

const int buttonCounter1 = 0;
const int buttonCounter2 = 0;
const int buttonCounter3 = 0;
const int buttonCounter4 = 0;
const int buttonCounter5 = 0;
const int buttonCounter6 = 0;
const int buttonCounter7 = 0;
const int buttonCounter8 = 0;
int buttonCounters[] = {buttonCounter1,buttonCounter2,buttonCounter3,buttonCounter4,buttonCounter5,buttonCounter6,buttonCounter7,buttonCounter8};

// CURRENT relay state
const int buttonState1 = 0;
const int buttonState2 = 0;
const int buttonState3 = 0;
const int buttonState4 = 0;
const int buttonState5 = 0;
const int buttonState6 = 0;
const int buttonState7 = 0;
const int buttonState8 = 0;
int buttonStates[] = {buttonState1,buttonState2,buttonState3,buttonState4,buttonState5,buttonState6,buttonState7,buttonState8};
int pushbuttonState = 0;

// LAST relay state
const int buttonLastState1 = 0;
const int buttonLastState2 = 0;
const int buttonLastState3 = 0;
const int buttonLastState4 = 0;
const int buttonLastState5 = 0;
const int buttonLastState6 = 0;
const int buttonLastState7 = 0;
const int buttonLastState8 = 0;
int buttonLastStates[] = {buttonLastState1,buttonLastState2,buttonLastState3,buttonLastState4,buttonLastState5,buttonLastState6,buttonLastState7,buttonLastState8};
int pushbuttonLastState = 0;


// INITIAL relay state
const int relayState1 = 0;
const int relayState2 = 0;
const int relayState3 = 0;
const int relayState4 = 0;
const int relayState5 = 0;
const int relayState6 = 0;
const int relayState7 = 0;
const int relayState8 = 0;
int relayStates[] = {relayState1,relayState2,relayState3,relayState4,relayState5,relayState6,relayState7,relayState8};


/********************************** START SETUP*****************************************/
void setup()
{

  // start the serial library:
  Serial.begin(19200);
  Serial.println("Boot Started on " + String(controllerName) + "...");

  // Pin mapping setup
  Serial.println("Boot Start Board Config...");
  setup_pins();
  Serial.println("Boot Start Board Config...Done");

  // Network setup 
  // TODO: can I get a generic type to assign netclient or wificlient ?

  Serial.println("Boot Start Network...");
//  if (<<test for ethernet plugin>>) {
    Serial.println("Boot Start Network...Ethernet...");
    network_wifi = false;
    setup_ethernet();
    Serial.println("Boot Start Network...Ethernet...Done");
//  } else {
//    Serial.println("Boot Start Network...Wifi...");
//    network_wifi = true;
//    setup_wifi();
//    Serial.println("Boot Start Network...Wifi...Done");
//  }
  
  
  
  
  // MQTT setup
  Serial.println("Boot Start MQTT...");
  if (network_wifi) {
    setup_wifimqtt();
  } else {
    setup_mqtt();  
  }
  Serial.println("Boot Start MQTT...Done");

  //OTA SETUP
  Serial.println("Boot Start OTA...");
  setup_OTA();
  Serial.println("Boot Start OTA...Done");

  // Start watchdog
  Serial.println("Boot Start watchdog...");
  wdt_enable(WDTO_8S);
  Serial.println("Watchdog started at 8 seconds...");
  Serial.println("Boot Start watchdog...Done");

  // Finish
  Serial.println("Boot Completed on " + String(controllerName));
  Serial.println("Ready");


}

/********************************** START SETUP PINS*****************************************/
void setup_pins() {

  //// Set Pins START ////
  Serial.println("Setting up I/O Pins...");
  pinMode(pushButton,INPUT_PULLUP);  
  for (int i = 0; i < NUM_RELAYS; i++) {
    String msg = "setting button " + String(i);
    String msg2 = msg + " for pin " + String(buttons[i]);
    Serial.println(msg2);
    pinMode(buttons[i], INPUT_PULLUP); // INPUT for Switch - INPUT_PULLUP for buttons
    String msg3 = "setting relay " + String(i);
    String msg4 = msg3 + " for pin " + String(relays[i]);
    Serial.println(msg4);
    pinMode(relays[i], OUTPUT);
    digitalWrite(relays[i],RELAY_OFF);
  }
  



/****************************** START SETUP ETHERNET*****************************************/
void setup_ethernet() {

//   Serial.println("Getting IP Address...");
//   if (Ethernet.begin(mac) == 0) {
//     Serial.println("Failed to configure Ethernet using DHCP");
//     // no point in carrying on, so do nothing forevermore:
//     for(;;)
//       ;
//   }
//   Serial.println("");
//   Serial.println("Ethernet connected");
//   Serial.print("Got IP address from DHCP server ");
//   Serial.print("IP address: ");
//   Serial.println(Ethernet.localIP());

//   delay(1000); // wait a second and test

//   Serial.print("Testing connection to internet...");

//   if (netclient.connect(server, 80)) {
//     Serial.println("connected");
//     netclient.println("GET /search?q=arduino HTTP/1.0");
//     netclient.println();
//   } else {
//     Serial.println("connection failed");
//   }

//  client = netclient;
  
}
    
/********************************** START SETUP WIFI*****************************************/
void setup_wifi() {

//   delay(10);
//   // We start by connecting to a WiFi network
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(ssid);
  
// //  WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, wifi_password);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("");
//   Serial.println("WiFi connected");
//   Serial.println("Got IP address from DHCP server ");
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());

//   delay(1000); // wait a second and test

//   Serial.print("Testing connection to internet...");

//   if (wificlient.connect(server, 80)) {
//     Serial.println("connected");
//     wificlient.println("GET /search?q=arduino HTTP/1.0");
//     wificlient.println();
//   } else {
//     Serial.println("connection failed");
//   }

//  client = wificlient;

}

/********************************** START SETUP MQTT*****************************************/
void setup_mqtt() {

//   Serial.print("Connecting to MQTT server...");
//   Serial.println(mqtt_server);
//   mqttclient.setServer(mqtt_server, mqtt_port);
//   mqttclient.connect(controllerName);
//   Serial.print("mqtt connected status...");
//   Serial.println(mqttclient.connected());
//   Serial.print("mqtt state...");
//   Serial.println(mqttclient.state());
//   mqttclient.setCallback(callback);

  
}

/********************************** FUNCTION STRING *****************************************/
void StringToChar(String input) {

  // Length (with one extra character for the null terminator)
  int str_len = input.length() + 1; 
  String output = "";
  
  // Prepare the character array (the buffer) 
  char char_array[str_len];
  
  // Copy it over 
  //output = input.toCharArray(char_array, str_len);
  //return output;
  return input.toCharArray(char_array, str_len);
  
}

/********************************** START SETUP MQTT*****************************************/
void setup_wifimqtt() {

//   Serial.print("Connecting to MQTT server...");
//   Serial.println(mqtt_server);
//   mqttwificlient.setServer(mqtt_server, mqtt_port);
//   mqttwificlient.connect(controllerName);
//   Serial.print("mqtt connected status...");
//   Serial.println(mqttclient.connected());
//   Serial.print("mqtt state...");
//   Serial.println(mqttclient.state());
//   mqttwificlient.setCallback(callback);


}

/********************************** START CALLBACK*****************************************/
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  Serial.println(message);

  if (!processJson(message)) {
    return false;
  }

  //TODO sendState should be on the triggering cycle
  //sendState();
}

/********************************** START PROCESS JSON*****************************************/
bool processJson(char* message) {


/*
  SAMPLE PAYLOAD:
  {
    "relay": 1,
    "action": "ON"  // ON  / OFF  / TOGGLE
  }
*/

  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(message);

  int relay = -1;

  if (!root.success()) {
    Serial.println("ERR: parseObject() json message failed");
    return false;
  }


  if (!root.containsKey("relay")) {
    Serial.println("ERR: no relay provided in message");
    return false;
  } else {
    relay = root["relay"];
  }

  if (!root.containsKey("action") ) {
    Serial.println("ERR: no action directive");
    return false;
  }

  
  if (root.containsKey("action")) {
    if (strcmp(root["action"], on_cmd) == 0) {
      relayStates[relay] = RELAY_ON;
    }
    else if (strcmp(root["action"], off_cmd) == 0) {
      relayStates[relay] = RELAY_OFF;
    }
    else if (strcmp(root["action"], toggle_cmd) == 0) {
      relayStates[relay] = !relayStates[relay];
    }
  }

  return true;
}

/********************************** START SEND STATE*****************************************/
void sendState(int relay) {

  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["state"] = (relayStates[relay]) ? on_cmd : off_cmd;

  char buffer[root.measureLength() + 1];
  root.printTo(buffer, sizeof(buffer));

//  String _pubTopic = mqtt_channel_pub + controllerName;
//  _pubTopic = _pubTopic + mqtt_seperator;
//  _pubTopic = _pubTopic + String(relay);
  
//  char* _pubTopic = mqtt_channel_pub; 
//  _pubTopic = _pubTopic, mqtt_seperator; 
//  _pubTopic = _pubTopic, controllerName; 
//  _pubTopic = _pubTopic, mqtt_seperator; 
//  _pubTopic = _pubTopic, String(relay);

  if (network_wifi) {
    mqttwificlient.publish(mqtt_channel_pub, buffer, true);      
  } else {
    mqttclient.publish(mqtt_channel_pub, buffer, true);  
  }
}

/***************************** START SEND HEARTBEAT *****************************************/
void sendHeartbeat() {

  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["alive"] = on_cmd;

  char buffer[root.measureLength() + 1];
  root.printTo(buffer, sizeof(buffer));

//  String _pubTopic = mqtt_channel_pub + controllerName;
//  _pubTopic = _pubTopic + mqtt_heartbeat;

//  char* _pubTopic = mqtt_channel_pub; 
//  _pubTopic = _pubTopic, mqtt_seperator; 
//  _pubTopic = _pubTopic, controllerName;
//  _pubTopic = _pubTopic, mqtt_heartbeat;

  if (network_wifi) {
    mqttwificlient.publish(mqtt_heartbeat, buffer, true);      
  } else {
    mqttclient.publish(mqtt_heartbeat, buffer, true);  
  }

}

/********************************** START RECONNECT*****************************************/
void reconnect() {
  Serial.println("Checking MQTT connection...");
  // Loop until we're reconnected
  while (!mqttclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttclient.connect(controllerName)) {
      Serial.println("connected... Subscribing to " + String(mqtt_channel_sub));
      //mqttclient.publish(mqtt_channel_hb,"I'm alive"); 
      // ... and subscribe to topic
      mqttclient.subscribe(mqtt_channel_sub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttclient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


/********************************** START SETUP PINS*****************************************/
void setup_OTA() {

//  ArduinoOTA.setPort(OTAport);
//  // Hostname defaults to esp8266-[ChipID]
//  ArduinoOTA.setHostname(cpntrollerName);
//
//  // No authentication by default
//  ArduinoOTA.setPassword((const char *)OTApassword);
//
//  ArduinoOTA.onStart([]() {
//    Serial.println("Starting OTA Update");
//  });
//  ArduinoOTA.onEnd([]() {
//    Serial.println("\nEnd");
//  });
//  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
//    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
//  });
//  ArduinoOTA.onError([](ota_error_t error) {
//    Serial.printf("Error[%u]: ", error);
//    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
//    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
//    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
//    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
//    else if (error == OTA_END_ERROR) Serial.println("End Failed");
//  });
//  ArduinoOTA.begin();
  
}


/********************************** START MAIN LOOP*****************************************/
void loop()
{

  //safely in the loop
  wdt_reset();


//   if (!mqttclient.connected()) {
//     reconnect();
//   }

//   if (network_wifi) {
//     if (WiFi.status() != WL_CONNECTED) {
//       delay(1);
//       Serial.print("WIFI Disconnected. Attempting reconnection.");
//       setup_wifi();
//       return;
//     }
//   }

  for (int i = 0; i < NUM_RELAYS; i++) 
  {
    buttonStates[i] = digitalRead(buttons[i]);
    if (buttonStates[i] != buttonLastStates[i]) {
      if (buttonStates[i] == LOW) {
        buttonCounters[i]++;
        Serial.println("Button " + String(i) + " pressed for " + String(buttonCounters[i]) + " times.");
        //Now toggle the relay state for the corresponding button
        relayStates[i] = !relayStates[i];
        Serial.println("Setting relay " + String(i) + " to " + String(relayStates[i]));
      } else {
        Serial.println("Button " + String(i) + " released. Relay state " + String(relayStates[i]));
      }
    }
    // save the current state as the last state,
    //for next time through the loop
    buttonLastStates[i] = buttonStates[i];
        
    //Set Relay based on stored state
    //This allows the state to be set by something else like the mqtt callback controller
    //publish a state message
    if (relays[i] != relayStates[i]) {
      sendState(i);
    }
    digitalWrite(relays[i],relayStates[i]);
    
  }

  // handle the local pushbutton
  pushbuttonState = digitalRead(pushButton);
  if (pushbuttonState != pushbuttonLastState) {
    if (pushbuttonState == LOW) {
      relayStates[pushButtonRelayMapping] = !relayStates[pushButtonRelayMapping];
        Serial.println("PushButton - Setting relay " + String(pushButtonRelayMapping) + " to " + String(relayStates[pushButtonRelayMapping]));
        digitalWrite(relays[pushButtonRelayMapping],relayStates[pushButtonRelayMapping]);
    } else {
        Serial.println("Pushbutton released.");
    }
  }
  pushbuttonLastState = pushbuttonState;


  // Delay a little bit to avoid bouncing
  delay(30);

//   mqttclient.loop();

//  ArduinoOTA.handle();

  
}




