/*
This is a Beta version.
last modified 18/08/2012.

This library is based on one developed by Arduino Labs
and it is modified to preserve the compability
with the Arduino's product.

The library is modified to use the GSM Shield,
developed by www.open-electronics.org
(http://www.open-electronics.org/arduino-gsm-shield/)
and based on SIM900 chip,
with the same commands of Arduino Shield,
based on QuectelM10 chip.
*/

#include "GSM.h"
#include "WideTextFinder.h"

//De-comment this two lines below if you have the
//first version of GSM GPRS Shield
//#define _GSM_TXPIN_ 4
//#define _GSM_RXPIN_ 5

//De-comment this two lines below if you have the
//second version og GSM GPRS Shield
#define _GSM_TXPIN_ 2
#define _GSM_RXPIN_ 3

#ifdef UNO
GSM::GSM():_cell(_GSM_TXPIN_,_GSM_RXPIN_),_tf(_cell, 10),_status(IDLE)
{
};
#endif
#ifdef MEGA
GSM::GSM()
{
     _cell.begin(9600);
};
#endif


int GSM::begin(long baud_rate)
{
	 // Set pin modes
	 pinMode(GSM_ON,OUTPUT);
	 pinMode(GSM_RESET,OUTPUT);

#ifdef UNO
     if (baud_rate==115200) {
          Serial.println(F("Don't use baudrate 115200 with Software Serial.\nAutomatically changed at 9600."));
          baud_rate=9600;
     }
#endif
     int response=-1;
     int cont=0;
     boolean norep=true;
     boolean turnedON=false;
     SetCommLineStatus(CLS_ATCMD);
     _cell.begin(baud_rate);
     p_comm_buf = &comm_buf[0];
     setStatus(IDLE);

     // if no-reply we turn to turn on the module
     for (cont=0; cont<3; cont++) {
          if (AT_RESP_ERR_NO_RESP == SendATCmdWaitResp(str_at, 500, 100, str_ok, 5)&&!turnedON) {		//check power
               // there is no response => turn on the module
#ifdef DEBUG_ON
               Serial.println(F("DB:NO RESP"));
#endif
               // generate turn on pulse
               digitalWrite(GSM_ON, HIGH);
               delay(1200);
               digitalWrite(GSM_ON, LOW);
               delay(10000);
               WaitResp(1000, 1000);
          } else {
#ifdef DEBUG_ON
               Serial.println(F("DB:ELSE"));
#endif
               WaitResp(1000, 1000);
          }
     }


     if (AT_RESP_OK == SendATCmdWaitResp(str_at, 500, 100, str_ok, 5)) {
#ifdef DEBUG_ON
          Serial.println(F("DB:CORRECT BR"));
#endif
          turnedON=true;
          norep=false;
     }


     if (AT_RESP_ERR_DIF_RESP == SendATCmdWaitResp(str_at, 500, 100, str_ok, 5)&&!turnedON) {		//check OK
#ifdef DEBUG_ON
          Serial.println(F("DB:AUTO BAUD RATE"));
#endif
          for (int i=0; i<8; i++) {
               switch (i) {
               case 0:
                    _cell.begin(1200);
                    break;

               case 1:
                    _cell.begin(2400);
                    break;

               case 2:
                    _cell.begin(4800);
                    break;

               case 3:
                    _cell.begin(9600);
                    break;

               case 4:
                    _cell.begin(19200);
                    break;

               case 5:
                    _cell.begin(38400);
                    break;

               case 6:
                    _cell.begin(57600);
                    break;

               case 7:
                    _cell.begin(115200);
                    break;

                    // if nothing else matches, do the default
                    // default is optional
               }

               delay(100);

#ifdef DEBUG_PRINT
               // parameter 0 - because module is off so it is not necessary
               // to send finish AT<CR> here
               DebugPrint(F("DEBUG: Stringa "), 0);
               DebugPrint(buff, 0);
#endif


               if (AT_RESP_OK == SendATCmdWaitResp(str_at, 500, 100, str_ok, 5)) {
#ifdef DEBUG_ON
                    Serial.println(F("DB:FOUND PREV BR"));
#endif
                    _cell.print("AT+IPR=");
                    _cell.print(baud_rate);
                    _cell.print("\r"); // send <CR>
                    delay(500);
                    _cell.begin(baud_rate);
                    delay(100);
                    if (AT_RESP_OK == SendATCmdWaitResp(str_at, 500, 100, str_ok, 5)) {
#ifdef DEBUG_ON
                         Serial.println(F("DB:OK BR"));
#endif
                    }
                    turnedON=true;
                    break;
               }
#ifdef DEBUG_ON
               Serial.println(F("DB:NO BR"));
#endif
          }
          // communication line is not used yet = free
          SetCommLineStatus(CLS_FREE);
          // pointer is initialized to the first item of comm. buffer
          p_comm_buf = &comm_buf[0];
     }

     if(norep==true&&!turnedON) {
          Serial.println(F("Trying to force the baud-rate to 9600\n"));
          for (int i=0; i<8; i++) {
               switch (i) {
               case 0:
                    _cell.begin(1200);
                    delay(1000);
                    Serial.println(F("1200"));
                    _cell.print(F("AT+IPR=9600\r"));
                    delay(1000);
                    _cell.begin(9600);
                    delay(1000);
                    SendATCmdWaitResp(str_at, 500, 100, str_ok, 5);
                    delay(1000);
                    WaitResp(1000,1000);
                    break;

               case 1:
                    _cell.begin(2400);
                    delay(1000);
                    Serial.println(F("2400"));
                    _cell.print(F("AT+IPR=9600\r"));
                    delay(1000);
                    _cell.begin(9600);
                    delay(1000);
                    SendATCmdWaitResp(str_at, 500, 100, str_ok, 5);
                    delay(1000);
                    WaitResp(1000,1000);
                    break;

               case 2:
                    _cell.begin(4800);
                    delay(1000);
                    Serial.println(F("4800"));
                    _cell.print(F("AT+IPR=9600\r"));
                    delay(1000);
                    _cell.begin(9600);
                    delay(1000);
                    SendATCmdWaitResp(str_at, 500, 100, str_ok, 5);
                    delay(1000);
                    WaitResp(1000,1000);
                    break;

               case 3:
                    _cell.begin(9600);
                    delay(1000);
                    Serial.println(F("9600"));
                    _cell.print(F("AT+IPR=9600\r"));
                    delay(1000);
                    _cell.begin(9600);
                    delay(1000);
                    SendATCmdWaitResp(str_at, 500, 100, str_ok, 5);
                    delay(1000);
                    WaitResp(1000,1000);
                    break;

               case 4:
                    _cell.begin(19200);
                    delay(1000);
                    Serial.println(F("19200"));
                    _cell.print(F("AT+IPR=9600\r"));
                    delay(1000);
                    _cell.begin(9600);
                    delay(1000);
                    SendATCmdWaitResp(str_at, 500, 100, str_ok, 5);
                    delay(1000);
                    WaitResp(1000,1000);
                    break;

               case 5:
                    _cell.begin(38400);
                    delay(1000);
                    Serial.println(F("38400"));
                    _cell.print(F("AT+IPR=9600\r"));
                    delay(1000);
                    _cell.begin(9600);
                    delay(1000);
                    SendATCmdWaitResp(str_at, 500, 100, str_ok, 5);
                    delay(1000);
                    WaitResp(1000,1000);
                    break;

               case 6:
                    _cell.begin(57600);
                    delay(1000);
                    Serial.println(F("57600"));
                    _cell.print(F("AT+IPR=9600\r"));
                    delay(1000);
                    _cell.begin(9600);
                    delay(1000);
                    SendATCmdWaitResp(str_at, 500, 100, str_ok, 5);
                    delay(1000);
                    WaitResp(1000,1000);
                    break;

               case 7:
                    _cell.begin(115200);
                    delay(1000);
                    Serial.println(F("115200"));
                    _cell.print(F("AT+IPR=9600\r"));
                    delay(1000);
                    _cell.begin(9600);
                    delay(1000);
                    SendATCmdWaitResp(str_at, 500, 100, str_ok, 5);
                    delay(1000);
                    WaitResp(1000,1000);
                    break;
               }
          }

          Serial.println(F("ERROR: SIM900 doesn't answer. Check power and serial pins in GSM.cpp"));
          digitalWrite(GSM_ON, HIGH);
          delay(1200);
          digitalWrite(GSM_ON, LOW);
          delay(10000);
          return 0;
     }

     SetCommLineStatus(CLS_FREE);

     if(turnedON) {
          WaitResp(50, 50);
          InitParam(PARAM_SET_0);
          InitParam(PARAM_SET_1);//configure the module
          Echo(0);               //enable AT echo
          setStatus(READY);
          return(1);

     } else {
          //just to try to fix some problems with 115200 baudrate
          _cell.begin(115200);
          delay(1000);
          _cell.print("AT+IPR=");
          _cell.print(baud_rate);
          _cell.print("\r"); // send <CR>
          return(0);
     }
}


