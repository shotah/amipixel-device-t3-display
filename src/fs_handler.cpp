#include "fs_handler.h" // Include the header file for this module
#include <Arduino.h>    // For Serial printing

namespace FSHandler
{

    bool setupFS()
    {
        if (!SPIFFS.begin(true))
        {
            Serial.println("[FSHandler] SPIFFS Mount Failed");
            return false; // Indicate failure
        }
        else
        {
            Serial.println("[FSHandler] SPIFFS Mount Success");
            return true; // Indicate success
        }
    }

    void listFSContents()
    {
        Serial.println("[FSHandler] --- SPIFFS Root Directory Contents: ---");
        File root = SPIFFS.open("/");
        if (!root)
        {
            Serial.println("[FSHandler] - Failed to open root directory");
        }
        else
        {
            File file = root.openNextFile();
            while (file)
            {
                Serial.print("[FSHandler] - File: ");
                Serial.println(file.name());
                file = root.openNextFile();
            }
            Serial.println("[FSHandler] --- End of SPIFFS Root Directory Contents ---");
        }
    }

} // namespace FSHandler