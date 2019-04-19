#include "Particle.h"
#include "Adafruit_GPS.h"
#include "spark-streaming.h"

#define PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ "$PMTK220,10000*2F" ///< Once every 10 seconds, 100 millihertz.
#define PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ "$PMTK220,5000*1B"  ///< Once every 5 seconds, 200 millihertz.
#define PMTK_SET_NMEA_UPDATE_1HZ "$PMTK220,1000*1F"				///<  1 Hz
#define PMTK_SET_NMEA_UPDATE_2HZ "$PMTK220,500*2B"				///<  2 Hz
#define PMTK_SET_NMEA_UPDATE_5HZ "$PMTK220,200*2C"				///<  5 Hz
#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"				///< 10 Hz
// // Position fix update rate commands.
#define PMTK_API_SET_FIX_CTL_100_MILLIHERTZ "$PMTK300,10000,0,0,0,0*2C" ///< Once every 10 seconds, 100 millihertz.
#define PMTK_API_SET_FIX_CTL_200_MILLIHERTZ "$PMTK300,5000,0,0,0,0*18"  ///< Once every 5 seconds, 200 millihertz.
#define PMTK_API_SET_FIX_CTL_1HZ "$PMTK300,1000,0,0,0,0*1C"				///< 1 Hz
#define PMTK_API_SET_FIX_CTL_5HZ "$PMTK300,200,0,0,0,0*2F"				///< 5 Hz

#define PMTK_SET_BAUD_57600 "$PMTK251,57600*2C" ///< 57600 bps
#define PMTK_SET_BAUD_9600 "$PMTK251,9600*17"   ///<  9600 bps

#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29" ///< turn on only the second sentence (GPRMC)
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"  ///< turn on GPRMC and GGA
#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28" ///< turn on ALL THE DATA
#define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"	 ///< turn off output

#define PMTK_LOCUS_STARTLOG "$PMTK185,0*22"			///< Start logging data
#define PMTK_LOCUS_STOPLOG "$PMTK185,1*23"			///< Stop logging data
#define PMTK_LOCUS_STARTSTOPACK "$PMTK001,185,3*3C" ///< Acknowledge the start or stop command
#define PMTK_LOCUS_QUERY_STATUS "$PMTK183*38"		///< Query the logging status
#define PMTK_LOCUS_ERASE_FLASH "$PMTK184,1*22"		///< Erase the log flash data

#define PMTK_ENABLE_SBAS "$PMTK313,1*2E" ///< Enable search for SBAS satellite (only works with 1Hz output rate)
#define PMTK_ENABLE_WAAS "$PMTK301,2*2E" ///< Use WAAS for DGPS correction data

#define PMTK_STANDBY "$PMTK161,0*28"			 ///< standby command & boot successful message
#define PMTK_STANDBY_SUCCESS "$PMTK001,161,3*36" ///< Not needed currently
#define PMTK_AWAKE "$PMTK010,002*2D"			 ///< Wake up

#define PMTK_Q_RELEASE "$PMTK605*31" ///< ask for the release and version

#define PGCMD_ANTENNA "$PGCMD,33,1*6C"   ///< request for updates on antenna status
#define PGCMD_NOANTENNA "$PGCMD,33,0*6D" ///< don't show antenna status messages
#define GPSSerial Serial1
#define GPSECHO false

Adafruit_GPS GPS(&GPSSerial);

void setup();
void loop();


void setup() {
	Cellular.off();
	Serial.begin(115200);
	GPS.begin(9600);
	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	GPS.sendCommand(PGCMD_ANTENNA);
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

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
	Serial << "GPS fix: " << GPS.fix << endl;
	Serial << "GPS fix quality: " << GPS.fixquality << endl;
	Serial << "GPS satellites: " << GPS.satellites << endl;
	delay(2000);

	// if (Serial.available()) {
	// 	Serial.print("Writing characters to the GPS: ");

	// 	while (Serial.available()) {
	// 		char c = Serial.read();
	// 		GPSSerial.write(c);
	// 		Serial.print(c);
	// 	}
	// 	Serial.println();
	// }
	// if (GPSSerial.available()) {
	// 	char c = GPSSerial.read();
	// 	Serial.write(c);
	// }
}
