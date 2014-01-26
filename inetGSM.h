#ifndef _INETGSM_H_
#define _INETGSM_H_

#define BUFFERSIZE 1

#include "SIM900.h"

class InetGSM
{
  private:
    char _buffer[BUFFERSIZE];
    
  public:
    int httpGET(const char* server, int port, const char* path, char* result, int resultlength);
    int httpPOST(const char* server, int port, const char* path, const char* parameters, char* result, int resultlength);
    
    // Fast and dirty solution. Should make a "mail" object. And by the moment it does not run.
    int openmail(char* server, char* loginbase64, char* passbase64, char* from, char* to, char* subj);
    int closemail();
    int attachGPRS(char* domain, char* dom1, char* dom2);
    int dettachGPRS();
    int connectTCP(const char* server, int port);
    int disconnectTCP();
    int connectTCPServer(int port);
    boolean connectedClient();

    // This runs, yes
    //int tweet(const char* token, const char* msg);

};

#endif
