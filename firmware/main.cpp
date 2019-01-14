#include "Particle.h"

SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);

void startupTasks() {
        WiFi.selectAntenna(ANT_AUTO);
        RGB.control(true);
        RGB.brightness(5);
        RGB.control(false);
}
STARTUP(startupTasks());

void listAPs()
{
        WiFiAccessPoint aps[20];
        int found = WiFi.scan(aps, 20);
        for (int i=0; i<found; i++) {
                WiFiAccessPoint& ap = aps[i];
                Serial.printlnf("SSID: %s, Security: %d, Channel: %d, RSSI: %d", ap.ssid, ap.security, ap.channel, ap.rssi);
        }

        Serial.println();
}

int toggleLed(const char* args) {
        pinMode(D7, OUTPUT);
        int command = atoi(args);
        if (command == 1) {
                digitalWrite(D7, HIGH);
        } else {
                digitalWrite(D7, LOW);
        }
        return command;
}


void setup() // Put setup code here to run once
{
        Serial.begin(115200);
        WiFi.on();

        Particle.function("toggle", toggleLed);

        Particle.connect();
}

void loop() // Put code here to loop forever
{
        // Don't print in listening mode
        if (WiFi.listening() == false) {
                static uint32_t msDelay = 0;
                if (millis() - msDelay >= 5000) {
                        listAPs();
                        msDelay = millis();
                }
        }

}
