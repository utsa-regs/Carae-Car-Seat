/*  
  * Group: REGS
  * Programmer: Joshua Edwards 
  * 4/5/2019
  * This is the code for our transmitter. It will transmitted to the key fob
  * and send the status of the child in the car seat. If the child 
  * is in the seat as of the last update to the key fob and it is unable to 
  * transmit due to loss of connection (the parent walks away) then 
  * the buzzer on the key fob will go off.
*/

#include <SPI.h>                // Needed to talk to xmitter
#include <RF24.h>
#include <nRF24L01.h>
#include <RF24_config.h>

#define CE_PIN   14
#define CSN_PIN  10
#define SIG_PIN  4

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(CE_PIN, CSN_PIN);
/**********************************************************/

byte addresses[2][6] = { "Node1", "Node2" };
bool childInCar = true;

void setup()
{
  pinMode(SIG_PIN, INPUT);
  Serial.begin(9600);
  Serial.println(F("Transmitter."));

  radio.begin();
  radio.setChannel(1);
  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.setRetries(2,15);                  // Optionally, increase the delay between retries & # of retries
  radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);

  // Open a writing and reading pipe on each radio, with opposite addresses

  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
}

void loop()
{
  childInCar = digitalRead(SIG_PIN);
  if (childInCar)
  {
    Serial.println(F("Baby is in the seat."));
  }
  else
  {
    Serial.println(F("Baby is not in the seat."));
  }
  Serial.println(F("Now sending"));
  if (!radio.write(&childInCar, sizeof(bool)))
  {
    Serial.println(F("failed"));
  }
  // Try again 1s later
  delay(1000);
} // Loop
