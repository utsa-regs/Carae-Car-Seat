/*  
  * Group: REGS
  * Programmer: Joshua Edwards 
  * 4/5/2019
  * This is the code for our key fob. It will transmitted to the Car seat
  * and recieve the status of the child in the car seat. If the child 
  * is in the seat as of the last update to the key fob and it is unable to 
  * transmit due to loss of connection (the parent walks away) then 
  * the buzzer will go off.
*/

#include <SPI.h>                // Needed to talk to xmitter
#include <RF24.h>
#include <nRF24L01.h>
#include <RF24_config.h>

#define CE_PIN   7
#define CSN_PIN  8
#define BUZZ_PIN 5              //buzzer is on D5

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(CE_PIN, CSN_PIN);
/**********************************************************/

byte addresses[2][6] = {"Node1", "Node2"};
bool childInCar = false;
bool sleep = false;
unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 5000;  // check for 5 second gap
unsigned long sleeptime = 20000;        //sleep for 20 seconds

void setup()
{
  pinMode(BUZZ_PIN, OUTPUT);
  Serial.begin(9600);             // TODO: Remove this when I don't need to display to serial
  Serial.println(F("Receiver.")); // TODO: Remove this when I don't need to display to serial

  radio.begin();
  radio.setChannel(1);
  radio.setAutoAck(1);            // Ensure autoACK is enabled
  radio.setRetries(2, 15);        // Optionally, increase the delay between retries & # of retries
  radio.setCRCLength(RF24_CRC_8); // Use 8-bit CRC for performance

  radio.setPALevel(RF24_PA_LOW);  // Using low power to save power and have range short on purpose

  // Open a writing and reading pipe on each radio, with opposite addresses

  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

  // Start the radio listening for data
  radio.startListening();
}

void loop()
{
  prevMillis = millis();                            // Get current time and put it into prevMillis
  if (sleep)                                        // If the RF24 is asleep
  {
    currentMillis = millis();                       // Get time and put into currentMillis
    while(currentMillis - prevMillis < sleeptime)   // While time is less than 20 secs
    {
      currentMillis = millis();                     // Get current time
    }                                               // Come out of loop
    sleep = false;                                  // Device coming out of sleep
    radio.PowerUp();                                // Power up RF24
    prevMillis = millis();                          // Put current time into prevMillis
  }
  while (!radio.available())                        // If there is nothing in the buffer we are gonna wait
  {
    currentMillis = millis();                       // Get current time while in loop
    if (currentMillis - prevMillis >= txIntervalMillis) // If it has been longer than 5 seconds
    {
        if(childInCar)                              // If the child is in the seat
        {
          digitalWrite(BUZZ_PIN, HIGH);             // Set off the buzzer
        }
        else                                        // Otherwise
        {
          radio.powerDown();                        // It is safe to power down the RF24 for 20 seconds
          sleep = true;                             // We are in sleep mode
        }
    }
  } //end while loop
  if (radio.available())
  {                                                 // If there is data ready
    radio.read(&childInCar, sizeof(bool));          // Get the payload
    Serial.println(F("Received data."));            // TODO: Remove this when I don't need to display to serial
  }
  if (childInCar)
  {
    Serial.println(F("Baby is in the seat!!!!!!!!!!!"));  // TODO: Remove this when I don't need to display to serial
  }
  else
  {
    Serial.println(F("Baby is not in the seat."));  // TODO: Remove this when I don't need to display to serial
  }
}
