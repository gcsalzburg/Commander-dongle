#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>

// Setup for LoRa radio
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
#define RF95_FREQ 868.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

// To flash
#define STATUS_LED LED_BUILTIN

// Message store for incoming Serial messages
char serial_msg[100] = "";
uint8_t serial_i = 0;

// Send/receive msg fns
void msg_receive();
void msg_send(char *msg, uint8_t len);

// RSSI transmission details
bool report_rssi = true;
int16_t last_rssi = 0;
     
//
// Setup
void setup(){
	pinMode(STATUS_LED, OUTPUT);
	pinMode(RFM95_RST, OUTPUT);
	digitalWrite(RFM95_RST, HIGH);

	// Begin Serial
	Serial.begin(115200);
	delay(100);

	// Manual reset of RFM95
	digitalWrite(RFM95_RST, LOW);
	delay(10);
	digitalWrite(RFM95_RST, HIGH);
	delay(10);

	while (!rf95.init()) {
		Serial.println("LoRa radio init failed, uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug");
		while (1);
	}
	Serial.println("LoRa init OK!");

	// Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
	if (!rf95.setFrequency(RF95_FREQ)) {
		// Set frequency failed?
		while (1);
	}
	// Transmitter power possible from 5 to 23 dBm
	rf95.setTxPower(23, false);
}

//
// Loop
void loop(){
	if(rf95.available()){
		msg_receive();
	}

	while(Serial.available()){
		char c = (char)Serial.read();
		if(c != '\n' && c != '\r'){
			serial_msg[serial_i++] = c;
		}else if(c == '\r'){
			// Send msg
			serial_msg[serial_i++] = '\0';
			msg_send(serial_msg, serial_i);
			serial_i = 0;
		}
	}
}


// Forward incoming RF95 msg to Serial
void msg_receive(){
	
	digitalWrite(STATUS_LED, HIGH);

	uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
	uint8_t len = sizeof(buf);

	if (rf95.recv(buf, &len)){

		// Send to Serial
		Serial.print((char*)buf);

		// Save last RSSI in case we want it
		last_rssi = rf95.lastRssi();

		if(report_rssi){
			Serial.print("|");
			Serial.print(last_rssi);
		}

		// Terminate messages with new line
		Serial.println();

	}else{
		// Receive must have failed, for some reason
	}
	digitalWrite(STATUS_LED, LOW);
}

// Send a message to the RF95
void msg_send(char *msg, uint8_t len){

	char msg_to_send[20] = "";
	strcpy(&msg_to_send[0], msg);
	
	// We need to set the len parameter here precisely because it affects the wait time.
	// e.g. 200 char array = ~350ms send time
	// 5 char array = ~30ms send time
	digitalWrite(STATUS_LED, HIGH);
	rf95.send((const uint8_t *)msg_to_send, len);
	rf95.waitPacketSent();
	digitalWrite(STATUS_LED, LOW);
}