#define ARDUINO 101

#include "Adafruit_GPS.h"
#include "Particle.h"
#include "TinyGPS++.h"
#include "spark-streaming.h"

#define PGCMD_ANTENNA "$PGCMD,33,1*6C" ///< request for updates on antenna status
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28" ///< turn on GPRMC and GGA
#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"              ///<  1 Hz

#define GPSSerial Serial1

// Adafruit_GPS GPS(&GPSSerial);
TinyGPSPlus gps;
TinyGPSCustom fix(gps, "GPGGA", 6);
TinyGPSCustom antenna(gps, "PGTOP", 2);

void setup();
void loop();

// Cloud function declarations
int cloudSetInterval(String ms);

uint32_t interval = 5000;
uint32_t timer	= millis();

void setup() {
	Serial.begin(115200);
	GPSSerial.begin(9600);
	GPSSerial.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	GPSSerial.println(PGCMD_ANTENNA);
	GPSSerial.println(PMTK_SET_NMEA_UPDATE_1HZ);

	// Variables
	if (!Particle.variable("interval", interval)) { Serial << "FAILED TO REGISTER interval VARIABLE!" << endl; }

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

void loop() {
	while (GPSSerial.available() > 0) gps.encode(GPSSerial.read());

	if (timer > millis()) timer = millis();

	if (millis() - timer > interval) {
		Serial << _FLOAT(gps.location.lat(), 6) << ", " << _FLOAT(gps.location.lng(), 6) << endl;
		Serial << "Satellites: " << gps.satellites.value() << endl;
		Serial << "Location age: " << gps.location.age() << endl;
		Serial << "Altitude in meters: " << gps.altitude.meters() << endl;
		Serial << "Speed (mps): " << gps.speed.mps() << endl;
		Serial << "Time: " << gps.time.value() << endl;
		Serial << "HDOP: " << gps.hdop.value() << endl;
		Serial << "Fix: " << fix.value() << endl;
		Serial << "Date: " << gps.date.value() << endl;
		Serial << "Antenna: " << antenna.value() << endl;
		timer = millis(); // reset the timer
	}
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

// void loop() {
// GPS.read();
// if (GPS.newNMEAreceived()) { GPS.parse(GPS.lastNMEA()); }

// if (timer > millis()) timer = millis();

// if (millis() - timer > interval) {
// 	CellularSignal sig = Cellular.RSSI();
// 	Serial << "Cell signal strength: " << sig.rssi << endl;
// 	Serial << "Cell signal quality: " << sig.qual << endl;
// 	Serial << "Cell access protocol: " << sig.getAccessTechnology() << endl;
// 	Serial << "Cell private IP: " << Cellular.localIP() << endl;
// 	Serial << "GPS fix: " << GPS.fix << endl;
// 	Serial << "GPS fix quality: " << GPS.fixquality << endl;
// 	Serial << "GPS satellites: " << GPS.satellites << endl;
// 	Serial << "Time: " << GPS.hour << ":" << GPS.minute << ":" << GPS.seconds << " " << GPS.milliseconds << endl;
// 	Serial << "Date: " << GPS.month << "/" << GPS.day << "/20" << GPS.year << endl;
// 	if (GPS.fix) {
// 		String msg = "";
// 		msg = msg + GPS.latitude + GPS.lat + ", " + GPS.longitude + GPS.lon;
//         Particle.publish("location", msg);
// 		Serial << "Location: " << GPS.latitude << GPS.lat << ", " << GPS.longitude << GPS.lon << endl;
// 		Serial << "Speed (knots): " << GPS.speed << endl;
// 		Serial << "Angle: " << GPS.angle << endl;
// 		Serial << "Altitude: " << GPS.altitude << endl;
// 		Serial << "Satellites: " << GPS.satellites << endl;
// 	}

// 	timer = millis(); // reset the timer
// }
// }
