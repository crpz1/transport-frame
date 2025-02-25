/**
 * reuseConnection.ino
 *
 *  Created on: 22.11.2015
 *
 */

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>

#include <HTTPClient.h>

#include "Inkplate.h"
#include <ArduinoJson.h>
#include <TimeLib.h>

Inkplate display(INKPLATE_3BIT);

JsonDocument filter;
JsonDocument doc;

WiFiMulti wifiMulti;

HTTPClient http;

void setup() {

    Serial.begin(115200);

    Serial.println();
    Serial.println();
    Serial.println();

    filter["locations"][0]["name"] = true;

    JsonObject filter_stopEvents_0 = filter["stopEvents"].add<JsonObject>();
    filter_stopEvents_0["isRealtimeControlled"] = true;
    filter_stopEvents_0["location"]["properties"] = true;
    filter_stopEvents_0["departureTimePlanned"] = true;
    filter_stopEvents_0["departureTimeEstimated"] = true;

    JsonObject filter_stopEvents_0_transportation = filter_stopEvents_0["transportation"].to<JsonObject>();
    filter_stopEvents_0_transportation["disassembledName"] = true;
    filter_stopEvents_0_transportation["number"] = true;
    filter_stopEvents_0_transportation["iconId"] = true;
    filter_stopEvents_0_transportation["description"] = true;

    for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    wifiMulti.addAP("ssid", "presharedkey");

    // allow reuse (if server supports it)
    http.useHTTP10(true);
    http.setAuthorization("apikey <keyhere>");
}

void loop() {
    // wait for WiFi connection
    if((wifiMulti.run() == WL_CONNECTED)) {

        http.begin("https://api.transport.nsw.gov.au/v1/tp/departure_mon?outputFormat=rapidJSON&coordOutputFormat=EPSG%3A4326&mode=direct&type_dm=stop&name_dm=10111010&departureMonitorMacro=true&excludedMeans=11&TfNSWDM=true&version=10.2.1.42");
        //http.begin("192.168.1.12", 80, "/test.html");

        int httpCode = http.GET();
        if(httpCode > 0) {
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));

                if (error) {
                    Serial.print("deserializeJson() failed: ");
                    Serial.println(error.c_str());
                    return;
                }
                
                JsonArray departures = doc["stopEvents"];
                const size_t numberOfDepartures = departures.size();
                Serial.printf("Decoded departures: %d", numberOfDepartures);
                for (JsonObject departure : departures) {
                    const char* routeName = departure["transportation"]["description"];
                    const char* routeNumber = departure["transportation"]["number"];
                    const char* estimatedDeparture = departure["departureTimeEstimated"];
                    if (estimatedDeparture == nullptr) {
                        estimatedDeparture = departure["departureTimePlanned"];
                    }
                    Serial.printf("[%s] %s departing at %s\n", routeNumber, routeName, estimatedDeparture);
                }
                //http.writeToStream(&USE_SERIAL);
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    delay(30000);
}



