/*
 * simple JSONP server 
 */
#include <EtherCard.h>
#include <map>

#define DEBUG 1;

// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
// ethernet interface ip address
// static byte myip[] = { 192,168,1,5 };
// gateway ip address
// static byte gwip[] = { 192,168,1,1 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer


// Some stuff for responding to the request
char* on = "ON";
char* off = "OFF";
char* statusLabel;
char* buttonLabel;

// Standard http response headers

char http200[] PROGMEM =
		"HTTP/1.0 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		;

char http200j[] PROGMEM =
		"HTTP/1.0 200 OK\r\n"
		"Content-Type: application/javascript\r\n"
		"\r\n"
		;

char http400[] PROGMEM =
		"HTTP/1.0 400 Bad Request\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"<html>"
		"<head><title>Error 400</title></head>"
		"<body>"
		"<h3>400: Bad Request</h3>"
		"</body>"
		"</html>"
		;

char http403[] PROGMEM =
		"HTTP/1.0 403 Forbidden\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"<html>"
		"<head><title>Error 403</title></head>"
		"<body>"
		"<h3>403: Forbidden</h3>"
		"</body>"
		"</html>"
		;

char http404[] PROGMEM =
		"HTTP/1.0 404 Not Found\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"<html>"
		"<head><title>Error 404</title></head>"
		"<body>"
		"<h3>404: Not Found</h3>"
		"</body>"
		"</html>"
		;

char http503[] PROGMEM =
		"HTTP/1.0 503 Service Unavailable\r\n"
		"Content-Type: text/html\r\n"
		"Retry-After: 600\r\n"
		"\r\n"
		"<html>"
		"<head><title>Error 503</title></head>"
		"<body>"
		"<h3>503: Service Unavailable</h3>"
		"</body>"
		"</html>"
		;


void setup(){
	if(DEBUG) {
		Serial.begin(57600);
		Serial.println("BasicServer Demo");
	}
	// initialize the EtherCard
	ether.begin(sizeof Ethernet::buffer, mymac);
	// Set IP using Static
	// ether.staticSetup(myip, gwip);

	if (!ether.begin(sizeof Ethernet::buffer, mymac, 10)) {
		if(DEBUG)
			Serial.println("Failed to access Ethernet controller");
	} else {
		if(DEBUG)
			Serial.println("Ethernet controller initialized");
	}

	if (!ether.dhcpSetup()) {
		if(DEBUG)
			Serial.println("Failed to get configuration from DHCP");
	} else {
		if(DEBUG) {
			Serial.println("DHCP configuration done");
			Serial.println("IP Address:\t" + ether.myip);
			Serial.println("Netmask:\t" + ether.mymask);
			Serial.println("Gateway:\t" + ether.gwip);
		}
	}

}

void loop(){

	word len = ether.packetReceive();
	word pos = ether.packetLoop(len);

	// IF LED2=ON turn it ON
	if(strstr((char *)Ethernet::buffer + pos, "GET /?LED2=ON") != 0) {
		Serial.println("Received ON command");
		digitalWrite(ledPin2, HIGH);
	}

	// IF LED2=OFF turn it OFF
	if(strstr((char *)Ethernet::buffer + pos, "GET /?LED2=OFF") != 0) {
		Serial.println("Received OFF command");
		digitalWrite(ledPin2, LOW);
	}

	// IF LED4=ON turn it ON
	if(strstr((char *)Ethernet::buffer + pos, "GET /?LED4=ON") != 0) {
		Serial.println("Received ON command");
		digitalWrite(ledPin4, HIGH);
	}

	// IF LED4=OFF turn it OFF
	if(strstr((char *)Ethernet::buffer + pos, "GET /?LED4=OFF") != 0) {
		Serial.println("Received OFF command");
		digitalWrite(ledPin4, LOW);
	}

	//Return a page so the request is completed.

	memcpy_P(ether.tcpOffset(), page, sizeof page);
	ether.httpServerReply(sizeof page - 1);
}
