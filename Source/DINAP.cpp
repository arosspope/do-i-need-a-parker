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

DINAP::DINAP(TSTATE location=STATE_NSW, int parker_temp=18, int shorts_temp=25)
{
  DINAP::location = location;
  DINAP::parker_temp = parker_temp;
  DINAP::shorts_temp = shorts_temp;
}

void DINAP::CheckWeatherInfo(void)
{
  bool scrapeSucceeded;
  //TODO: expand functionality
  syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_NOTICE), "Checking the Weather... ");

  //TODO: Scrape weather information using location
  scrapeSucceeded = DINAP::scrapeWeatherData();
}

bool DINAP::scrapeWeatherData(void)
{
  DINAP::notifyUser("Weather is 17 degs", "Brrr it's chilly! Time for a coat.");    //TEST MESSAGE
  return true;
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
  notify_notification_set_urgency(weatherUpdate, NOTIFY_URGENCY_CRITICAL);

  //Show the notification
  if (!notify_notification_show(weatherUpdate, NULL))
  {
    syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_ERR), "Failed to send notification.");
  }

  //Unitialise lib-notify
  notify_uninit();
}

void DINAP::compareUserTemp(int scrapedTemp)
{

}
/* END DINAP */
/*!
** @}
*/
