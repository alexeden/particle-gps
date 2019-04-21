#include "Adafruit_GPS.h"
#include "Particle.h"
#include "spark-streaming.h"

#define GPSSerial Serial1

Adafruit_GPS GPS(&GPSSerial);

void setup();
void loop();

// Cloud function declarations
int cloudSetInterval(String ms);

uint32_t interval = 60000;
uint32_t timer	= millis();

void setup() {
	Serial.begin(115200);
	GPS.begin(9600);
	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	GPS.sendCommand(PGCMD_ANTENNA);
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

	// Variables
	if (!Particle.variable("interval", interval)) {
		Serial << "FAILED TO REGISTER interval VARIABLE!" << endl;
	}

    // Functions
    if (!Particle.function("setInterval", cloudSetInterval)) {
		Serial << "FAILED TO REGISTER setInterval FUNCTION!" << endl;
    }

	Serial << "Device ID: " << System.deviceID() << endl;
	Serial << "System reset enabled: " << System.resetEnabled() << endl;
	Serial << "System updates enabled: " << System.updatesEnabled() << endl;
	Serial << "System updates pending: " << System.updatesPending() << endl;
	Serial << "System version: " << System.version() << endl;
	Serial << "System version number: " << System.versionNumber() << endl;
	Serial << "System wake up pin: " << System.wakeUpPin() << endl;
	Serial << "System wake up reason: " << System.wakeUpReason() << endl;
	Serial << "System was woken up by pin: " << System.wokenUpByPin() << endl;
	Serial << "System was woken up by RTC: " << System.wokenUpByRtc() << endl;
}

int cloudSetInterval(String ms) {
    int newInterval = ms.toInt();
    if (newInterval < 1000 || newInterval % 1000 != 0) {
        Serial << "Interval must be greater than 0 and a factor of 1000" << endl;
        return -1;
    }
    else {
        interval = newInterval;
        Serial << "Interval set to " << interval << endl;
        return 0;
    }
}

void loop() {
	GPS.read();
	if (GPS.newNMEAreceived()) { GPS.parse(GPS.lastNMEA()); }

	if (timer > millis()) timer = millis();

	if (millis() - timer > interval) {
		CellularSignal sig = Cellular.RSSI();
		Serial << "Cell signal strength: " << sig.rssi << endl;
		Serial << "Cell signal quality: " << sig.qual << endl;
		Serial << "Cell access protocol: " << sig.getAccessTechnology() << endl;
		Serial << "Cell private IP: " << Cellular.localIP() << endl;
		Serial << "GPS fix: " << GPS.fix << endl;
		Serial << "GPS fix quality: " << GPS.fixquality << endl;
		Serial << "GPS satellites: " << GPS.satellites << endl;
		Serial << "Time: " << GPS.hour << ":" << GPS.minute << ":" << GPS.seconds << " " << GPS.milliseconds << endl;
		Serial << "Date: " << GPS.month << "/" << GPS.day << "/20" << GPS.year << endl;
		if (GPS.fix) {
			String msg = "";
			msg = msg + GPS.latitude + GPS.lat + ", " + GPS.longitude + GPS.lon;
            Particle.publish("location", msg);
			Serial << "Location: " << GPS.latitude << GPS.lat << ", " << GPS.longitude << GPS.lon << endl;
			Serial << "Speed (knots): " << GPS.speed << endl;
			Serial << "Angle: " << GPS.angle << endl;
			Serial << "Altitude: " << GPS.altitude << endl;
			Serial << "Satellites: " << GPS.satellites << endl;
		}

		timer = millis(); // reset the timer
	}
}