void GSM::InitParam(byte group)
{
     switch (group) {
     case PARAM_SET_0:
          // check comm line
          //if (CLS_FREE != GetCommLineStatus()) return;

          SetCommLineStatus(CLS_ATCMD);
          // Reset to the factory settings
          SendATCmdWaitResp(F("AT&F"), 1000, 50, str_ok, 5);
          // switch off echo
          SendATCmdWaitResp(F("ATE0"), 500, 50, str_ok, 5);
          // setup fixed baud rate
          //SendATCmdWaitResp("AT+IPR=9600", 500, 50, str_ok, 5);
          // setup mode
          //SendATCmdWaitResp("AT#SELINT=1", 500, 50, str_ok, 5);
          // Switch ON User LED - just as signalization we are here
          //SendATCmdWaitResp("AT#GPIO=8,1,1", 500, 50, str_ok, 5);
          // Sets GPIO9 as an input = user button
          //SendATCmdWaitResp("AT#GPIO=9,0,0", 500, 50, str_ok, 5);
          // allow audio amplifier control
          //SendATCmdWaitResp("AT#GPIO=5,0,2", 500, 50, str_ok, 5);
          // Switch OFF User LED- just as signalization we are finished
          //SendATCmdWaitResp("AT#GPIO=8,0,1", 500, 50, str_ok, 5);
          SetCommLineStatus(CLS_FREE);
          break;

     case PARAM_SET_1:
          // check comm line
          //if (CLS_FREE != GetCommLineStatus()) return;
          SetCommLineStatus(CLS_ATCMD);
          // Request calling line identification
          SendATCmdWaitResp(F("AT+CLIP=1"), 500, 50, str_ok, 5);
          // Mobile Equipment Error Code
          SendATCmdWaitResp(F("AT+CMEE=0"), 500, 50, str_ok, 5);
          // Echo canceller enabled
          //SendATCmdWaitResp("AT#SHFEC=1", 500, 50, str_ok, 5);
          // Ringer tone select (0 to 32)
          //SendATCmdWaitResp("AT#SRS=26,0", 500, 50, str_ok, 5);
          // Microphone gain (0 to 7) - response here sometimes takes
          // more than 500msec. so 1000msec. is more safety
          //SendATCmdWaitResp("AT#HFMICG=7", 1000, 50, str_ok, 5);
          // set the SMS mode to text
          SendATCmdWaitResp(F("AT+CMGF=1"), 500, 50, str_ok, 5);
          // Auto answer after first ring enabled
          // auto answer is not used
          //SendATCmdWaitResp("ATS0=1", 500, 50, str_ok, 5);
          // select ringer path to handsfree
          //SendATCmdWaitResp("AT#SRP=1", 500, 50, str_ok, 5);
          // select ringer sound level
          //SendATCmdWaitResp("AT+CRSL=2", 500, 50, str_ok, 5);
          // we must release comm line because SetSpeakerVolume()
          // checks comm line if it is free
          SetCommLineStatus(CLS_FREE);
          // select speaker volume (0 to 14)
          //SetSpeakerVolume(9);
          // init SMS storage
          InitSMSMemory();
          // select phonebook memory storage
          SendATCmdWaitResp(F("AT+CPBS=\"SM\""), 1000, 50, str_ok, 5);
          SendATCmdWaitResp(F("AT+CIPSHUT"), 500, 50, "SHUT OK", 5);
          break;
     }
}

