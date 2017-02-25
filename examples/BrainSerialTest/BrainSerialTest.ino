// Arduino Brain Library - Brain Serial Test

// Description: Grabs brain data from the serial RX pin and sends CSV out over the TX pin (Half duplex.)
// More info: https://github.com/kitschpatrol/Arduino-Brain-Library
// Author: Eric Mika, 2010 revised in 2014

#include <Brain.h>

// Set up the brain parser, pass it the hardware serial object you want to listen on.
Brain brain(Serial);

void setup() {
    // Start the hardware serial.
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
    // Expect packets about once per second.
    // The .readCSV() function returns a string (well, char*) listing the most recent brain data, in the following format:
    // "signal strength, attention, meditation, delta, theta, low alpha, high alpha, low beta, high beta, low gamma, high gamma"
    if (!brain.update()) {
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        return;
    }

    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED on
    Serial.println(brain.readErrors());

    char* str = brain.readCSV();
    Serial.println(str);

    // http://www.cplusplus.com/reference/cstring/strtok/
    char * pch;
    printf ("Splitting string \"%s\" into tokens:\n",str);
    pch = strtok (str," ,.-");
    while (pch != NULL)
    {
      printf ("%s\n",pch);
      pch = strtok (NULL, " ,.-");
    }

    delay(100);                       // wait for a bit
}
