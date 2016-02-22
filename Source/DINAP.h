/*! @file
 *
 *  @brief Do I need a Parker
 *
 *  This module contains the routines that drive the dinapd daemon.
 *
 *  @author APope
 *  @date 20-02-2016
 */
#ifndef DINAP_H
#define DINAP_H

#include <string>

typedef enum
{
  STATE_NSW,
  STATE_ACT,
  STATE_NT,
  STATE_QLD,
  STATE_SA,
  STATE_TAS,
  STATE_VIC,
  STATE_WA
} TSTATE;

class DINAP
{
private:
  TSTATE location;  /*!< Location of user within Australia. */
  int parker_temp;  /*!< Optimal Parker temperature for User. */
  int shorts_temp;  /*!< Optimal Shorts temperature for User. */

  /*! @brief Gets weather info from online source
   *
   *  @return TRUE - If data was scraped succesfully.
   */
  bool scrapeWeatherData(void);

  /*! @brief Compare optimal user temperature to scraped temperature
   *
   *  @param scrapedTemp - The scraped temperature from online source
   *
   *  @return void
   */
  void compareUserTemp(int scrapedTemp);

  /*! @brief Sends Pop-up weather notifications
   *
   *  @param summary - The message summary.
   *  @param message - The message to send.
   *
   *  @return void
   */
  void notifyUser(const char * summary, const char * message);

public:
  /*! @brief DINAP Constructor
   *
   *  @param location - One of the States of australia for weather information.
   *  @param parker_temp - User specified optimal parker temperature.
   *  @param shorts_temp - User specified optimal shorts temperature.
   */
  DINAP(TSTATE location, int parker_temp, int shorts_temp);

  /*! @brief Check weather conditions for specified location.
   *
   *  @return void
   */
  void CheckWeatherInfo(void);
};

#endif // DINAP_H