byte GSM::WaitResp(uint16_t start_comm_tmout, uint16_t max_interchar_tmout,
                   char const *expected_resp_string)
{
     byte status;
     byte ret_val;

     RxInit(start_comm_tmout, max_interchar_tmout);
     // wait until response is not finished
     do {
          status = IsRxFinished();
     } while (status == RX_NOT_FINISHED);

     if (status == RX_FINISHED) {
          // something was received but what was received?
          // ---------------------------------------------

          if(IsStringReceived(expected_resp_string)) {
               // expected string was received
               // ----------------------------
               ret_val = RX_FINISHED_STR_RECV;
          } else {
               ret_val = RX_FINISHED_STR_NOT_RECV;
          }
     } else {
          // nothing was received
          // --------------------
          ret_val = RX_TMOUT_ERR;
     }
     return (ret_val);
}


/**********************************************************
Method sends AT command and waits for response

return:
      AT_RESP_ERR_NO_RESP = -1,   // no response received
      AT_RESP_ERR_DIF_RESP = 0,   // response_string is different from the response
      AT_RESP_OK = 1,             // response_string was included in the response
**********************************************************/
char GSM::SendATCmdWaitResp(char const *AT_cmd_string,
                            uint16_t start_comm_tmout, uint16_t max_interchar_tmout,
                            char const *response_string,
                            byte no_of_attempts)
{
     byte status;
     char ret_val = AT_RESP_ERR_NO_RESP;
     byte i;

     for (i = 0; i < no_of_attempts; i++) {
          // delay 500 msec. before sending next repeated AT command
          // so if we have no_of_attempts=1 tmout will not occurred
          if (i > 0) delay(500);

          _cell.println(AT_cmd_string);
          status = WaitResp(start_comm_tmout, max_interchar_tmout);
          if (status == RX_FINISHED) {
               // something was received but what was received?
               // ---------------------------------------------
               if(IsStringReceived(response_string)) {
                    ret_val = AT_RESP_OK;
                    break;  // response is OK => finish
               } else ret_val = AT_RESP_ERR_DIF_RESP;
          } else {
               // nothing was received
               // --------------------
               ret_val = AT_RESP_ERR_NO_RESP;
          }

     }

     WaitResp(1000, 5000);
     return (ret_val);
}


