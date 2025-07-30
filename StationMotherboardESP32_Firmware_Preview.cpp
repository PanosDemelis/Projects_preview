
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <math.h>
#include "../AgroMeteo.h"

#if RF_PROTOCOL == LORA_PROTOCOL
RTC_DATA_ATTR int charge = 0;
RTC_DATA_ATTR int message = 0;
RTC_DATA_ATTR bool networkJoinedStatus = false;

String DEPLOYMENT_ID = "deploymentID";

long oldTime = millis();
long newTime;
long uplinkInterval = 20000; // ms

const float offset = -259.0;

char rxbuff[PAYLOAD_SIZE];
uint8_t rxbuffIndex = 0;

int c[3] = {0, 0, 0};

uint8_t watchdog_reset = 0;
int packetOffset = 0;
char sensor_data_buff[PAYLOAD_SIZE] = "\0";

int16_t case_temp_uint16 = 0.0;
int16_t case_humidity_uint16 = 0.0;
int16_t air_temp_uint16 = 0.0;
int16_t air_humidity_uint16 = 0.0;
int16_t solar_radiation_uint16 = 0.0;
int16_t windDirection_unint_16 = 0.0;
int16_t windSpeed_unint_16 = 0.0;
int16_t rainmm_unint_16 = 0.0;
int16_t soil_temperature_uint16 = 0.0;
int16_t soil_moisture_uint16 = 0.0;
int16_t leaf_wetness_uint16 = 0.0;
int16_t voltage_uint16 = 0.0;

void cleanBuffer() {
	for (int i = 0; i < PAYLOAD_SIZE + DEPLOYMENT_ID.length(); i++)
		sensor_data_buff[i] = '\0';
}

bool wasWatchdogReset() {
	return (esp_reset_reason() == ESP_RST_TASK_WDT || esp_reset_reason() == ESP_RST_WDT);
}

void readSensors(void) {
	#if defined(RS485)
		pinMode(MDir,OUTPUT);
		digitalWrite(MDir,HIGH);
		delay(500);
	#endif

	#if defined(SHT31)
		checkSensor(readSHT);
		
		#if defined(CASE)
		case_temp_uint16 = float_to_uint16(caseTemperature, -20.0, 80.0);
		case_humidity_uint16 = float_to_uint16(caseHumidity, 0.0, 100.0);
		#else
		case_temp_uint16 = BYPASS_SENSOR_CODE;
		case_humidity_uint16 = BYPASS_SENSOR_CODE;
		#endif

		#if defined(OUTDOOR)
		air_temp_uint16 = float_to_uint16(airTemperature, -20.0, 80.0);
		air_humidity_uint16 = float_to_uint16(airHumidity, 0.0, 100.0);
		#else
		air_temp_uint16 = BYPASS_SENSOR_CODE;
		air_humidity_uint16 = BYPASS_SENSOR_CODE;
		#endif
	#else
		air_temp_uint16 = BYPASS_SENSOR_CODE;
		air_humidity_uint16 = BYPASS_SENSOR_CODE;

		case_temp_uint16 = BYPASS_SENSOR_CODE;
		case_humidity_uint16 = BYPASS_SENSOR_CODE;
	#endif
	esp_task_wdt_reset();

	#if defined(SOLARRADIATION)
	readSolarRadiation();
	solar_radiation_uint16 = float_to_uint16(solarRadiation, 0.0, 2000.0);
	#else
	solar_radiation_uint16 = BYPASS_SENSOR_CODE;
	#endif
	esp_task_wdt_reset();

	#if defined(CURRWINDDIRECTION)
	calculateVane();
	windDirection_unint_16 = float_to_uint16(windDirection, 0.0, 360.0);
	#else
	windDirection_unint_16 = BYPASS_SENSOR_CODE;
	#endif
	esp_task_wdt_reset();

	#if defined(WINDSPEEDKMH)
	#if defined(DIGITAL_WINDSPEEDKMH)
	readAnemometer();
	windSpeed_unint_16 = float_to_uint16(windSpeed, 0.0,800.0);
	#elif defined(RS485_WINDSPEEDKMH)
	windSpeed_unint_16 = float_to_uint16(readWindSpeed(WINDSPEED_DEV_ADDR), 0.0, 60.0);
	#else
	windSpeed_unint_16 = BYPASS_SENSOR_CODE;
	#endif
	#endif
	esp_task_wdt_reset();

	#if defined(RAINMM)
	readPluviometer();
	rainmm_unint_16 = float_to_uint16(rainmm, 0.0, 800.0);
	#else
	rainmm_unint_16 = BYPASS_SENSOR_CODE;
	#endif
	esp_task_wdt_reset();

	#if defined(SOILTEMP)
	checkSensor(readDSoilTemperature);
	soil_temperature_uint16 = float_to_uint16(soilTemperature, -20.0, 80.0);
	#else
	soil_temperature_uint16 = BYPASS_SENSOR_CODE;
	#endif
	esp_task_wdt_reset();

	#if defined(WATERMARK)
	readWatermark();
	soil_moisture_uint16 = float_to_uint16(watermark, 0.0, 200.0);
	#else
	soil_moisture_uint16 = BYPASS_SENSOR_CODE;
	#endif
	esp_task_wdt_reset();

	#if defined(LEAFWET)
		#if defined(ANALOG_LEAFWET)
			readLeafWetness();
			#endif
			
			#if defined(RS485_LEAFWET)
			readRS485LeafWetness();
			Serial2.println(leafWetness);
		#endif
		leaf_wetness_uint16 = float_to_uint16(leafWetness, 0.0, 100.0);
	#else
		leaf_wetness_uint16 = BYPASS_SENSOR_CODE;
	#endif
	esp_task_wdt_reset();

	voltage_uint16 = float_to_uint16(voltage, 0.0, 16.0);
}

