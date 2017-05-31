#include "RF24.h"

//portas do mega  MOSI
				//MISO
				//

enum opts
{
	COUNTERCLOCKWISE = 0b0000,
	CLOCKWISE,
	MOTOR0 = 0b0000,
	MOTOR1 = 0b0010,
	MOTOR2,
	MOTOR3,
	ROBOT0 = 0b0000,
	ROBOT1 = 0b1000,
	ROBOT2,
	ROBOT3,
	ROBOT4,
	ROBOT5,
	ROBOT6,
	ROBOT7,
};


struct comando {
	unsigned char opt1;
	uint8_t opt2, opt3;
	bool dir;
};
RF24 NRF(7, 8);
const uint64_t pipe = 0xE8E8F0F0E1LL;
comando comm;
// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	Serial.println("Sender!");
	NRF.begin();
	//new code - setup
	NRF.setPALevel(RF24_PA_MAX);
	NRF.setDataRate(RF24_2MBPS);
	NRF.setChannel(124);
	NRF.setRetries(0, 10);
	//end of new code
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
		Serial.read();
		NRF.write(&comm, sizeof(comm));
		Serial.println("Sent!");
	}
}
