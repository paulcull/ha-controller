

//#include <Dhcp.h>
//#include <Dns.h>
//#include <Ethernet.h>
//#include <EthernetClient.h>
//#include <EthernetServer.h>
//#include <EthernetUdp.h>

//#include <EEPROM.h>
//#include <Wire.h>
//#include <Adafruit_INA219.h>

//#include <DallasTemperature.h>
//#include <OneWire.h>

//#include <PubSubClient.h>
//#include <aREST.h>
//#include <avr/wdt.h>

//const char* local_mqtt_server = "study-pc.local";
//
//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
////byte ip[] = { 192, 168, 1, 232 }; // use DHCP
//byte server[] = { 64, 233, 187, 99 }; // Google
//
//EthernetClient netclient;
//PubSubClient mqttclient(netclient);

// Create aREST instance
//aREST rest = aREST();

// Variables to be exposed to the API
//int temperature;
//int relay;


// Controller Name
const String controllerName = "Controller1";

// MQTT Channel variables
const String mqttChannel = "home/bus/#";

// button PINS
#define NUM_BUTTONS 8
const byte button1 = 49;// was 23 - pushButton on 49
const byte button2 = 25;
const byte button3 = 27;
const byte button4 = 29;
const byte button5 = 31;
const byte button6 = 33;
const byte button7 = 35;
const byte button8 = 37;
int buttons[] = {button1,button2,button3,button4,button5,button6,button7,button8};
int buttonStates[] = {};
const byte pushButton = 23;// was 49 - button on 23


// RELAY PIN NUMBERS
#define NUM_RELAYS 8
const byte relay1 = 22;
const byte relay2 = 24;
const byte relay3 = 26;
const byte relay4 = 28;
const byte relay5 = 30;
const byte relay6 = 32;
const byte relay7 = 34;
const byte relay8 = 36;
int relays[] = {relay1,relay2,relay3,relay4,relay5,relay6,relay7,relay8};
int relayStates[] = {0,0,0,0,0,0,0,0};

// INITIAL relay state
//int relay1State = 0;
//int relay2State = 0;
//int relay3State = 0;
//int relay4State = 0;
//int relay5State = 0;
//int relay6State = 0;
//int relay7State = 0;
//int relay8State = 0;
//int relayStates[] = {relay1State,relay2State,relay3State,relay4State,relay5State,relay6State,relay7State,relay8State};

// INITIAL temp levels
int temps[] = {0,0,0,0,0,0,0,0};
//int tempCount = 4;


//// Start OneWire setup ////
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 13
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
//OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
//DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
//DeviceAddress therm1, therm2, therm3, therm4;
//DeviceAddress tempSensors[] = {therm1,therm2,therm3,therm4};
//// End OneWire setup ////

//const byte ledPin = 0; // Pin with LED on Adafruit Huzzah

//// FUNCTIONS ////
//// dallas onewire function to print a device address
//void printOneWireAddress(DeviceAddress deviceAddress)
//{
//  for (uint8_t i = 0; i < 8; i++)
//  {
//    // zero pad the address if necessary
//    if (deviceAddress[i] < 16) Serial.print("0");
//    Serial.print(deviceAddress[i], HEX);
//  }
//}
//
//// function to print out the temperatures
//void printTemperature(DeviceAddress deviceAddress)
//{
//  float tempC = sensors.getTempC(deviceAddress);
//  Serial.print("Temp C: ");
//  Serial.print(tempC);
//  Serial.print(" Temp F: ");
//  Serial.print(DallasTemperature::toFahrenheit(tempC));
//}
//
//
//// function to print a device"s resolution
//void printResolution(DeviceAddress deviceAddress)
//{
//  Serial.print("Resolution: ");
//  Serial.print(sensors.getResolution(deviceAddress));
//  Serial.println();    
//}
//
//// main function to print information about a device
//void printOneWireData(DeviceAddress deviceAddress)
//{
//  Serial.print("OneWire Device Address: ");
//  printOneWireAddress(deviceAddress);
//  Serial.print(" ");
//  printTemperature(deviceAddress);
//  Serial.println();
//}

//// relayController
// Custom function accessible by the API
//void relayToggle(int relay, int buttonState) {
//
//  if (buttonState == LOW) { 
//    //...ones, turn on!
//    if ( relayStates[relay] == 0){
//      digitalWrite(relays[relay], HIGH);
//      relayStates[relay]=1; //change flag variable
//    }
//    //...twice, turn off!
//    else if ( relayStates[relay] == 1){
//      digitalWrite(relays[relay], LOW);
//      relayStates[relay]=0; //change flag variable again 
//    }    
//  }
//
//}

