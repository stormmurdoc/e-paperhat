/***************************************************************************
  ESP Skeleton
 ***************************************************************************/

/* include stuff */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

#define ST(A) #A
#define STR(A) ST(A)

#ifdef OTA_PASSWORD
    #pragma message STR(OTA_PASSWORD)
#else
    #warning "OTA_PASSWORD NOT defined"
#endif

#ifdef WIFI_SSID
    #pragma message STR(WIFI_SSID)
#else
    #warning "WIFI_SSID NOT defined"
#endif

#ifdef WIFI_PASSWORD
    #pragma message STR(WIFI_PASSWORD)
#else
    #warning "WIFI_PASSWORD NOT defined"
#endif

#ifdef MQTT_SERVER
    #pragma message STR(MQTT_SERVER)
#else
    #warning "MQTT_SERVER NOT defined"
#endif

#ifdef MQTT_PASSWORD
    #pragma message STR(MQTT_PASSWORD)
#else
    #warning "MQTT_PASSWORD NOT defined"
#endif

#ifdef MQTT_PORT
    #pragma message STR(MQTT_PORT)
#else
    #warning "MQTT_PORT NOT defined"
#endif

#ifdef ESP_HOSTNAME
    #pragma message STR(ESP_HOSTNAME)
#else
    #warning "ESP_HOSTNAME NOT defined"
#endif

void setup()
{

	Serial.begin(115200);
	Serial.println(">> Booting");
	Serial.print(">> git rev: ");
    Serial.println(GIT_REV);
	Serial.print(">> ESP_HOSTNAME: ");
    Serial.println(ESP_HOSTNAME);
    Serial.print(">> MQTT_PORT: ");
    Serial.println(MQTT_PORT);
    Serial.print(">> WIFI SSID: ");
    Serial.println(WIFI_SSID);
	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_SSID,WIFI_PASSWORD);

	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.print("Connection to ");
        Serial.print(WIFI_SSID);
        Serial.println(" failed! Rebooting...");
		delay(5000);
		ESP.restart();
	}

	// ArduinoOTA.setPort(8266);
	ArduinoOTA.setHostname(ESP_HOSTNAME);
	ArduinoOTA.setPassword(OTA_PASSWORD);
	ArduinoOTA.onStart([]()
	{
		String type;
		if (ArduinoOTA.getCommand() == U_FLASH) {
			type = "sketch";
		}
		else {
		    type = "filesystem"; // U_FS
		}
		// NOTE: if updating FS this would be the place to unmount FS using FS.end()
		Serial.println("Start updating " + type);
	});

	ArduinoOTA.onEnd([]() {
		Serial.println("\nEnd");
	});

	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	});

	ArduinoOTA.onError([](ota_error_t error)
	{
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) {
			Serial.println("Auth Failed");
		}
		else if (error == OTA_BEGIN_ERROR) {
			Serial.println("Begin Failed");
		}
		else if (error == OTA_CONNECT_ERROR) {
			Serial.println("Connect Failed");
		}
		else if (error == OTA_RECEIVE_ERROR)
		{
			Serial.println("Receive Failed");
		}
		else if (error == OTA_END_ERROR) {
			Serial.println("End Failed");
		} });

	ArduinoOTA.begin();
	Serial.println(">> Ready");
	Serial.print(">> IP address: ");
	Serial.println(WiFi.localIP());

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
	ArduinoOTA.handle();                // handle over the air updates
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED on
    delay(1000);                        // wait for half a second
    digitalWrite(LED_BUILTIN, LOW);     // turn the LED off
    delay(1000);                        // wait for half a second
}