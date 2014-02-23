#include "HWSerial.h"
#ifdef MEGA
HWSerial::HWSerial()
{
     Serial1.begin(9600);

}
/*
bool HWSerial::listen(){
	return Serial1.listen();
}
*/
void HWSerial::end()
{
     Serial1.end();
}
/*
bool HWSerial::isListening(){
	return Serial1.isListening();
}

bool HWSerial::overflow(){
	return Serial1.overflow();
}
*/
void HWSerial::begin(long baud_rate)
{
     Serial1.begin(baud_rate);
}

int HWSerial::peek()
{
     return Serial1.peek();
}

size_t HWSerial::write(uint8_t byte)
{
     return Serial1.write(byte);
}

int HWSerial::read()
{
     return Serial1.read();
}

int HWSerial::available()
{
     return Serial1.available();
}

void HWSerial::flush()
{
     Serial1.flush();
}


/**************************************/



size_t HWSerial::print(const __FlashStringHelper *ifsh)
{
     const prog_char *p = (const prog_char *)ifsh;
     size_t n = 0;
     while (1) {
          unsigned char c = pgm_read_byte(p++);
          if (c == 0) break;
          n += write(c);
     }
     return n;
}

size_t HWSerial::print(const String &s)
{
     size_t n = 0;
     for (uint16_t i = 0; i < s.length(); i++) {
          n += write(s[i]);
     }
     return n;
}

size_t HWSerial::print(const char str[])
{
     return write(str);
}

size_t HWSerial::print(char c)
{
     return write(c);
}

size_t HWSerial::print(unsigned char b, int base)
{
     return print((unsigned long) b, base);
}

size_t HWSerial::print(int n, int base)
{
     return print((long) n, base);
}

size_t HWSerial::print(unsigned int n, int base)
{
     return print((unsigned long) n, base);
}

size_t HWSerial::print(long n, int base)
{
     if (base == 0) {
          return write(n);
     } else if (base == 10) {
          if (n < 0) {
               int t = print('-');
               n = -n;
               return printNumber(n, 10) + t;
          }
          return printNumber(n, 10);
     } else {
          return printNumber(n, base);
     }
}

size_t HWSerial::print(unsigned long n, int base)
{
     if (base == 0) return write(n);
     else return printNumber(n, base);
}

size_t HWSerial::print(double n, int digits)
{
     return printFloat(n, digits);
}

size_t HWSerial::println(const __FlashStringHelper *ifsh)
{
     size_t n = print(ifsh);
     n += println();
     return n;
}


size_t HWSerial::println(void)
{
     size_t n = print('\r');
     n += print('\n');
     return n;
}

size_t HWSerial::println(const String &s)
{
     size_t n = print(s);
     n += println();
     return n;
}


size_t HWSerial::println(const char c[])
{
     size_t n = print(c);
     n += println();
     return n;
}

size_t HWSerial::println(char c)
{
     size_t n = print(c);
     n += println();
     return n;
}

size_t HWSerial::println(unsigned char b, int base)
{
     size_t n = print(b, base);
     n += println();
     return n;
}

size_t HWSerial::println(int num, int base)
{
     size_t n = print(num, base);
     n += println();
     return n;
}

size_t HWSerial::println(unsigned int num, int base)
{
     size_t n = print(num, base);
     n += println();
     return n;
}

size_t HWSerial::println(long num, int base)
{
     size_t n = print(num, base);
     n += println();
     return n;
}

size_t HWSerial::println(unsigned long num, int base)
{
     size_t n = print(num, base);
     n += println();
     return n;
}

size_t HWSerial::println(double num, int digits)
{
     size_t n = print(num, digits);
     n += println();
     return n;
}


/******************************/
size_t HWSerial::write(const uint8_t *buffer, size_t size)
{
     size_t n = 0;
     while (size--) {
          n += write(*buffer++);
     }
     return n;
}

size_t HWSerial::printNumber(unsigned long n, uint8_t base)
{
     char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
     char *str = &buf[sizeof(buf) - 1];

     *str = '\0';

     // prevent crash if called with base == 1
     if (base < 2) base = 10;

     do {
          unsigned long m = n;
          n /= base;
          char c = m - base * n;
          *--str = c < 10 ? c + '0' : c + 'A' - 10;
     } while(n);

     return write(str);
}

size_t HWSerial::printFloat(double number, uint8_t digits)
{
     size_t n = 0;

     // Handle negative numbers
     if (number < 0.0) {
          n += print('-');
          number = -number;
     }

     // Round correctly so that print(1.999, 2) prints as "2.00"
     double rounding = 0.5;
     for (uint8_t i=0; i<digits; ++i)
          rounding /= 10.0;

     number += rounding;

     // Extract the integer part of the number and print it
     unsigned long int_part = (unsigned long)number;
     double remainder = number - (double)int_part;
     n += print(int_part);

     // Print the decimal point, but only if there are digits beyond
     if (digits > 0) {
          n += print(".");
     }

     // Extract digits from the remainder one at a time
     while (digits-- > 0) {
          remainder *= 10.0;
          int toPrint = int(remainder);
          n += print(toPrint);
          remainder -= toPrint;
     }

     return n;
}



boolean  HWSerial::find(char *target)
{
     return findUntil(target, NULL);
}


boolean  HWSerial::findUntil(char *target, char *terminate)
{
     byte targetLen = strlen(target);
     byte index = 0;  // maximum target string length is 255 bytes
     byte termIndex = 0;
     byte termLen = (terminate == NULL ? 0 : strlen(terminate));
     char c;

     if( *target == 0)
          return true;   // return true if target is a null string
     while( (c = read()) != 0) {
          if( c == target[index]) {
               if(++index >= targetLen) { // return true if all chars in the target match
                    return true;
               }
          } else {
               index = 0;  // reset index if any char does not match
          }
          if(termLen > 0 && c == terminate[termIndex]) {
               if(++termIndex >= termLen)
                    return false;       // return false if terminate string found before target string
          } else
               termIndex = 0;
     }
     return false;
}



int HWSerial::getString( char *pre_string, char *post_string, char *buffer, int length)
{
     if( find(pre_string) ) {
          int index = 0;
          *buffer = 0;
          while(index < length-1 ) {
               char c = read();
               if( c == 0 ) {
                    return 0;   // timeout returns 0 !
               } else if( c == *post_string ) {

                    while (index < length) {
                         buffer[index] = '\0'; // terminate the string !!!!!!!!!!!IT DOESN'T WORK!!!!!!!!!!!!!!!!!
                         index++;
                    }

                    //buffer[index]=0;
                    return index;               // data got successfully
               } else {
                    buffer[index++] = c;
               }
          }
          buffer[index] = 0;
          return index; // Note: buffer full before the closing post_string encountered
     }
     return 0;    //failed to find the prestring
}
#endif