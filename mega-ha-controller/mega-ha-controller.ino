// Include Watchdog Controller
#include <avr/wdt.h>

// Include Network Controllers
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>

// Include JSON library
#include <ArduinoJson.h>

// Include MQTT Controller
#include <PubSubClient.h>

// Controller Name
const char* controllerName = "HA-Controller1";

// Network Info
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };
//byte ip[] = { 192, 168, 1, 232 }; // use DHCP
byte server[] = { 64, 233, 187, 99 }; // Google

// MQTT Info
//const char* mqtt_server = "openHABianPine64.local";
const char* mqtt_server = "Pauls-MacBook-Pro.local";
const int mqtt_port = 1883;
//const char* mqtt_channel = "home/bus/";
const char* mqtt_channel_hb = "/home/bus/controller/heartbeat";
const char* mqtt_channel_pub = "/home/bus/controller/";
const char* mqtt_channel_sub = "/home/bus/controller/";
char message_buff[100]; //callback message buffer for 100 chars
StaticJsonBuffer<200> jsonBuffer;

// Create Ethernet Client
EthernetClient netclient;

// Create MQTT Client
PubSubClient mqttclient(netclient);


// button PINS
#define NUM_BUTTONS 8
const int button1 = 23;// was 23 - pushButton on 49
const int button2 = 25;
const int button3 = 27;
const int button4 = 29;
const int button5 = 31;
const int button6 = 33;
const int button7 = 35;
const int button8 = 37;
int buttons[] = {button1,button2,button3,button4,button5,button6,button7,button8};
//int buttonStates[] = {};
const byte pushButton = 49;// was 49 - button on 23
const int pushButtonRelayMapping = 1;

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


void setup()
{

  // start the serial library:
  Serial.begin(9600);
  Serial.println("Programme started on " + String(controllerName) + "...");


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

  
//  //// NETWORK setup START ////
//  // start the Ethernet connection:
//  Serial.println("Getting IP Address...");
//  if (Ethernet.begin(mac) == 0) {
//    Serial.println("Failed to configure Ethernet using DHCP");
//    // no point in carrying on, so do nothing forevermore:
//    for(;;)
//      ;
//  }
//  Serial.print("Got IP address from DHCP server: ");
//  // print your local IP address:
//  Serial.println(Ethernet.localIP());
//    
//  delay(1000); // wait a second and test
//
//  Serial.print("testing connection to internet...");
//
//  if (netclient.connect(server, 80)) {
//    Serial.println("connected");
//    netclient.println("GET /search?q=arduino HTTP/1.0");
//    netclient.println();
//  } else {
//    Serial.println("connection failed");
//  }
//  //// NETWORK setup END ////
//
//  //// MQTT setup START ////
//  //now connect to the mqtt server
//  Serial.print("Connecting to MQTT server...");
//  Serial.println(mqtt_server);
//  mqttclient.setServer(mqtt_server, mqtt_port);
//  mqttclient.connect(controllerName);
//  Serial.print("mqtt connected status...");
//  Serial.println(mqttclient.connected());
//  Serial.print("mqtt state...");
//  Serial.println(mqttclient.state());
//  reconnect();
////  Serial.println("Going to put test on MQTT:" + String(mqtt_channel_hb));
//  Serial.print("sending mqtt msg...");
//  Serial.println(mqttclient.publish(mqtt_channel_hb,"I'm alive"));
//  mqttclient.setCallback(callback);
//  //// MQTT setup END ////


  // Start watchdog
  wdt_enable(WDTO_2S);
  Serial.println("Watchdog started at 2 seconds...");

}

void loop()
{

  //safely in the loop
  wdt_reset();

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
}


//// FUNCTIONS ////

//// reconnect to the MQTT server
void reconnect() {
  Serial.println("Checking MQTT connection...");
  // Loop until we're reconnected
  while (!mqttclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttclient.connect(controllerName)) {
      Serial.println("connected... Subscribing to " + String(mqtt_channel_sub));
      mqttclient.publish(mqtt_channel_hb,"I'm alive"); 
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

//// MQTT message processor
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  Serial.println("Length: " + String(length,DEC));

  for (unsigned int i = 0; i < length; i++) {
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar);
    message_buff[i] = payload[i];
  }
  message_buff[length+1] = '\0';
  String msgString = String(message_buff);
  
  Serial.println("Payload: " + msgString);

  // {"relay":1,"state":1} - relays are 0-7 / state is 0 or 1
  JsonObject& root = jsonBuffer.parseObject(msgString);
  int relay = root["relay"];
  int state = root["state"];

  relayStates[relay] = state;
  //this should get picked up in the next loop, as the relay state register controls the relay
  
}
 
//// END FUNCTIONS ////


