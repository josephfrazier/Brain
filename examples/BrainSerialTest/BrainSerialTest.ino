// Arduino Brain Library - Brain Serial Test

// Description: Grabs brain data from the serial RX pin and sends CSV out over the TX pin (Half duplex.)
// More info: https://github.com/kitschpatrol/Arduino-Brain-Library
// Author: Eric Mika, 2010 revised in 2014

#include <Brain.h>

// Set up the brain parser, pass it the hardware serial object you want to listen on.
Brain brain(Serial);

const int firePin  =  8;     //use digital I/O pin 8
const int chargePin  =  4;     //use digital I/O pin 4

int chargeCount = 0;
const int chargeMax = 10;

void setup() {
    // Start the hardware serial.
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(firePin,OUTPUT);   //set pin 8 to be an output output
    pinMode(chargePin,OUTPUT);   //set pin 4 to be an output output
}

void charge() {
  if (chargeCount < chargeMax) {
    digitalWrite(chargePin,HIGH);    //set pin 8 LOW, turning off LED
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED on
    chargeCount = chargeCount + 1;
  }
}

void fire() {
  if (chargeCount == chargeMax) {
    digitalWrite(firePin,HIGH);   //set pin 8 HIGH, turning on LED
    digitalWrite(chargePin,LOW);
    chargeCount = 0;
  }
}

void loop() {
    // Expect packets about once per second.
    // The .readCSV() function returns a string (well, char*) listing the most recent brain data, in the following format:
    // "signal strength, attention, meditation, delta, theta, low alpha, high alpha, low beta, high beta, low gamma, high gamma"
    if (!brain.update()) {
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        digitalWrite(firePin,LOW);    //set pin 8 LOW, turning off LED

        return;
    }

    Serial.println(brain.readErrors());

    Serial.println("chargeCount:");
    Serial.println(chargeCount);
    Serial.println();

    charge();

    char* str = brain.readCSV();

    // http://www.cplusplus.com/reference/cstring/strtok/
    char * pch;
    int i = 0;
    pch = strtok(str," ,.-");
    while (pch != NULL)
    {
      // look for the "meditation" value, per http://www.frontiernerds.com/brain-hack
      if (i == 2) {
        Serial.println("meditation:");
        Serial.println(pch);
        Serial.println();
        int val = atoi(pch);
        if (0 < val && val < 40) {
          fire();
        } else {
          digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
          digitalWrite(firePin,LOW);    //set pin 8 LOW, turning off LED
        }
      }

      pch = strtok(NULL, " ,.-");
      i = i + 1;
    }

    delay(200);                       // wait for a bit
}
