#include "RF24.h"

//portas do mega  MOSI
				//MISO
				//

struct comando {
	unsigned char opt1, verify;
	uint8_t opt2, opt3;
	bool dir;
};
RF24 NRF(7, 8);
byte addresses[][6] = { "1Node","2Node" };
const uint64_t pipe = 0xE8E8F0F0E1LL;
comando comm;
// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	Serial.println("Sender!");
	NRF.begin();
	NRF.openWritingPipe(pipe);
	Serial.println("setup OK!");
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (Serial.available())
	{
		comm.opt1 = Serial.read();
		comm.opt2 = Serial.parseInt() - 1;
		comm.opt3 = Serial.parseInt();
		if (Serial.parseInt() == 0) comm.dir = false;
		else comm.dir = true;
		comm.verify = Serial.read();
		Serial.println("comand received!");
		Serial.println("sending!");
		NRF.write(&comm, sizeof(comm));
		Serial.println("Sent!");
		Serial.println(comm.opt1);
		Serial.println(comm.opt2);
		Serial.println(comm.opt3);
		Serial.println(comm.dir);
	}
}


//#include <RF24_config.h>
//#include <printf.h>
//#include <nRF24L01.h>
//#include "RF24.h"
//
//RF24 NRF(9, 10);
//const uint64_t pipe = 0xE8E8F0F0E1LL;
//unsigned char opt;
//void setup() {
//	Serial.begin(9600);
//	Serial.write("Sender!");
//	NRF.begin();
//	NRF.openWritingPipe(pipe);
//	Serial.write("setup ok!");
//}
//
//void loop() {
//	if (Serial.available()) {
//		opt = Serial.read();
//		Serial.println("sending...");
//		NRF.write(&opt, sizeof(unsigned char));
//		Serial.println("Done!");
//	}
//}
//
// Name:		Arduino_NRF_Sender.ino
// Created:	5/22/2017 9:07:54 PM
// Author:	orley
//*/