/**********************************************************
Method sends AT command and waits for response

return:
      AT_RESP_ERR_NO_RESP = -1,   // no response received
      AT_RESP_ERR_DIF_RESP = 0,   // response_string is different from the response
      AT_RESP_OK = 1,             // response_string was included in the response
**********************************************************/
char GSM::SendATCmdWaitResp(const __FlashStringHelper *AT_cmd_string,
                            uint16_t start_comm_tmout, uint16_t max_interchar_tmout,
                            char const *response_string,
                            byte no_of_attempts)
{
     byte status;
     char ret_val = AT_RESP_ERR_NO_RESP;
     byte i;

     for (i = 0; i < no_of_attempts; i++) {
          // delay 500 msec. before sending next repeated AT command
          // so if we have no_of_attempts=1 tmout will not occurred
          if (i > 0) delay(500);

          _cell.println(AT_cmd_string);
          status = WaitResp(start_comm_tmout, max_interchar_tmout);
          if (status == RX_FINISHED) {
               // something was received but what was received?
               // ---------------------------------------------
               if(IsStringReceived(response_string)) {
                    ret_val = AT_RESP_OK;
                    break;  // response is OK => finish
               } else ret_val = AT_RESP_ERR_DIF_RESP;
          } else {
               // nothing was received
               // --------------------
               ret_val = AT_RESP_ERR_NO_RESP;
          }

     }

     return (ret_val);
}

