#include "Particle.h"

SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);

void startupTasks() {
        WiFi.selectAntenna(ANT_AUTO);
        RGB.control(true);
        RGB.brightness(5);
        RGB.control(false);

        System.set(SYSTEM_CONFIG_SOFTAP_PREFIX, "hello");
        System.set(SYSTEM_CONFIG_SOFTAP_SUFFIX, "world");
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

void connectionStats()
{
  WiFiSignal sig = WiFi.RSSI();
  int rssi = sig.rssi;
  float strength = sig.getStrength();
  float quality = sig.getQuality();

  Serial.printlnf("[%s]: rssi: %d, strength: %1.2f, quality: %1.2f", WiFi.SSID(), rssi, strength, quality);
}

void testByteToString()
{
  const uint8_t myBytes[] = {0xff, 0x0d, 0x03};
  char myBytesBuffer[255];
  for (unsigned i=0; i < sizeof(myBytes); i++) {
    snprintf(myBytesBuffer, sizeof(myBytesBuffer), "%s%02X", myBytesBuffer, myBytes[i]);
  }

  Serial.println(myBytesBuffer);
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

// [eduroam]: rssi: -69, strength: 62.00, quality: 45.16

void loop() // Put code here to loop forever
{
        // Don't print in listening mode
        if (WiFi.listening()) {
                return;
        }

        static uint32_t msDelay = 0;
        if (millis() - msDelay >= 3000) {

                if (WiFi.ready()) {
                  connectionStats();
                  //testByteToString();
                } else {
                    listAPs();
                }

                msDelay = millis();
        }
}
