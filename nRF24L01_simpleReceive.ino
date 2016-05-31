/*
 * nRF24L01 Send Sample Program
 * 
 * VCC  3.3V
 * MISO 12
 * MOSI 11
 * SCK  13
 * CE   8
 * CSN  7
 */
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
//RF24 radio(9,10);
RF24 radio(8,7);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };


void setup(void)
{
  Serial.begin(9600);
  Serial.println("nTF24L01 Send Demo");
  printf_begin();
  
  //
  // Setup and configure rf radio
  //
  radio.begin();
  radio.openReadingPipe(0,pipes[1]);
  radio.startListening();//
  radio.printDetails();
}

void loop(void)
{
  static int value = 0;
  uint8_t len;//
  uint8_t data[32];

  // Take the time, and send it.  This will block until complete
  data[0]=value;
  value++;

  if(radio.available()){
    len = radio.getDynamicPayloadSize();
    radio.read(&data,len);
    Serial.println(data[0]);
  }else{
    Serial.println("radio not available...");
  }

}

