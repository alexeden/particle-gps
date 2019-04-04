#include "Particle.h"
#define GPSSerial Serial1

void setup();
void loop();

void setup() {
	Serial.begin(115200);

	GPSSerial.begin(9600);
}

void loop() {
	if (Serial.available()) {
		Serial.print("Writing characters to the GPS: ");

		while (Serial.available()) {
			char c = Serial.read();
			GPSSerial.write(c);
			Serial.print(c);
		}
		Serial.println();
	}
	if (GPSSerial.available()) {
		char c = GPSSerial.read();
		Serial.write(c);
	}
}