byte GSM::WaitResp(uint16_t start_comm_tmout, uint16_t max_interchar_tmout)
{
     byte status;

     RxInit(start_comm_tmout, max_interchar_tmout);
     // wait until response is not finished
     do {
          status = IsRxFinished();
     } while (status == RX_NOT_FINISHED);
     return (status);
}

byte GSM::IsRxFinished(void)
{
     byte num_of_bytes;
     byte ret_val = RX_NOT_FINISHED;  // default not finished

     // Rx state machine
     // ----------------

     if (rx_state == RX_NOT_STARTED) {
          // Reception is not started yet - check tmout
          if (!_cell.available()) {
               // still no character received => check timeout
               /*
               #ifdef DEBUG_GSMRX

               		DebugPrint("\r\nDEBUG: reception timeout", 0);
               		Serial.print((unsigned long)(millis() - prev_time));
               		DebugPrint("\r\nDEBUG: start_reception_tmout\r\n", 0);
               		Serial.print(start_reception_tmout);


               #endif
               */
               if ((unsigned long)(millis() - prev_time) >= start_reception_tmout) {
                    // timeout elapsed => GSM module didn't start with response
                    // so communication is takes as finished
                    /*
                    	#ifdef DEBUG_GSMRX
                    		DebugPrint("\r\nDEBUG: RECEPTION TIMEOUT", 0);
                    	#endif
                    */
                    comm_buf[comm_buf_len] = 0x00;
                    ret_val = RX_TMOUT_ERR;
               }
          } else {
               // at least one character received => so init inter-character
               // counting process again and go to the next state
               prev_time = millis(); // init tmout for inter-character space
               rx_state = RX_ALREADY_STARTED;
          }
     }

     if (rx_state == RX_ALREADY_STARTED) {
          // Reception already started
          // check new received bytes
          // only in case we have place in the buffer
          num_of_bytes = _cell.available();
          // if there are some received bytes postpone the timeout
          if (num_of_bytes) prev_time = millis();

          // read all received bytes
          while (num_of_bytes) {
               num_of_bytes--;
               if (comm_buf_len < COMM_BUF_LEN) {
                    // we have still place in the GSM internal comm. buffer =>
                    // move available bytes from circular buffer
                    // to the rx buffer
                    *p_comm_buf = _cell.read();

                    p_comm_buf++;
                    comm_buf_len++;
                    comm_buf[comm_buf_len] = 0x00;  // and finish currently received characters
                    // so after each character we have
                    // valid string finished by the 0x00
               } else {
                    // comm buffer is full, other incoming characters
                    // will be discarded
                    // but despite of we have no place for other characters
                    // we still must to wait until
                    // inter-character tmout is reached

                    // so just readout character from circular RS232 buffer
                    // to find out when communication id finished(no more characters
                    // are received in inter-char timeout)
                    _cell.read();
               }
          }

          // finally check the inter-character timeout
          /*
          #ifdef DEBUG_GSMRX

          		DebugPrint("\r\nDEBUG: intercharacter", 0);
          <			Serial.print((unsigned long)(millis() - prev_time));
          		DebugPrint("\r\nDEBUG: interchar_tmout\r\n", 0);
          		Serial.print(interchar_tmout);


          #endif
          */
          if ((unsigned long)(millis() - prev_time) >= interchar_tmout) {
               // timeout between received character was reached
               // reception is finished
               // ---------------------------------------------

               /*
               #ifdef DEBUG_GSMRX

               	DebugPrint("\r\nDEBUG: OVER INTER TIMEOUT", 0);
               #endif
               */
               comm_buf[comm_buf_len] = 0x00;  // for sure finish string again
               // but it is not necessary
               ret_val = RX_FINISHED;
          }
     }


     return (ret_val);
}

