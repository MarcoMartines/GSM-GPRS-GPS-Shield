#ifndef WideTextFinder_h
#define WideTextFinder_h

//#include <WProgram.h>
#include <Arduino.h>
#include <SoftwareSerial.h>


class WideTextFinder {
private:
     SoftwareSerial* nSerialStream;

     unsigned long timeout;    // number of seconds to wait for the next char before aborting read
     unsigned long startMillis; // used for timeout measurement
     boolean debug;

     char read();             // private function to read from the stream

public:
     // constructor:
     // default timeout is 5 seconds
     WideTextFinder(SoftwareSerial &stream, int timeout = 5);          // Ethernet constructor

     // Manage debug
     void setDebug(boolean d);

     // find methods - these seek through the data but do not return anything
     // they are useful to skip past unwanted data
     //
     boolean find(char *target);   // reads data from the stream until the target string is found
     // returns true if target string is found

     boolean findUntil(char *target, char *terminate);   // as above but search ends if the terminate string is found


     // get methods - these get a numeric value or string from the data stream
     //
     long getValue();    // returns the first valid (long) integer value from the current position.
     // initial characters that are not digits (or the minus sign) are skipped
     // integer is terminated by the first character that is not a digit.

     long getValue(char skipChar); // as above but the given skipChar is ignored
     // this allows format characters (typically commas) in values to be ignored

     float getFloat();  // float version of getValue
     float getFloat(char skipChar);  // as above but the given skipChar is ignored

     int getString( char *pre_string, char *post_string, char *buffer, int length); //puts string found between given delimiters in buffer
     // string will be truncated to fit the buffer length
     // end of string determined by a match of a character to the first char of close delimiter
     // returns the number of characters placed in the buffer (0 means no valid data found)

     unsigned long getTimeout();   // returns the number of seconds to wait for the next char before aborting read

     // set methods
     //
     void setTimeout(unsigned long timeout);  // set a new value for the wait timeout

};

#endif
