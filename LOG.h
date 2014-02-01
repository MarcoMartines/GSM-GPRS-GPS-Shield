/*
  LOG.h - Library for standard logging convention.
  Created by Meir Michanie, June 9, 2010.
  Released into the public domain.
  Version 0.1
*/

#ifndef LOG_h
#define LOG_h
#include "Arduino.h"

class LOG {
public:
     LOG(int level);
     void DEBUG(const char* string);
     void INFO(const char* string);
     void WARNING(const char* string);
     void CRITICAL(const char* string);
     void DATA(const char* string);
     void DATA(int number);

     inline int  getLevel(void)      {
          return _level;
     }
     inline void setLevel(int level) {
          _level = level;
     }

private:
     int _level;
};

#endif
