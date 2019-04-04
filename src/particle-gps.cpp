#include "Particle.h"
#define GPSSerial Serial1

void setup();
void loop();

void setup() {
	Serial.begin(115200);

	GPSSerial.begin(9600);
}

void loop() {
	if (GPSSerial.available()) {
		char c = GPSSerial.read();
		Serial.write(c);
	}
}
