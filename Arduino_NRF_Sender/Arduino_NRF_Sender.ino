#include "RF24.h"
#include <avr/wdt.h>

//opts - em desenvolvimento para troca de dados
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

struct command {
	unsigned char opt1;
	uint8_t opt2, opt3;
	bool dir;
};

RF24 NRF(7, 8);
const uint64_t pipe[3] = {0xE8E8F0F0E1LL,0xE8E8F0F0E10L,0xE8E8F0F0E0LL};
command comm;
uint8_t robot;

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
	Serial.println("setup OK!");
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (Serial.available())
	{
		comm.opt1 = Serial.read();
		robot = Serial.parseInt();
		comm.opt2 = Serial.parseInt() - 1;
		comm.opt3 = Serial.parseInt();
		if (Serial.parseInt() == 0) comm.dir = false;
		else comm.dir = true;
		Serial.read();
		SendNRF(robot, &comm, sizeof(comm));
		Serial.println("Sent!");
	}
}

void SendNRF(uint8_t robot, const void *buf, uint8_t len) {
	NRF.openWritingPipe(pipe[robot]);
	if (!NRF.write(buf, sizeof(len))) On_Error("Error sending data!");
}

void On_Error(String message) {
	pinMode(10, OUTPUT);
	while (true) {
		if (Serial.available()) {
			switch (Serial.read()) {
			case 'p':
				Serial.println(message);
				Serial.println("'c' to continue, 'r' to reboot");
				while (!Serial.available());
				break;
			case 'c':
				return;
				break;
			case 'r':
				Reboot();
				break;
			default:
				break;
			}
		}
		digitalWrite(10, !digitalRead(10));
		delay(500);
	}
}

void Reboot()
{
	wdt_enable(WDTO_15MS);
	while (1)
	{
	}
}