//// MQTT message processor
//void callback(char* topic, byte* payload, unsigned int length) {
//  Serial.print("Message arrived [");
//  Serial.print(topic);
//  Serial.print("] ");
//  for (unsigned int i = 0; i < length; i++) {
//    char receivedChar = (char)payload[i];
//    Serial.print(receivedChar);
////    if (receivedChar = 0)
////      digitalWrite(relay1, LOW);
////    if (receivedChar = 1)
////      digitalWrite(relay1, HIGH);
//      //TODO::: Update to use relay control - specify channel to use for this device
//  }
//  Serial.println();
//}

//// reconnect to the MQTT server
//void reconnect() {
//  // Loop until we"re reconnected
//  while (!mqttclient.connected()) {
//    Serial.print("Attempting MQTT connection...");
//    // Attempt to connect
//    if (mqttclient.connect("Mega 2560 Client")) {
//      Serial.println("connected");
//      // ... and subscribe to topic
//      //mqttclient.subscribe(mqttChannel);
//      mqttclient.subscribe("ha/bus/#");
//    } else {
//      Serial.print("failed, rc=");
//      Serial.print(mqttclient.state());
//      Serial.println(" try again in 5 seconds");
//      // Wait 5 seconds before retrying
//      delay(5000);
//    }
//  }
//}

void setup()
{

  // start the serial library:
  Serial.begin(9600);
  Serial.println("Programme started...");

  
  //// NETWORK setup START ////
  // start the Ethernet connection:
//  if (Ethernet.begin(mac) == 0) {
//    Serial.println("Failed to configure Ethernet using DHCP");
//    // no point in carrying on, so do nothing forevermore:
//    for(;;)
//      ;
//  }
//   Serial.println("Got IP address from DHCP server");
//  // print your local IP address:
//  Serial.println(Ethernet.localIP());
//  // Start watchdog
//  wdt_enable(WDTO_4S);
//    
//  delay(1000); // wait a second and test
//
//  Serial.println("connecting to internet...");
//
//  if (netclient.connect(server, 80)) {
//    Serial.println("connected");
//    netclient.println("GET /search?q=arduino HTTP/1.0");
//    netclient.println();
//  } else {
//    Serial.println("connection failed");
//  }
  //// NETWORK setup END ////



  //// MQTT setup START ////

  //now connect to the mqtt server
//  mqttclient.setServer(local_mqtt_server, 1883);
//  mqttclient.setCallback(callback);
//
  //// MQTT setup END ////

  //// Set Pins START ////

  for (int i = 0; i < NUM_RELAYS; i++) {
    String msg = "setting button " + String(i);
    String msg2 = msg + " for pin " + String(buttons[i]);
    Serial.println(msg2);
    pinMode(buttons[i], INPUT); // INPUT for Switch - INPUT_PULLUP for buttons
    String msg3 = "setting relay " + String(i);
    String msg4 = msg3 + " for pin " + String(relays[i]);
    Serial.println(msg4);
    pinMode(relays[i], OUTPUT);
  }

  pinMode(pushButton, INPUT);
  //// Set Pins END ////


  //// Dallas Temp sensor setup START ////
  //// https://github.com/milesburton/Arduino-Temperature-Control-Library/blob/master/examples/Multiple/Multiple.pde
  // Start up the library
//  sensors.begin();  
//  // locate devices on the bus
//  Serial.print("Locating devices...");
//  Serial.print("Found ");
//  Serial.print(sensors.getDeviceCount(), DEC);
//  Serial.println(" devices.");
//  // report parasite power requirements
//  Serial.print("Parasite power is: "); 
//  if (sensors.isParasitePowerMode()) Serial.println("ON");
//  else Serial.println("OFF");
//
////// TODO figure out generic temp handler
////  for (int i = 0; i < sensors.getDeviceCount(); i++) {
////     if (!sensors.getAddress(DeviceAddress[i], 0)) Serial.println("Unable to find address for Device ",i); 
////    // show the addresses we found on the bus
////    Serial.print("Address for Device " + i +  " : ");
////    printOneWireAddress(DeviceAddress[i]);
////    Serial.println();
////    sensors.setResolution(DeviceAddress[i], TEMPERATURE_PRECISION);
////    Serial.print("Resolution for Device " + i + " : ");
////    Serial.print(sensors.getResolution(DeviceAddress[i]), DEC); 
////    Serial.println();
////  }
//  
//  if (!sensors.getAddress(therm1, 0)) Serial.println("Unable to find address for Device 0"); 
//  if (!sensors.getAddress(therm2, 1)) Serial.println("Unable to find address for Device 1"); 
//  if (!sensors.getAddress(therm3, 2)) Serial.println("Unable to find address for Device 2"); 
//  if (!sensors.getAddress(therm4, 3)) Serial.println("Unable to find address for Device 3"); 
// //  show the addresses we found on the bus
//  Serial.print("Device 0 Address: ");
//  printOneWireAddress(therm1);
//  Serial.println();
//  Serial.print("Device 1 Address: ");
//  printOneWireAddress(therm2);
//  Serial.println();
//  Serial.print("Device 2 Address: ");
//  printOneWireAddress(therm3);
//  Serial.println();
//  Serial.print("Device 3 Address: ");
//  printOneWireAddress(therm4);
//  Serial.println();
////   set the resolution to 9 bit per device
//  sensors.setResolution(therm1, TEMPERATURE_PRECISION);
//  sensors.setResolution(therm2, TEMPERATURE_PRECISION);
//  sensors.setResolution(therm3, TEMPERATURE_PRECISION);
//  sensors.setResolution(therm4, TEMPERATURE_PRECISION);
//  Serial.print("Device 0 Resolution: ");
//  Serial.print(sensors.getResolution(therm1), DEC); 
//  Serial.println();
//  Serial.print("Device 1 Resolution: ");
//  Serial.print(sensors.getResolution(therm2), DEC); 
//  Serial.println();
//  Serial.print("Device 2 Resolution: ");
//  Serial.print(sensors.getResolution(therm3), DEC); 
//  Serial.println();
//  Serial.print("Device 3 Resolution: ");
//  Serial.print(sensors.getResolution(therm4), DEC); 
//  Serial.println();
  //// Dallas Temp sensor setup END ////

  //// REST Setup START ////
  
//  rest.variable("temperature1",&temp1);
//  rest.variable("temperature2",&temp2);
//  rest.variable("temperature3",&temp3);
//  rest.variable("temperature4",&temp4);
//  rest.variable("relay1",&relay1State);  
//  rest.variable("relay2",&relay2State);  
//  rest.variable("relay3",&relay3State);  
//  rest.variable("relay4",&relay4State);  
//  rest.variable("relay5",&relay5State);  
//  rest.variable("relay6",&relay6State);  
//  rest.variable("relay7",&relay7State);  
//  rest.variable("relay8",&relay8State);  
//  rest.function("button1",relayControl("1"));
//  rest.function("button2",relayControl("2"));
//  rest.function("button3",relayControl("3"));
//  rest.function("button4",relayControl("4"));
//  rest.function("button5",relayControl("5"));
//  rest.function("button6",relayControl("6"));
//  rest.function("button7",relayControl("7"));
//  rest.function("button8",relayControl("8"));
//
//  rest.set_id("001");
//  rest.set_name("test_brick_controller");

  //// REST Setup END ////


}

