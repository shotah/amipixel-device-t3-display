#ifndef DATETIME_MODULE_H
#define DATETIME_MODULE_H

#include <Arduino.h>
#include <LilyGo_AMOLED.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

namespace DateTime {
void setupDateTime(
    LilyGo_Class &amoled_instance); // Function to setup the datetime module
                                    // (including message subscription)
void datetimeSyncTask(
    void *parameter); // Task to sync time (no more client/https params)

} // namespace DateTime

#endif // DATETIME_MODULE_H