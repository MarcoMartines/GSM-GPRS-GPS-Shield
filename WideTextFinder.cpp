#include "WideTextFinder.h"

#define NO_SKIP_CHAR  1  // a magic char not found in a valid numeric field

// private function to read characters from stream
// the constructors allow one and only one of the streams to be initialized
  char WideTextFinder::read()
  {   
    char r;
    startMillis = millis();  
    if (nSerialStream != NULL)
    {     
      while(millis() < (startMillis + timeout)) 
      {
        if (nSerialStream->available() > 0)
	  {
          r=nSerialStream->read();
		  //if(debug)
			//Serial.print(r);
          return r;
        }
      }
    }
    return 0;     // 0 indicates timeout     
  }

  // constructors 
  //default timeout is 5 seconds
  
  WideTextFinder::WideTextFinder(SoftwareSerial &stream, int timeout) :
                  nSerialStream(&stream) 
  { 
    this->timeout = timeout * 1000L;     
	debug=true;
  }
  
  void WideTextFinder::setDebug(boolean d)
  {
	debug=d;
  }

  // public methods
  //
  // find returns true if the target string is found
  boolean  WideTextFinder::find(char *target)
  {
    return findUntil(target, NULL);
  }
 
  // as above but search ends if the terminate string is found
  boolean  WideTextFinder::findUntil(char *target, char *terminate)
  {
    byte targetLen = strlen(target); 
    byte index = 0;  // maximum target string length is 255 bytes
    byte termIndex = 0;
    byte termLen = (terminate == NULL ? 0 : strlen(terminate));  
    char c;
 
    if( *target == 0)
       return true;   // return true if target is a null string 
    while( (c = read()) != 0){   
      if( c == target[index]){        
        if(++index >= targetLen){ // return true if all chars in the target match
          return true;    
        }
      }
      else{        
        index = 0;  // reset index if any char does not match   
      }
      if(termLen > 0 && c == terminate[termIndex]){
         if(++termIndex >= termLen)  
           return false;       // return false if terminate string found before target string
      }
      else
          termIndex = 0;
    }
    return false;
  }

    // places the string between the prestring  and poststring in the given buffer
    // buffer must be one more than the longest string to get
    // the string will be truncated to fit the buffer length
    // end of string determined by a single character match to the first char of poststring
    // returns the number of characters placed in the buffer (0 means no valid data found)
  int WideTextFinder::getString( char *pre_string, char *post_string, char *buffer, int length)
  {  
    if( find(pre_string) ){ 
      int index = 0;   
      *buffer = 0;
      while(index < length-1 ){
        char c = read();
        if( c == 0 ){
          return 0;   // timeout returns 0 !
        }
        else if( c == *post_string ){
	  
          while (index < length){
          	buffer[index] = '\0'; // terminate the string !!!!!!!!!!!IT DOESN'T WORK!!!!!!!!!!!!!!!!!
		index++;
		}
          
	  //buffer[index]=0;
          return index;               // data got successfully
        }
        else{
          buffer[index++] = c;   
        }
      }
      buffer[index] = 0;
      return index; // Note: buffer full before the closing post_string encountered
    }   
    return 0;    //failed to find the prestring
  } 

  // getValue method:
  // returns the first valid (long) integer value from the current position.
  // initial characters that are not digits (or the minus sign) are skipped
  // function is terminated by the first character that is not a digit.
  long WideTextFinder::getValue()
  {
      return getValue(NO_SKIP_CHAR); // terminate on first non-digit character 
  }
  
  // as above but a given skipChar is ignored
  // this allows format characters (typically commas) in values to be ignored
  long WideTextFinder::getValue(char skipChar)
  {
      boolean isNegative = false;
      long value = 0;   
      char c;

      while( (c = read()) != '-' && (c < '0' || c > '9') && (c!=0) )
         ;// ignore non numeric leading characters  
      do{ 
        if(c == skipChar )
           ; // ignore this charactor        
        else if (c==0) // Timeout
           return 0;
        else if(c == '-') 
           isNegative = true;          
        else if(c >= '0' && c <= '9')        // is c a digit?  
           value = value * 10 + c - '0';         
        c = read();        
      }
      while( (c >= '0' && c <= '9')  || c == skipChar );
      if(isNegative)
        value = -value;   
     return value;
  }

  // float version of getValue method:
  // as integer version but returns a floating point value
    float WideTextFinder::getFloat()
    {
       getFloat(NO_SKIP_CHAR);
    }

  // as above but the given skipChar is ignored
  // this allows format characters (typically commas) in values to be ignored  
  float WideTextFinder::getFloat(char skipChar){
      boolean isNegative = false;
      boolean isFraction = false;
      long value = 0;  
      float fValue;       
      char c;
      float fraction = 1.0;

      while( (c = read()) != '-' && (c < '0' || c > '9') )
         ; // ignore non numeric leading characters
      do{    
        if(c == skipChar)
           ; // ignore 
        else if(c == '-') 
           isNegative = true;     
        else if (c == '.')
            isFraction = true;
        else if(c >= '0' && c <= '9')  {      // is c a digit?  
           value = value * 10 + c - '0'; 
           if(isFraction)
              fraction *= 0.1;
        }           
        c = read();     
      }
      while( (c >= '0' && c <= '9')  || c == '.' || c == skipChar );
      if(isNegative)
        value = -value;      
      if(isFraction)
        return value * fraction;        
      else  
        return value;
  }


  // returns the number of seconds to wait for the next char before aborting read
  unsigned long WideTextFinder::getTimeout()
  {
	   return timeout;
  }

  // set a new value for the wait timeout
  void WideTextFinder::setTimeout(unsigned long timeout)
  {
	   this->timeout = timeout * 1000L;
  }
