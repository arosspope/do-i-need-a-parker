/*! @file
 *
 *  @brief Do I need a Parker
 *
 *  This module contains the routines that drive the dinapd daemon.
 *
 *  @author APope
 *  @date 20-02-2016
 */
/*!
**  @addtogroup DINAP_module DINAP module documentation
**  @{
*/
/* MODULE DINAP */
#include <syslog.h>
#include <stdlib.h>
#include <unistd.h>
#include <glib.h>
#include <libnotify/notify.h>
#include "DINAP.h"
#include "bomparser.h"

DINAP::DINAP(TSTATE location=STATE_NSW, int parker_temp=18, int shorts_temp=25)
{
  DINAP::location = location;
  DINAP::parker_temp = parker_temp;
  DINAP::shorts_temp = shorts_temp;
}

void DINAP::CheckWeatherInfo(void)
{
  syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_NOTICE), "Checking the Weather... ");

  if (!(DINAP::scrapeWeatherData()))
  {
    syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_ERR), "Weather check failed.");
  }
}

bool DINAP::convertTempString(std::string temp_str, int *temp_int)
{
  /* If the corect information has been returned from the XML document, 
   * temperature should be in the format "25° or 3°" for example.
   */
  bool rc = true;   // Return code

  try
  {
    *temp_int = std::stoi(temp_str, nullptr, 10);  //Attempt to convert to a decimal integer
  } catch(const std::exception& e) {
    syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_ERR), "Stoi error: Unable to convert string.");    
    rc = false;  
  }
  
  return rc;
}
  

bool DINAP::scrapeWeatherData(void)
{
  bool rc;  //Return code
    
  BomParser bomSpider = BomParser();
  
  std::string scrapedTemp = bomSpider.GetWeatherInfo(DINAP::location, INFO_Temp); //TEST CODE

  if (!scrapedTemp.empty())
  {    
    syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_INFO), scrapedTemp.c_str());
    compareUserTemp(scrapedTemp);
    rc = true;
  }
  else
  {
    syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_ERR), "Unable to obtain weather information.");
    rc = false;
  }

  return rc;
}

void DINAP::notifyUser(std::string summary, std::string message)
{
  //Initialise the lib-notify handle
  notify_init("Do I need A Parker?");

  //Create the notification
  NotifyNotification * weatherUpdate;
  weatherUpdate = notify_notification_new(summary.c_str(), message.c_str(), NULL);

  //Set timeout - 4 seconds
  notify_notification_set_timeout(weatherUpdate, 4000);

  //Set the urgency level
  notify_notification_set_urgency(weatherUpdate, NOTIFY_URGENCY_NORMAL);

  //Show the notification
  if (!notify_notification_show(weatherUpdate, NULL))
  {
    syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_ERR), "Failed to send notification.");
  }

  //Unitialise lib-notify
  notify_uninit();
}

void DINAP::compareUserTemp(std::string scrapedTemp)
{  
  int temperature; 
  
  //Setup the notification messages
  std::string summary = "It's " + scrapedTemp + "outside!";
  std::string message = "It's quite nice outside, take it easy.";   //Standard message

  if ((DINAP::convertTempString(scrapedTemp, &temperature)))
  {
    if (temperature >= DINAP::shorts_temp)
      message = "Crikey that's hot mate! Time to get your shorts on.";
    
    if (temperature <= DINAP::parker_temp)
      message = "Brrr, it's bloody cold outside. Get your parker on mate.";

    DINAP::notifyUser(summary, message); //Send the notification
  }
}
/* END DINAP */
/*!
** @}
*/
