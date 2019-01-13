
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(2,3);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

// Used to control whether this node is sending or receiving
bool connec;
unsigned long started_waiting_at;
int input;
int t = 1;
void setup() {
  Serial.begin(9600);
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  // Open a writing and reading pipe on each radio, with opposite addresses
  /*if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }*/
  radio.setAutoAck(true);
  radio.openReadingPipe(1,addresses[0]);
  // Start the radio listening for data
  radio.startListening();
  connec = false;
  pinMode(10,OUTPUT);
}

void loop() {
    
    started_waiting_at = micros();
    while(!radio.available()){  
      if (micros() - started_waiting_at > 100000 ){            // If waited longer than 0.1s, indicate timeout and exit while loop
            connec=false;
            t=abs(t-1);
            break;
      }
      else{
        connec=true;
      }
    }
    digitalWrite(10,t?HIGH:LOW);
    if(connec){                                                                // Variable for the received timestamp
      
      while (radio.available()) {                                   // While there is data ready
        radio.read( &input, sizeof(int) );             // Get the payload
      }
      
      Serial.write(input & 0x00FF);
      Serial.write((input & 0xFF00) >> 8);
    }
    else{
      Serial.write(0);
    }
} // Loop