void createLoRaPacket(void) {
	packetOffset = snprintf(sensor_data_buff, PAYLOAD_SIZE + DEPLOYMENT_ID.length(), "AT+SENDB=0,2,%d,", LORA_PAYLOAD_SIZE + DEPLOYMENT_ID.length());

	packetOffset += snprintf(sensor_data_buff + packetOffset, PAYLOAD_SIZE + DEPLOYMENT_ID.length() - packetOffset, "%02X%02X",
			(voltage_uint16 >> 8) & 0xFF, voltage_uint16 & 0xFF);
	packetOffset += snprintf(sensor_data_buff + packetOffset, PAYLOAD_SIZE + DEPLOYMENT_ID.length() - packetOffset, "%02X%02X%02X%02X",
			(air_temp_uint16 >> 8) & 0xFF, air_temp_uint16 & 0xFF,
			(air_humidity_uint16 >> 8) & 0xFF, air_humidity_uint16 & 0xFF);
	packetOffset += snprintf(sensor_data_buff + packetOffset, PAYLOAD_SIZE + DEPLOYMENT_ID.length() - packetOffset, "%02X%02X",
			(windDirection_unint_16 >> 8) & 0xFF, windDirection_unint_16 & 0xFF);
	packetOffset += snprintf(sensor_data_buff + packetOffset, PAYLOAD_SIZE + DEPLOYMENT_ID.length() - packetOffset, "%02X%02X",
			(windSpeed_unint_16 >> 8) & 0xFF, windSpeed_unint_16 & 0xFF);
	packetOffset += snprintf(sensor_data_buff + packetOffset, PAYLOAD_SIZE + DEPLOYMENT_ID.length() - packetOffset, "%02X%02X",
			(rainmm_unint_16 >> 8) & 0xFF, rainmm_unint_16 & 0xFF);
	packetOffset += snprintf(sensor_data_buff + packetOffset, PAYLOAD_SIZE + DEPLOYMENT_ID.length() - packetOffset, "%02X%02X%02X%02X",
			(case_temp_uint16 >> 8) & 0xFF, case_temp_uint16 & 0xFF,
			(case_humidity_uint16 >> 8) & 0xFF, case_humidity_uint16 & 0xFF);
	packetOffset += snprintf(sensor_data_buff + packetOffset, PAYLOAD_SIZE + DEPLOYMENT_ID.length() - packetOffset, "%02X%02X",
			(soil_moisture_uint16 >> 8) & 0xFF, soil_moisture_uint16 & 0xFF);
	packetOffset += snprintf(sensor_data_buff + packetOffset, PAYLOAD_SIZE + DEPLOYMENT_ID.length() - packetOffset, "%02X%02X",
			(leaf_wetness_uint16 >> 8) & 0xFF, leaf_wetness_uint16 & 0xFF);
	packetOffset += snprintf(sensor_data_buff + packetOffset, PAYLOAD_SIZE + DEPLOYMENT_ID.length() - packetOffset, "%02X%02X",
			(soil_temperature_uint16 >> 8) & 0xFF, soil_temperature_uint16 & 0xFF);
	packetOffset += snprintf(sensor_data_buff + packetOffset, PAYLOAD_SIZE + DEPLOYMENT_ID.length() - packetOffset, "%02X%02X",
			(solar_radiation_uint16 >> 8) & 0xFF, solar_radiation_uint16 & 0xFF);
	packetOffset += snprintf(sensor_data_buff + packetOffset, PAYLOAD_SIZE + DEPLOYMENT_ID.length() - packetOffset, "%02X", watchdog_reset);
	
	for (int counter = 0; DEPLOYMENT_ID[counter] != '\0'; counter++) {
		packetOffset += snprintf(sensor_data_buff + packetOffset, PAYLOAD_SIZE + DEPLOYMENT_ID.length() - packetOffset, "%02X", DEPLOYMENT_ID[counter]);
	}
	
	esp_task_wdt_reset();
}

