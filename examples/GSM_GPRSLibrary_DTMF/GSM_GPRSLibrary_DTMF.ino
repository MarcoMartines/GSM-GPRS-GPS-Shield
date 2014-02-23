#include "SIM900.h"
#include <SoftwareSerial.h>
//We don't need the http functions. So we can disable the next line.
//#include "inetGSM.h"
#include "sms.h"
#include "call.h"

//To change pins for Software Serial, use the two lines in GSM.cpp.

//GSM Shield for Arduino
//www.open-electronics.org
//this code is based on the example of Arduino Labs.

//Simple sketch to detect DTMF tones during a call

//We have to create the classes for calls.
CallGSM call;

char number[20];
byte stat=0;
int value=0;
int pin=1;
char value_str[5];
char DTMF_char='_';
int count=0;

void setup()
{
     pinMode(pin,INPUT);
     //Serial connection.
     Serial.begin(9600);
     Serial.println("GSM Shield testing.");
     //Start configuration of shield with baudrate.
     //For http uses is raccomanded to use 4800 or slower.
     if (gsm.begin(9600))
          Serial.println("\nstatus=READY");
     else Serial.println("\nstatus=IDLE");
     //Enable DTMF detection for SIM900
     call.SetDTMF(1);
};

void loop()
{
     //Chekcs status of call
     stat=call.CallStatus();
     //If the incoming call is from an authorized number
     //saved on SIM in the positions range from 1 to 3.
     if(stat==CALL_INCOM_VOICE) {
          Serial.println("Pick Up");
          delay(50);
          call.PickUp();
     }
     while(stat==CALL_ACTIVE_VOICE) {
          for (int k=0; k<100; k++) {
               DTMF_char=call.DetDTMF();
               if(DTMF_char!='-')
                    Serial.println(DTMF_char);
          }
          stat=call.CallStatus();

     }
     delay(1000);
};
