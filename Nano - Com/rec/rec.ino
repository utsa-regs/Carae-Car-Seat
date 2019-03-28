
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
  Serial.println(F("Reciever."));
  //Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);

  // Open a writing and reading pipe on each radio, with opposite addresses
  
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
  

  // Start the radio listening for data
  radio.startListening();
}

void loop()
{
  childInCar = false; //TODO: once the signal is reliably received this needs to be removed
  if(!childInCar) Serial.println("Child status is cleared."); //TODO: this will be removed eventually
  else Serial.println("Error: childInCar wasn't reset to false"); //TODO: this will be removed eventually

  while (!radio.available()){}
  radio.read(&childInCar, sizeof(bool)); // Get the payload
  /*
  if (radio.available())
  {
    // Variable for the received timestamp
    while (radio.available())
    {                                        // While there is data ready
      radio.read(&childInCar, sizeof(bool)); // Get the payload
      //!received = true;
      //!Serial.println(F("Received data."));
      //!radio.stopListening();                // First, stop listening so we can talk
    }
    //end while loop
    //!radio.write(&received, sizeof(bool)); // Send the final one back.
    //!radio.startListening();               // Now, resume listening so we catch the next packets.
    //!Serial.println(F("Sent response."));
  }
  */
  if (childInCar) Serial.println(F("Child is in Car Seat.")); //TODO: this will be removed eventually
  else Serial.println(F("Child NOT in Car Seat.")); //TODO: this will be removed eventually
}