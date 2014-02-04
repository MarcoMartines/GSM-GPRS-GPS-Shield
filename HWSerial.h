//#define MEGA

#ifndef _HWSERIAL_H_
#define _HWSERIAL_H_

#include "Arduino.h"
#include "Streaming.h"
#include <inttypes.h>


class HWSerial {
#ifdef MEGA
private:
     int write_error;
     size_t printNumber(unsigned long, uint8_t);
     size_t printFloat(double, uint8_t);

public:
     HWSerial();

     //bool listen();
     void end();
     /*
     bool isListening();
     bool overflow();
     */
     int peek();
     virtual void begin(long baud_rate);

     virtual size_t write(uint8_t byte);
     size_t write(const char *str) {
          return write((const uint8_t *)str, strlen(str));
     }
     virtual size_t write(const uint8_t *buffer, size_t size);

     virtual int read();
     virtual int available();
     virtual void flush();

     size_t print(const __FlashStringHelper *);
     size_t print(const String &);
     size_t print(const char[]);
     size_t print(char);
     size_t print(unsigned char, int = DEC);
     size_t print(int, int = DEC);
     size_t print(unsigned int, int = DEC);
     size_t print(long, int = DEC);
     size_t print(unsigned long, int = DEC);
     size_t print(double, int = 2);


     size_t println(const __FlashStringHelper *);
     size_t println(const String &s);
     size_t println(const char[]);
     size_t println(char);
     size_t println(unsigned char, int = DEC);
     size_t println(int, int = DEC);
     size_t println(unsigned int, int = DEC);
     size_t println(long, int = DEC);
     size_t println(unsigned long, int = DEC);
     size_t println(double, int = 2);

     size_t println(void);

     int getString( char *pre_string, char *post_string, char *buffer, int length);
     boolean find(char *target);
     boolean  findUntil(char *target, char *terminate);

#endif

};
#endif