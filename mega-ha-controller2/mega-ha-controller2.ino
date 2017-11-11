// Put the libraries in
#include <avr/wdt.h>

// Controller Name
const String controllerName = "HA-Controller1";

// button PINS
#define NUM_BUTTONS 8
const int button1 = 23;// was 23 - pushButton on 49
const int button2 = 25;
const int button3 = 27;
const int button4 = 29;
const int button5 = 31;
const int button6 = 33;\
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
#define RELAY_ON 0
#define RELAY_OFF 1
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

//// FUNCTIONS ////
// NONE



void setup()
{

  // start the serial library:
  Serial.begin(19200);
  Serial.println("Programme started on " + controllerName + "...");

  // Start watchdog
  wdt_enable(WDTO_1S);
  Serial.println("Watchdog started at 1 seconds");


  //// Set Pins START ////
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
        Serial.println("Button " + String(i) + " pressed for " + String(buttonCounters[i]) + " time.");
        //Now toggle the relay state for the corresponding button
        relayStates[i] = !relayStates[i];
        //Set Relay based on new state
        Serial.println("Setting relay " + String(i) + " to " + String(relayStates[i]));
        digitalWrite(relays[i],relayStates[i]);
      } else {
        Serial.println("Button " + String(i) + " released.");
      }
    }
    // save the current state as the last state,
    //for next time through the loop
    buttonLastStates[i] = buttonStates[i];
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
  delay(50);
  
  
}



