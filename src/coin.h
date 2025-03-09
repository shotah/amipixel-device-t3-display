#ifndef COIN_H
#define COIN_H

#include "lvgl.h"
#include <Arduino.h>
#include <string> // Include for std::string

// --- Define CoinMarketCap API Data Structures ---
typedef struct __CoinMarketCapApiSubsribe {
  int id;
  const lv_img_dsc_t *src_img;
  const char *name;
} CoinMarketCapApiSubsribe;

typedef struct __CoinMarketCapApiDataStream {
  int id;
  double price;
  double percent_change_1h;
  double percent_change_24h;
  double percent_change_7d;
} CoinMarketCapApiDataStream;

namespace Coin {
struct CoinApiData // Renamed struct for consistency
{
  double price_usd;
  double percent_change_24h;
  // You might want to add coin name/symbol here if needed for display
};

void setupCoin();              // Setup function for the coin module
extern CoinApiData coinApi[4]; // Declaration of the global coinApi array
                               // instance (for 4 coins, adjust size if needed)

// --- Consider removing these if CoinMarketCapApiSubsribe and
// CoinMarketCapApiDataStream are only used internally --- extern
// CoinMarketCapApiSubsribe coinSubsribe[4]; // If needed externally, keep,
// otherwise, make static in .cpp extern CoinMarketCapApiDataStream coinData[4];
// // If needed externally, keep, otherwise, make static in .cpp

} // namespace Coin

#endif // COIN_H