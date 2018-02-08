#include "RF24.h"

RF24 NRF(7, 8);

//pipe[0] = allways listenning - setup pipe, [1] = Runtime pipe

uint64_t pipe[5] = { 0xAAAAAAAAAAAA, 0xBBBBBBBBBBBB };

void setup() {
	Serial.begin(115200);
	Serial.println("Sender!");
	NRF.begin();
	//new code - setup - Set nrf to full speed
	NRF.setPALevel(RF24_PA_MAX);
	NRF.setDataRate(RF24_2MBPS);
	NRF.setChannel(124);
	NRF.setRetries(0, 10);
	//end of new code
	Serial.println("setup OK!");
	NRF.openReadingPipe(1, pipe[0]);
	NRF.openReadingPipe(2, pipe[1]);
}


void SetRobot() {
	uint8_t pp;
	Serial.println("Turn the robot on now!");
	NRF.openReadingPipe(1, pipe[0]);
	while (1) {
		if (NRF.available(&pp))	if (pp == 1) {


		}
	}
}

uint64_t uint64_tRND(){
	return uint64_t(random()) << 32 + uint64_t(random());
}