void loop()
{

//  if (!mqttclient.connected()) {
//    reconnect();
//  }
//  mqttclient.loop();

  //// one wire loop
//  Serial.print("Requesting temperatures...");
//  sensors.requestTemperatures();
//  Serial.println("DONE");  
  // print the device information

  
  
//  printOneWireData(therm1);
//  printOneWireData(therm2);
//  printOneWireData(therm3);
//  printOneWireData(therm4);


     for (int i=0 ; i< NUM_RELAYS ; ++i) {
         while(digitalRead(buttons[i]))
              digitalWrite(relays[i], HIGH);
         digitalWrite(relays[i], LOW);
      delay(200);
     }

//  for (int i = 0; i < relayCount; i++) 
//  {
//    buttonStates[i] = digitalRead(buttons[i]);
//    if (buttonStates[i] == HIGH) {
//      digitalWrite(relays[i], HIGH);
//    } else {
//      digitalWrite(relays[i], LOW);
//    }
//    delay(200);
//  }
//
//  for (int i = 0; i < NUM_RELAYS; i++) {
//    Serial.println(buttonStates[i]);
//    Serial.println(relayStates[i]);
//  }

  //digitalWrite(relay1, HIGH);

//  int pusbuttonval = digitalRead(pushButton);   // read the input pin
////  Serial.println("Button Pressed ");
//  digitalWrite(relay1, pusbuttonval);
  
    
  //watch for dbl triggering
//  delay(200); //Small delay



  
}



