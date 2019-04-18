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
#define CHILD_PIN  4            // input from KL25Z (child status)
#define CSTATUS_PIN 5           // output to KL25z (comms)

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(CE_PIN, CSN_PIN);
/**********************************************************/

byte addresses[2][6] = { "Node1", "Node2" };
bool childInCar = true;
bool comm;
bool x = false;
bool movement1 = false, movement2 = false;
unsigned long currMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 5000;  // check for 5 second gap

void setup()
{
  pinMode(CSTATUS_PIN, OUTPUT);
  pinMode(CHILD_PIN, INPUT);
  pinMode(PIR1_PIN, INPUT);
  pinMode(PIR2_PIN, INPUT);
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
  childInCar = digitalRead(CHILD_PIN);
  /* The following code is to test the PIR sensors */
  if (movement2) Serial.println(F("PIR is true."));
  else Serial.println(F("PIR is false."));
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
    Serial.println(F("Failed"));
    comm = false;                                     // can't communicate
    if (!x)                                           // if first time can't communicate
    {
      prevMillis = millis();                          // get time
      x = true;                                       // no longer first time
    }
  }
  else
  {
    comm = true;                                      // if able to communicate
    x = false;                                        // set x to false so prevMillis will be gotten next time due to first failure
    digitalWrite(CSTATUS_PIN, HIGH);                  // set CSTATUS_PIN high
  }
  if(!comm)
  {
    currMillis = millis();
    if (currMillis - prevMillis >= txIntervalMillis)  // if not able to send after 5 seconds then 
    {
      digitalWrite(CSTATUS_PIN, LOW);                 // set CSTATUS_PIN low
    }
  } 
  // Try again 1s later
  delay(1000);
} // Loop
