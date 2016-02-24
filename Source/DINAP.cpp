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

int DINAP::convertTempString(std::string temp)
{
  /* If the corect information has been returned from the XML document, 
   * temperature should be in the format "25° or 3°" for example.
   */
  if (temp.length() == 4 || temp.length() == 3) //This is factoring in the null byte
  {
    return atoi(temp.c_str());  //As we are expecting the above format, we only want to convert the front portion of the string
  }
  else
  {
    return -1;
  }
}
  

bool DINAP::scrapeWeatherData(void)
{
  bool scrapeOK = false;
    
  BomParser bomSpider = BomParser();
  
  std::string scrapedTemp = bomSpider.GetWeatherInfo(DINAP::location, INFO_Temp); //TEST CODE

  if (!scrapedTemp.empty())
  {    
    syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_INFO), scrapedTemp.c_str());
    compareUserTemp(scrapedTemp);
    scrapeOK = true;
  }
  else
  {
    syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_ERR), "Unable to obtain weather information.");
  }

  return scrapeOK;
}

void DINAP::notifyUser(const char * summary, const char * message)
{
  //Initialise the lib-notify handle
  notify_init("Do I need A Parker?");

  //Create the notification
  NotifyNotification * weatherUpdate;
  weatherUpdate = notify_notification_new(summary, message, NULL);

  //Set timeout - 3 seconds
  notify_notification_set_timeout(weatherUpdate, 3000);

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
  //Convert the temperature string into an integer  
  int temp_int = DINAP::convertTempString(scrapedTemp);
  
  //Setup the notification messages
  std::string summary = "It's " + scrapedTemp + "outside!";
  std::string message = "It's quite nice outside, take it easy.";
  
  if (temp_int >= 0)  //The conversion worked
  {
    if (temp_int >= DINAP::shorts_temp)
      message = "Crikey that's hot mate! Time to get your shorts on.";
    
    if (temp_int <= DINAP::parker_temp)
      message = "Brrr, it's bloody cold outside. Get your parker on mate.";

    DINAP::notifyUser(summary.c_str(), message.c_str()); //Send the notification
  }
  else
  {
    syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_ERR), "Unable to convert string to integer.");
  }  
}
/* END DINAP */
/*!
** @}
*/
