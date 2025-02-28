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

#include <time.h> 
#include <stdio.h>
#include <math.h>

#include "generatedUI.h"

Inkplate display(INKPLATE_3BIT);

JsonDocument filter;
JsonDocument doc;

WiFiMulti wifiMulti;

HTTPClient http;

void setup() {

    Serial.begin(115200);
    display.begin();

    display.setTextColor(0, 7);
    display.setCursor(0, 128);
    display.setTextSize(1);
    //display.print("Welcome to Inkplate 6!");
    display.display();

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
        display.setCursor(0, 128);
        configTime(39600, 0, "pool.ntp.org");

        http.begin("https://api.transport.nsw.gov.au/v1/tp/departure_mon?outputFormat=rapidJSON&coordOutputFormat=EPSG%3A4326&mode=direct&type_dm=stop&name_dm=200060&departureMonitorMacro=true&excludedMeans=11&TfNSWDM=true&version=10.2.1.42");
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

                struct tm nowTm;
                getLocalTime(&nowTm);
                time_t now = mktime(&nowTm);

                display.clearDisplay();
                display.setTextColor(0, 7);
                display.setFont();
                display.setTextSize(1);
                
                for (JsonObject departure : departures) {
                    const char* routeName = departure["transportation"]["description"];
                    const char* routeNumber = departure["transportation"]["disassembledName"];
                    const char* estimatedDeparture = departure["departureTimeEstimated"];
                    if (estimatedDeparture == nullptr) {
                        estimatedDeparture = departure["departureTimePlanned"];
                    }

                    const time_t departureTime = utcToTime(estimatedDeparture) - 7200;
                    struct tm* departureTm = gmtime(&departureTime);

                    const time_t scheduledTime = utcToTime(departure["departureTimePlanned"]) - 7200;
                    struct tm* scheduledTm = gmtime(&scheduledTime);

                    double timeDifference = difftime(departureTime, now);
                    double timeScheduledDifference = difftime(scheduledTime, departureTime);

                    double minsToDeparture = round(timeDifference / 60) - 660;
                    double minsDeviation = round(timeScheduledDifference / 60);

                    char* timeString = (char*) malloc(sizeof(char) * 32);
                    char* timeDeviationString = (char*) malloc(sizeof(char) * 32);

                    if (minsDeviation == 0) {
                        sprintf(timeDeviationString, "on time");
                    } else if (minsDeviation < 0) {
                        sprintf(timeDeviationString, "%f min late", -minsDeviation);
                    } else {
                        sprintf(timeDeviationString, "%f min early", minsDeviation);
                    }

                    if (minsToDeparture > 60.0) {
                        if (departureTm->tm_yday > nowTm.tm_yday) {
                            strftime(timeString, 30, "on %A at %I:%M%p", departureTm);
                        }
                        strftime(timeString, 30, "at %I:%M%p", departureTm);
                    } else if (minsToDeparture < 1) {
                        sprintf(timeString, "now");
                    } else {
                        sprintf(timeString, "in %f mins", minsToDeparture);
                    }

                    char* temp = (char*) malloc(sizeof(char) * 512);
                    sprintf(temp, "[%s] %s departing %s (%s)\n", routeNumber, routeName, timeString, timeDeviationString);
                    Serial.println(temp);
                    display.print(temp);

                    free(temp);
                    free(timeString);
                    free(timeDeviationString);
                }
                mainDraw();
                display.display();
                //http.writeToStream(&USE_SERIAL);
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    delay(30000);
}

time_t utcToTime(const char* utc) {
    int y = 0, mo = 0, d = 0, h = 0, mi = 0, s = 0;

    sscanf(utc, "%d-%d-%dT%d:%d:%d", &y, &mo, &d, &h, &mi, &s);

    struct tm result = {0};
    result.tm_year = y - 1900;
    result.tm_mon = mo - 1;
    result.tm_mday = d + 1;
    result.tm_hour = h;
    result.tm_min = mi;
    result.tm_sec = s;
    result.tm_isdst = -1;

    time_t tt;
    tt = mktime(&result);

    return tt;
}

