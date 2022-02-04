#include <Arduino.h>
#include "WiFi.h"

struct TCP_LINK
{
    WiFiClass *INO_WIFI; // Used to connect to Wifi
    WiFiClient INO_CLIENT;  // Used to connect to TCP client

    static String SSID;     // Network name
    static String PASSWORD; // Network Passord
    static uint16_t PORT;   // Netork Port

    void begin()
    {
        char ssid[SSID.length() + 1];
        char pass[PASSWORD.length() + 1];

        TCP_LINK::SSID.toCharArray(ssid, SSID.length() + 1);
        TCP_LINK::PASSWORD.toCharArray(pass, PASSWORD.length() + 1);

        INO_WIFI->begin(ssid, pass);
    };
};
