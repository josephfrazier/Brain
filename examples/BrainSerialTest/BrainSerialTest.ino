// Arduino Brain Library - Brain Serial Test

// Description: Grabs brain data from the serial RX pin and sends CSV out over the TX pin (Half duplex.)
// More info: https://github.com/kitschpatrol/Arduino-Brain-Library
// Author: Eric Mika, 2010 revised in 2014

#include <Brain.h>

// Set up the brain parser, pass it the hardware serial object you want to listen on.
Brain brain(Serial);

const int led  =  8;     //use digital I/O pin 8

void setup() {
    // Start the hardware serial.
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(led,OUTPUT);   //set pin 8 to be an output output
}

void loop() {
    // Expect packets about once per second.
    // The .readCSV() function returns a string (well, char*) listing the most recent brain data, in the following format:
    // "signal strength, attention, meditation, delta, theta, low alpha, high alpha, low beta, high beta, low gamma, high gamma"
    if (!brain.update()) {
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        digitalWrite(led,LOW);    //set pin 8 LOW, turning off LED

        return;
    }

    Serial.println(brain.readErrors());

    char* str = brain.readCSV();
    Serial.println(str);

    // http://www.cplusplus.com/reference/cstring/strtok/
    char * pch;
    printf ("Splitting string \"%s\" into tokens:\n",str);
    int i = 0;
    pch = strtok(str," ,.-");
    while (pch != NULL)
    {
      if (i == 2) {
        int val = atoi(pch);
        if (0 < val && val < 40) {
          digitalWrite(LED_BUILTIN, HIGH);    // turn the LED on
          digitalWrite(led,HIGH);   //set pin 8 HIGH, turning on LED
        } else {
          digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
          digitalWrite(led,LOW);    //set pin 8 LOW, turning off LED      
        }
      }
      
      Serial.println(pch);
      pch = strtok(NULL, " ,.-");
      i = i + 1;
    }
 
    delay(200);                       // wait for a bit
}