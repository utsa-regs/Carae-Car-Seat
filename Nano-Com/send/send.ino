
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"

/****************** User Config ***************************/
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7, 8);
/**********************************************************/

byte addresses[][6] = {"1Node", "2Node"};

// Used to control whether this node is sending or receiving
bool childInCar = false;

void setup()
{
  Serial.begin(9600);
  Serial.println(F("Transmitter."));
  //Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);

  // Open a writing and reading pipe on each radio, with opposite addresses

  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

  // Start the radio listening for data
  // radio.startListening();
}

void loop()
{

  /****************** Ping Out Role ***************************/
  radio.stopListening();                                    // First, stop listening so we can talk.
  Serial.println(F("Now sending"));
  if (!radio.write(&childInCar, sizeof(childInCar)))
  {
    Serial.println(F("failed"));
  }

  radio.startListening(); // Now, listening for response

  unsigned long started_waiting_at = micros(); // Set up a timeout period, get the current microseconds
  boolean timeout = false;                     // Set up a variable to indicate if a response was received or not

  while (!radio.available())
  { // While nothing is received
    if (micros() - started_waiting_at > 100000)
    { // If waited longer than 200ms, indicate timeout and exit while loop
      timeout = true;
      break;
    }
  }

  if (timeout)
  { // Describe the results
    Serial.println(F("Failed, response timed out."));
  }
  else
  {
    bool received = false; // Grab the response, compare, and send to debugging spew
    radio.read(&received, sizeof(received));

    // Spew it
    if (received)
    {
      Serial.println(F("Message was recieved."));
    }
    else
    {
      Serial.println(F("Message was not recieved."));
    }
  }

  // Try again 1s later
  delay(1000);
} // Loop
