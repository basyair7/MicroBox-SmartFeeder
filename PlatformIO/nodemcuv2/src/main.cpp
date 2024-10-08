/*! @file main.cpp
 *  @version 1.0.0
*/

#include "main.h"

#include "TypeLittleFS"
#include "BootButton.h"
#include "ultrasonic.h"
#include "ProgramWiFi.h"
#include "WebServer.h"
// #include "SerialData.h"
#include "I2C_Master.h"

#include "WIFISETTING.h"
#include "variable.h"

#include "envBlynk.h"
#include <BlynkSimpleEsp8266.h>
#include <EEPROM.h>

BlynkTimer TIMER;

ProgramWiFi programWiFi;
WebServer webServer(80);
// SerialData serialData(RX_TX, TX_RX);
I2C_Master i2cMaster(I2C_SLAVE_ADDR, JSONSIZE);

BootButton bootBtn(BOOTBUTTON, INPUT);
Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);

unsigned long LastTimeGetDistance = 0, LastTimeSendCapacity = 0;
unsigned long LastTimeLED = 0, LastTimeMonitor = 0;
bool __switch_state__ = false, __auto_state__ = false, ESPRestart = false;
float distance; int capacity;

void __indikator__(void) {
	if ((unsigned long) (millis() - LastTimeLED) >= 100) {
		LastTimeLED = millis();

		if (capacity > 0) {
			digitalWrite(LED_YELLOW, HIGH);
			digitalWrite(LED_RED, LOW);
		}
		else {
			digitalWrite(LED_YELLOW, LOW);
			digitalWrite(LED_RED, HIGH);
		}
	}
}

void SwitchRecovery(bool state) {
	EEPROM.put(EEPROM_BLYNK_ADDR, state);
	EEPROM.commit();
	delay(5);
}

void __get_capacity__(void) {
	if ((unsigned long) (millis() - LastTimeGetDistance) >= (long unsigned int) SPEED_GETDISTANCE) {
		LastTimeGetDistance = millis();

		distance = ultrasonic.getDistance();
		capacity = map(distance, EMPTY + 1, FULL, 0, 100); // mapping
		capacity = (capacity <= 0 ? 0 : (capacity >= 100 ? 100 : capacity)); // filter 0 - 100
	}
}

void __monitor_data_(void) {
	if ((unsigned long) (millis() - LastTimeMonitor) >= 1000) {
		LastTimeMonitor = millis();

		Serial.print(F("Distance : "));
		Serial.print(distance);
		Serial.println(F(" cm"));

		Serial.print(F("Capacity : "));
		Serial.print(capacity);
		Serial.println(F("%"));

		Serial.println();
		Serial.print(F("Auto : "));
		Serial.println(__auto_state__ ? "Enable" : "Disable");
		Serial.print(F("Gif Feed : "));
		Serial.println(__switch_state__ ? "Enable" : "Disable");
	}
}

void sendCapacity(void) {
	if ((unsigned long) (millis() - LastTimeSendCapacity) >= 100) {
		LastTimeSendCapacity = millis();
		Blynk.virtualWrite(V0, capacity);
	}
}

BLYNK_WRITE(V1) {
	int paramBlynk = param.asInt();
	__auto_state__ = (paramBlynk == 1 ? true : false);
}

/* Feed Manual (Push Button Mode) */
BLYNK_WRITE(V2) {
	int paramBlynk = param.asInt();
	__switch_state__ = (paramBlynk == 1 ? true : false);
	// Blynk.virtualWrite(V1, 0);
}

/* Mode Blynk Disable (Push Button Mode) : Restart ESP*/
BLYNK_WRITE(V3) {
	int paramBlynk = param.asInt();
	SwitchRecovery(paramBlynk == 1 ? true : false);
	Blynk.virtualWrite(V3, 0);
	delay(5000);
	ESP.restart();
}

BLYNK_WRITE(V4) {
	int paramBlynk = param.asInt();
	ESPRestart = (paramBlynk == 1 ? true : false);
	Blynk.virtualWrite(V4, 0);
	delay(5000);
	if (ESPRestart) {
		ESP.restart();
	}
}

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	// serialData.begin(9600);
	i2cMaster.begin(SDA_PIN, SCL_PIN);

	while(!LFS.begin()) {
		Serial.println(F("Error initializing LittleFS, please try again..."));
		delay(500);
	}

	EEPROM.begin(512);
	ultrasonic.begin();
	bootBtn.begin();

	pinMode(LED_RED, OUTPUT);
	pinMode(LED_YELLOW, OUTPUT);

	programWiFi.setup(SSID_STA, PASSWORD_STA, SSID_AP, PASSWORD_AP);
	
	bool WiFiState = EEPROM.read(EEPROM_BLYNK_ADDR);
	programWiFi.initWiFi(WiFiState);
	if (WiFi.getMode() == WIFI_AP) {
		webServer.Serverinit();
	}
	else {
		Blynk.begin(BLYNK_AUTH_TOKEN, programWiFi.__SSID_STA__.c_str(), programWiFi.__PASSWORD_STA__.c_str());
		TIMER.setInterval(100L, sendCapacity);
	}
}

void loop() {
	// put your main code here, to run repeatedly:
	bootBtn.WiFiMode();
	__indikator__();
	__get_capacity__();
	__monitor_data_();

	// send data to atmega328p
	// serialData.setCapacity(capacity);
	// serialData.setBlynkCmd(__auto_state__, __switch_state__);
	// serialData.setTimeAuto("7:0:0", "12:0:0", "17:0:0");
	// serialData.runSendData(100);

	i2cMaster.setCapacity(capacity);
	i2cMaster.setBlynkCmd(__auto_state__, __switch_state__);
	i2cMaster.runSendData(100);
	
	if (WiFi.getMode() == WIFI_STA) {	
		Blynk.run();
		TIMER.run();
	}
	else {
		__auto_state__ = true;
		webServer.updateOTAloop();
	}

}