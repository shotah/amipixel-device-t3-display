#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <Arduino.h>
#include <LilyGo_AMOLED.h>

namespace DisplayDriver {

void setupDisplayDriver(LilyGo_Class &amoled_instance); // Pass amoled instance

} // namespace DisplayDriver

#endif // DISPLAY_DRIVER_H