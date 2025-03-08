#include "display_driver.h"

namespace DisplayDriver
{

    static LilyGo_Class *amoled_ptr = nullptr; // Static pointer to LilyGo_Class

    void setupDisplayDriver(LilyGo_Class &amoled_instance)
    {
        amoled_ptr = &amoled_instance; // Store the amoled instance
        // Add any display initialization code here later if needed.
        Serial.println("Display Driver Setup Done"); // Confirmation message
    }

} // namespace DisplayDriver