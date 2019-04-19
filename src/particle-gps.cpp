#include "Adafruit_EPD.h"
#include "Particle.h"

#define SD_CS D2
#define SRAM_CS -1
#define EPD_CS D4
#define EPD_DC D5
// #define SD_CS D5
// #define SRAM_CS -1
// #define EPD_CS D9
// #define EPD_DC D10

// #define SD_CS       5
// #define SRAM_CS     -1
// #define EPD_CS      9
// #define EPD_DC      10

#define EPD_RESET -1 // can set to -1 and share with microcontroller Reset!
#define EPD_BUSY -1  // can set to -1 to not use a pin (will wait a fixed delay)
#define REDRAW_DELAY_SEC 90

// #define PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ "$PMTK220,10000*2F" ///< Once every 10 seconds, 100 millihertz.
// #define PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ "$PMTK220,5000*1B"  ///< Once every 5 seconds, 200 millihertz.
// #define PMTK_SET_NMEA_UPDATE_1HZ "$PMTK220,1000*1F"				///<  1 Hz
// #define PMTK_SET_NMEA_UPDATE_2HZ "$PMTK220,500*2B"				///<  2 Hz
// #define PMTK_SET_NMEA_UPDATE_5HZ "$PMTK220,200*2C"				///<  5 Hz
// #define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"				///< 10 Hz
// // Position fix update rate commands.
// #define PMTK_API_SET_FIX_CTL_100_MILLIHERTZ "$PMTK300,10000,0,0,0,0*2C" ///< Once every 10 seconds, 100 millihertz.
// #define PMTK_API_SET_FIX_CTL_200_MILLIHERTZ "$PMTK300,5000,0,0,0,0*18"  ///< Once every 5 seconds, 200 millihertz.
// #define PMTK_API_SET_FIX_CTL_1HZ "$PMTK300,1000,0,0,0,0*1C"				///< 1 Hz
// #define PMTK_API_SET_FIX_CTL_5HZ "$PMTK300,200,0,0,0,0*2F"				///< 5 Hz

// #define PMTK_SET_BAUD_57600 "$PMTK251,57600*2C" ///< 57600 bps
// #define PMTK_SET_BAUD_9600 "$PMTK251,9600*17"   ///<  9600 bps

// #define PMTK_SET_NMEA_OUTPUT_RMCONLY \
// 	"$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29" ///< turn on only the second sentence (GPRMC)
// #define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"  ///< turn on GPRMC and GGA
// #define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28" ///< turn on ALL THE DATA
// #define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"	 ///< turn off output

// // to generate your own sentences, check out the MTK command datasheet and use a checksum calculator
// // such as the awesome http://www.hhhh.org/wiml/proj/nmeaxor.html

// #define PMTK_LOCUS_STARTLOG "$PMTK185,0*22"			///< Start logging data
// #define PMTK_LOCUS_STOPLOG "$PMTK185,1*23"			///< Stop logging data
// #define PMTK_LOCUS_STARTSTOPACK "$PMTK001,185,3*3C" ///< Acknowledge the start or stop command
// #define PMTK_LOCUS_QUERY_STATUS "$PMTK183*38"		///< Query the logging status
// #define PMTK_LOCUS_ERASE_FLASH "$PMTK184,1*22"		///< Erase the log flash data

// #define PMTK_ENABLE_SBAS "$PMTK313,1*2E" ///< Enable search for SBAS satellite (only works with 1Hz output rate)
// #define PMTK_ENABLE_WAAS "$PMTK301,2*2E" ///< Use WAAS for DGPS correction data

// #define PMTK_STANDBY "$PMTK161,0*28"			 ///< standby command & boot successful message
// #define PMTK_STANDBY_SUCCESS "$PMTK001,161,3*36" ///< Not needed currently
// #define PMTK_AWAKE "$PMTK010,002*2D"			 ///< Wake up

// #define PMTK_Q_RELEASE "$PMTK605*31" ///< ask for the release and version

// #define PGCMD_ANTENNA "$PGCMD,33,1*6C"   ///< request for updates on antenna status
// #define PGCMD_NOANTENNA "$PGCMD,33,0*6D" ///< don't show antenna status messages
// #define GPSSerial Serial1

void setup();
void loop();

Adafruit_IL0373 epd(212, 104, EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

void setup() {
	// Cellular.off();
	Serial.begin(115200);
	Serial.println("I'm the main serial port");
	// GPSSerial.begin(9600);

	epd.begin();
	epd.setTextWrap(true);
	epd.setTextSize(2);
	Serial.println("EDP started");
	Serial.print("Display width: ");
	Serial.println(epd.width());
	Serial.print("Display height: ");
	Serial.println(epd.height());
	delay(2500);
	Serial.println("Drawing");
	// Draw some text
	epd.clearBuffer();
	epd.setCursor(10, 10);
	epd.setTextColor(EPD_BLACK);
	epd.print("Do neat shit.");
	epd.setCursor(50, 70);
	// epd.setTextColor(EPD_RED);
	epd.print("-- Alex Eden");
	epd.display();
	Serial.println("Done drawing!");
}

void loop() {
	// epd.clearBuffer();
	// for (int16_t i = 0; i < epd.width(); i += 4) { epd.drawLine(0, 0, i, epd.height() - 1, EPD_BLACK); }
	// // for (int16_t i = 0; i < epd.height(); i += 4) { epd.drawLine(epd.width() - 1, 0, 0, i, EPD_RED); }
	// epd.display();
	// Serial.println("Lines drawn, starting delay");

	// delay(1000 * REDRAW_DELAY_SEC);

	// // // Draw some text
	// epd.clearBuffer();
	// epd.setCursor(10, 10);
	// epd.setTextColor(EPD_BLACK);
	// epd.print("Do neat shit.");
	// epd.setCursor(50, 70);
	// epd.setTextColor(EPD_RED);
	// epd.print("-- Alex Eden");
	// epd.display();

	// delay(1000 * REDRAW_DELAY_SEC);
}
/*
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
*/
