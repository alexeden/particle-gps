#include "ArduinoJson.h"
#include "Particle.h"
#include "spark-streaming.h"
#include "TinyGPS++.h"

#define PGCMD_ANTENNA "$PGCMD,33,1*6C" ///< request for updates on antenna status
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28" ///< turn on GPRMC and GGA
#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"              ///<  1 Hz
#define GPSSerial Serial1


void setup();
void loop();

// Cloud function declarations
int cloudSetInterval(String ms);

// Cloud publish timers
uint32_t interval = 30000;
uint32_t timer	= millis();

// Cloud publish document
const int event_fields = JSON_OBJECT_SIZE(11);
StaticJsonDocument<event_fields> doc;
char serialized_doc[512];

// GPS
TinyGPSPlus gps;
TinyGPSCustom fix(gps, "GPGGA", 6);
TinyGPSCustom antenna(gps, "PGTOP", 2);


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

	// One-time doc updates
	// doc["deviceId"].set(serialized("\"" + System.deviceID() + "\""));
	// doc["version"].set(serialized("\"" + System.version() + "\""));
	// doc["versionNumber"] = System.versionNumber();
	doc["lat"] = 0;
	doc["lng"] = 0;
	doc["locationAge"] = -1;
	doc["satellites"] = 0;
	doc["altitude"] = 0;
	doc["speed"] = 0;
	doc["date"] = 0;
	doc["time"] = 0;
	doc["hdop"] = 0;
	doc["hasFix"] = false;
	doc["antenna"] = 0;
}

void loop() {
	while (GPSSerial.available() > 0) gps.encode(GPSSerial.read());

	if (gps.location.isUpdated()) {
		doc["lat"] = gps.location.lat();
		doc["lng"] = gps.location.lng();
	}

	doc["locationAge"] = gps.location.age();
	doc["satellites"] = gps.satellites.value();
	doc["altitude"] = gps.altitude.meters();
	doc["speed"] = gps.speed.mps();
	doc["date"] = gps.date.value();
	doc["time"] = gps.time.value();
	doc["hdop"] = gps.hdop.value();
	doc["hasFix"] = fix.value() > 0 && gps.satellites.value() > 0;
	doc["antenna"] = antenna.value();


	if (timer > millis()) timer = millis();

	if (millis() - timer > interval) {
		serializeJson(doc, serialized_doc);
		Particle.publish("location", serialized_doc, PUBLIC);
		Serial << serialized_doc << endl;
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
