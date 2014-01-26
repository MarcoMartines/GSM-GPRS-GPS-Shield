#ifndef _SMS_H_
#define _SMS_H_

#include "SIM900.h"

class SMSGSM
{
	public:
    // SMS's methods 
    char SendSMS(char *number_str, char *message_str);
    char SendSMS(byte sim_phonebook_position, char *message_str);
    char IsSMSPresent(byte required_status);
    char GetSMS(byte position, char *phone_number, char *SMS_text, byte max_SMS_len);
    
    char GetAuthorizedSMS(byte position, char *phone_number, char *SMS_text, byte max_SMS_len,
                          byte first_authorized_pos, byte last_authorized_pos);
    char DeleteSMS(byte position);

};

#endif

