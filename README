GSM GPRS Shield for Arduino Uno

For informations and support:
http://code.google.com/p/gsm-shield-arduino/
http://www.open-electronics.org/arduino-gsm-shield/
http://www.futurashop.it/

The below functions are always available in the library.

int attachGPRS(char* domain, char* dom1, char* dom2)
	establishes a GPRS connection with domain using authentication (if needed)
	parameters and return values:
	domain: pointer to a string that contains the domain address of GPRS connection
	dom1: pointer to the username string (don't use if not needed)
	dom2: pointer to the password string (don't use if not needed)
	return:
	0 - unable to establish a GPRS connection 
	1 - connection successfully established

int dettachGPRS()
	disconnects the GPRS connection

int connectTCP(const char* server, int port)
	starts a socket connection to the server at specific port
	parameters and return values:
	server: pointer to the server address string, it can be the IP or normal address
	port: port used to establish a connection
	e.g. gsm.connectTCP("www.google.it",80)
	e.g. gsm.connectTCP("74.125.39.106",80)
	return:
	0 - unable to start the TCP connection
	1 - connected to the server as client
	Sample: GSM_GPRSLibrary_Client

int disconnectTCP()
	stops the TCP connection to the server

int connectTCPServer(int port)
	puts the SIM900 into server mode, waiting for TCP client connection on the specified port.
	parameters and return values:
	port: port used for establish a connection
	return:
	0 - unable to start server connection
	1 - server started successfully, waiting for client connection
	Sample: GSM_GPRSLibrary_Server

void SimpleRead()
	simple way to read from the newsoftserial and print the value on the hardware serial
	Sample: GSM_GPRSLibrary_AT

void SimpleWrite(char *comm)
	simple way to write a string on newsoftserial
	parameters and return values:
	comm: pointer to the string to write
	Sample: GSM_GPRSLibrary_AT

For memory saving, next functions are included in a class, so before use them is needed to include the header and to create the class.

For calling functions is necessary to include the next two lines
include "call.h";
CallGSM call_classname; //the name you choose must be the same in the functions below

void Call(char *number_string)
	calls the specific number
	e.g. call_classname.Call(“+390123456789″);

void Call(int sim_position)
	calls the number stored at the specified SIM position
	e.g. call_classname.Call(1); // call to the number stored at the 1st SIM position

void HangUp(void)
	hangs up call(incoming or active)
	e.g. call_classname.HangUp();

void PickUp(void)
	picks up the incoming call
	e.g. call_classname.PickUp();

For using SMS functions is necessary to include the next two lines
include "sms.h";
SMSGSM sms_classname; //the name you choose must be the same in the functions below

char SendSMS(char *number_str, char *message_str)
	sends SMS to the specific phone number
	parameters and return values:
	number_str: pointer to the phone number string
	message_str: pointer to the SMS text string
	return:
	ERROR ret. val:
	—————
	-1 – comm. line to the GSM module is not free
	-2 – GSM module didn’t answer in timeout
	-3 – GSM module has answered “ERROR” string
	OK ret val:
	———–
	0 – SMS was not sent
	1 – SMS was sent 9/15
	example of use:
	sms_classname.SendSMS(“00XXXYYYYYYYYY”, “SMS text”);

char SendSMS(byte sim_phonebook_position, char *message_str)
	sends SMS to the specified SIM phonebook position
	parameters and return values:
	sim_phonebook_position: SIM phonebook position <1..20>
	message_str: pointer to the SMS text string
	return:
	ERROR ret. val:
	—————
	-1 – comm. line to the GSM module is not free
	-2 – GSM module didn’t answer in timeout
	-3 – specified position must be > 0
	OK ret val:
	———–
	0 – SMS was not sent
	1 – SMS was sent
	an example of usage:
	GSM gsm;
	include "sms.h";
	SMSGSM sms_classname;
	sms_classname.SendSMS(1, “SMS text”);

char IsSMSPresent(byte required_status)
	finds out if there is present at least one SMS with specified status
	if there is new SMS before IsSMSPresent() is executed this SMS has a status UNREAD and then after calling IsSMSPresent() method status of SMS is automatically changed to READ
	parameters and return values:
	required_status:
	SMS_UNREAD – new SMS – not read yet
	SMS_READ – already read SMS
	SMS_ALL – all stored SMS
	return:
	ERROR ret. val:
	—————
	-1 – comm. line to the GSM module is not free
	-2 – GSM module didn’t answer in timeout
	OK ret val:
	———–
	0 – there is no SMS with specified status
	1..20 – position where SMS is stored
	example of use:
	char position;
	char phone_number[20]; // array for the phone number string
	char *sms_text;
	position = sms_classname.IsSMSPresent(SMS_UNREAD);
	if (position) { // read new SMS
	sms_classname.GetGSM(position, tel_number, &sms_text);
	}

char GetSMS(byte position, char *phone_number, char *SMS_text, byte max_SMS_len)
	reads SMS from specified memory(SIM) position
	parameters and return values:
	position: SMS position <1..20>
	phone_number: a pointer where the phone number string of received SMS will be placed
	so the space for the phone number string must be reserved – see example
	SMS_text : a pointer where SMS text will be placed
	max_SMS_len: maximum length of SMS text excluding also string terminating 0×00 character
	return:
	ERROR ret. val:
	—————
	-1 – comm. line to the GSM module is not free
	-2 – GSM module didn’t answer in timeout
	-3 – specified position must be > 0
	OK ret val:
	———–
	GETSMS_NO_SMS – no SMS was found at the specified position
	GETSMS_UNREAD_SMS – new SMS was found at the specified position
	GETSMS_READ_SMS – already read SMS was found at the specified position
	GETSMS_OTHER_SMS – other type of SMS was found an example of usage:
	GSM gsm;
	include "sms.h";
	SMSGSM sms_classname;
	char position;
	char phone_num[20]; // array for the phone number string
	char sms_text[100]; // array for the SMS text string
	position = sms_classname.IsSMSPresent(SMS_UNREAD);
	if (position) {
	// there is new SMS => read it
	sms_classname.GetGSM(position, phone_num, sms_text, 100);
	Serial.println(“DEBUG SMS phone number: “, 0);
	Serial.println(phone_num, 0);
	Serial.println(“\r\n SMS text: “, 0);
	Serial.println(sms_text, 1);
	}

char GetAuthorizedSMS( byte position, char *phone_number, char *SMS_text, byte max_SMS_len, byte first_authorized_pos, byte last_authorized_pos)
	reads SMS from specified memory(SIM) position and makes authorization -
	it means SMS phone number is compared with specified SIM phonebook position(s) and in case numbers match GETSMS_AUTH_SMS is returned, otherwise GETSMS_NOT_AUTH_SMS is returned
	parameters and return values:
	position: SMS position to be read <1..20>
	phone_number: a pointer where the tel. number string of received SMS will be placed so the space for the phone number string must be reserved – see example
	SMS_text : a pointer where SMS text will be placed
	max_SMS_len: maximum length of SMS text excluding terminating 0×00 character
	first_authorized_pos: initial SIM phonebook position where the authorization process starts
	last_authorized_pos: last SIM phonebook position where the authorization proces finishes
	Note(important):
	================
	In case first_authorized_pos=0 and also last_authorized_pos=0
	the received SMS phone number is NOT authorized at all, so every
	SMS is considered as authorized (GETSMS_AUTH_SMS is returned)
	return:
	ERROR ret. val:
	—————
	-1 – comm. line to the GSM module is not free
	-2 – GSM module didn’t answer in timeout
	-3 – position must be > 0
	OK ret val:
	———–
	GETSMS_NO_SMS – no SMS was found at the specified position
	GETSMS_NOT_AUTH_SMS – NOT authorized SMS found at the specified position
	GETSMS_AUTH_SMS – authorized SMS found at the specified position
	an example of usage:
	GSM gsm;
	include "sms.h";
	SMSGSM sms_classname;
	char phone_num[20]; // array for the phone number string 12/15
	char sms_text[100]; // array for the SMS text string
	// authorize SMS with SIM phonebook positions 1..3
	if (GETSMS_AUTH_SMS == sms_classname.GetAuthorizedSMS(1, phone_num, sms_text, 100, 1, 3)) {
	// new authorized SMS was detected at the SMS position 1
	Serial.println(“DEBUG SMS phone number: “, 0);
	Serial.println(phone_num, 0);
	Serial.println(“\r\n SMS text: “, 0);
	Serial.println(sms_text, 1);
	}
	// don’t authorize SMS with SIM phonebook at all
	if (GETSMS_AUTH_SMS == sms_classname.GetAuthorizedSMS(1, phone_num, sms_text, 100, 0, 0)) {
	// new SMS was detected at the SMS position 1
	// because authorization was not required
	// SMS is considered authorized
	Serial.println(“DEBUG SMS phone number: “, 0);
	Serial.println(phone_num, 0);
	Serial.println(“\r\n SMS text: “, 0);
	Serial.println(sms_text, 1);
	}

char DeleteSMS(byte position)
	deletes SMS from specified SMS position
	parameters and return values:
	position: SMS position <1..20>
	return:
	ERROR ret. val:
	—————
	-1 – comm. line to the GSM module is not free
	-2 – GSM module didn’t answer in timeout
	-3 – position must be > 0
	OK ret val:
	———–
	0 – SMS was not deleted
	1 – SMS was deleted

For HTTP functions is necessary to include the next two lines
include "inetGSM.h";
inetGSM inet_classname; //the name you choose must be the same in the functions below

int httpGET(const char* server, int port, const char* path, char* result, int resultlength)
	Send a GET request to server, at specified port, for the requested path and save the reply in a string
	parameters and return values:
	server: the name or IP address of the server
	port: number of port used for the connection
	path: pointer to the string that contains the requested path e.g. / or /test/test.html
	result: pointer to the reply string from the server
	resultlength: size of the string with result
	return:
	0 - nothing received, maybe there is an error
	other - number of byte received
	Sample: GSM_GPRSLibrary_Client

