/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte address[5] = {0x11, 0x11, 0x11, 0x11, 0x11};
struct payload_t{
  bool Child;
  unsigned long ms;
  unsigned long counter;
};

//bool childInSeat = 0; //1 means the child is in the seat

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(2);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setCRCLength(RF24_CRC_16);
}

//need to include code once out of range check for transmitter every 1min
//can use void   whatHappened (bool &tx_ok, bool &tx_fail, bool &rx_ready)
//                              successful, too many retries, msg to recieve
void loop() {
  Serial.print("test");
  payload_t test = {
    0,
    100,
    0
  };
  radio.printDetails(); //for debug purpose remove on final version
  delay(5000);
  radio.startListening();
  if (radio.available())
  {
    while (radio.available())
    {
      delay(5);
      radio.read(&test, sizeof(test));
      Serial.print(test.Child);
    }
  }
  
  //if (radio.available()) 
  //{
   
    
    //char text[32] = "";
    //radio.read(&text, sizeof(text));
    //Serial.println(text);
  //}
}