/**********************************************************
Method checks received bytes

compare_string - pointer to the string which should be find

return: 0 - string was NOT received
        1 - string was received
**********************************************************/
byte GSM::IsStringReceived(char const *compare_string)
{
     char *ch;
     byte ret_val = 0;

     if(comm_buf_len) {
          /*
          	#ifdef DEBUG_GSMRX
          		DebugPrint("DEBUG: Compare the string: \r\n", 0);
          		for (int i=0; i<comm_buf_len; i++){
          			Serial.print(byte(comm_buf[i]));
          		}

          		DebugPrint("\r\nDEBUG: with the string: \r\n", 0);
          		Serial.print(compare_string);
          		DebugPrint("\r\n", 0);
          	#endif
          */
#ifdef DEBUG_ON
          Serial.print("ATT: ");
          Serial.println(compare_string);
          Serial.print("RIC: ");
          Serial.println((char *)comm_buf);
#endif
          ch = strstr((char *)comm_buf, compare_string);
          if (ch != NULL) {
               ret_val = 1;
               /*#ifdef DEBUG_PRINT
               DebugPrint("\r\nDEBUG: expected string was received\r\n", 0);
               #endif
               */
          } else {

               /*#ifdef DEBUG_PRINT
               DebugPrint("\r\nDEBUG: expected string was NOT received\r\n", 0);
               #endif
               */
          }
     } else {
#ifdef DEBUG_ON
          Serial.print(F("ATT: "));
          Serial.println(compare_string);
          Serial.print(F("RIC: NO STRING RCVD"));
#endif
     }

     return (ret_val);
}


void GSM::RxInit(uint16_t start_comm_tmout, uint16_t max_interchar_tmout)
{
     rx_state = RX_NOT_STARTED;
     start_reception_tmout = start_comm_tmout;
     interchar_tmout = max_interchar_tmout;
     prev_time = millis();
     comm_buf[0] = 0x00; // end of string
     p_comm_buf = &comm_buf[0];
     comm_buf_len = 0;
     _cell.flush(); // erase rx circular buffer
}

void GSM::Echo(byte state)
{
     if (state == 0 or state == 1) {
          SetCommLineStatus(CLS_ATCMD);

          _cell.print("ATE");
          _cell.print((int)state);
          _cell.print("\r");
          delay(500);
          SetCommLineStatus(CLS_FREE);
     }
}

char GSM::InitSMSMemory(void)
{
     char ret_val = -1;

     if (CLS_FREE != GetCommLineStatus()) return (ret_val);
     SetCommLineStatus(CLS_ATCMD);
     ret_val = 0; // not initialized yet

     // Disable messages about new SMS from the GSM module
     SendATCmdWaitResp(F("AT+CNMI=2,0"), 1000, 50, str_ok, 2);

     // send AT command to init memory for SMS in the SIM card
     // response:
     // +CPMS: <usedr>,<totalr>,<usedw>,<totalw>,<useds>,<totals>
     if (AT_RESP_OK == SendATCmdWaitResp(F("AT+CPMS=\"SM\",\"SM\",\"SM\""), 1000, 1000, "+CPMS:", 10)) {
          ret_val = 1;
     } else ret_val = 0;

     SetCommLineStatus(CLS_FREE);
     return (ret_val);
}

int GSM::isIP(const char* cadena)
{
     int i;
     for (i=0; i<strlen(cadena); i++)
          if (!(cadena[i]=='.' || ( cadena[i]>=48 && cadena[i] <=57)))
               return 0;
     return 1;
}