void TransmitDatawithLoRaWAN(void) {
	bool status;
	uint8_t retries = 0;

	cleanBuffer();
	packetOffset = 0;

	measureVoltage();

	if (charge == 0) {
		if (message == 1) {
			delay(100);
			
			message = 0;
		}

		readSensors();

		if (wasWatchdogReset()) {
			watchdog_reset = 1;
		}

		while (1) {
			newTime = millis();
			esp_task_wdt_reset();

			if ((newTime - oldTime >= uplinkInterval) && networkJoinedStatus) {
				oldTime = newTime;

				createLoRaPacket();

				Serial2.println(sensor_data_buff);
				delay(100);

				esp_task_wdt_reset();

				break;
			}

			while (Serial2.available()) {
				char inChar = (char)Serial2.read();

				rxbuff[rxbuffIndex++] = inChar;

				if (rxbuffIndex >= PAYLOAD_SIZE - 1)
					break;

				if (inChar == '\n' || inChar == '\r') {
					rxbuff[rxbuffIndex] = '\0';

					if (strncmp(rxbuff, "JOINED", strlen("JOINED")) == 0)
						networkJoinedStatus = true;

					if (strncmp(rxbuff, "Dragino LA66 Device", strlen("Dragino LA66 Device")) == 0)
						networkJoinedStatus = false;
					rxbuffIndex = 0;
				}
				esp_task_wdt_reset();
			}
		}
		esp_task_wdt_reset();

		#if defined(RS485)
		digitalWrite(MDir,LOW);
		delay(10);
		#endif

		turnOffPattern();

		watchdog_reset = 0;
		esp_task_wdt_reset();
	} else if (charge == 1) {
		if (message == 0) {
			delay(50);

			message = 1;
		}

		pinMode(RESETn, OUTPUT);
		digitalWrite(RESETn, LOW);
		delay(150);
		digitalWrite(RESETn, HIGH);
		delay(200);

		digitalWrite(LED, HIGH);
		delay(100);
		digitalWrite(LED, LOW);
	}
}
#elif RF_PROTOCOL == XBEE_PROTOCOL
#include "xbeeTransmission.h"
#endif
