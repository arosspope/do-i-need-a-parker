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
#include "types.h"

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
  void compareUserTemp(std::string scrapedTemp);

  /*! @brief Sends Pop-up weather notifications
   *
   *  @param summary - The message summary.
   *  @param message - The message to send.
   *
   *  @return void
   */
  void notifyUser(std::string summary, std::string message);
  
  /*! @brief Converts the temp string from the XML into an integer
   *
   *  @param temp_str - The string to convert.
   *  @param temp_int - A pointer to the variable to store the converted string to int.
   *
   *  @return TRUE - if conversion was successfull
   */
  bool convertTempString(std::string temp_str, int *temp_int);

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
