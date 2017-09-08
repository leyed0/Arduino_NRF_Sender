#include "RF24.h"
#include <avr/wdt.h>

//struct com os dados necessarios para setar a velocidade dos motores
struct MotorCmd {
	uint8_t speed[2];
	bool direction[2];
};

//declarar o NRF - declarar portas utilizadas na comunicação serial
RF24 NRF(7, 8);
const uint64_t ack = 0x08E8F0F0E1LL;
const uint64_t pipe[3] = {0xE8E8F0F0E1LL,0xE8E8F0F0E10L,0xE8E8F0F0E0LL};
//cada pipe é o "endereço" de uma placa- Facilita o controle de varios robos ao mesmo tempo

MotorCmd MtCmd;
unsigned char CMDID;
uint8_t robot, rbt;

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

// command: CMDID|destiny|speed0|direction0|speed1|direction1.
void loop() {
	if (Serial.available())
	{
		CMDID = Serial.read();

		switch (CMDID) //verify the command type
		{
		case 'M':
			robot = Serial.parseInt();
			MtCmd.speed[0] = Serial.parseInt();
			MtCmd.direction[0] = Serial.parseInt();
			MtCmd.speed[1] = Serial.parseInt();
			MtCmd.direction[1] = Serial.parseInt();
			Serial.read();
			SendNRF(&CMDID, sizeof(CMDID));
			SendNRF(&MtCmd, sizeof(MtCmd));
			break;
		case 'S':
			robot = Serial.parseInt();
			SendNRF(&CMDID, sizeof(CMDID));
			rbt = Serial.parseInt();
			SendNRF(&rbt, sizeof(rbt));
			break;
		default:
			//remove garbage - may be trash
			Serial.println(Serial.read());
			Serial.println(Serial.read());
			Serial.println("Invalid command!");
			break;
		}
	}
}


void SendNRF(const void *buf, uint8_t len) {
	//delay(5);
	NRF.openWritingPipe(pipe[robot]);
	if (!NRF.write(buf, len)) {
		//Reboot();
		// On_Error("Error sending data!");
		Serial.flush();
	}
	return;
}


//em caso de erro, essa função deve ser chamada. Ela pode ser utilizada para detalhar o mesmo
void On_Error(String message) {
	pinMode(10, OUTPUT);
	Serial.println("error!");
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


//soft rebott - tem bugs
void Reboot()
{
	wdt_enable(WDTO_15MS);
	while (1)
	{
	}
}