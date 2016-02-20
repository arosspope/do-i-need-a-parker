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
#include <string>
#include "DINAP.h"

DINAP::DINAP(TSTATE location=STATE_NSW, int parker_temp=18, int shorts_temp=25)
{
  DINAP::location = location;
  DINAP::parker_temp = parker_temp;
  DINAP::shorts_temp = shorts_temp;
}

void DINAP::GetWeatherInfo(void)
{
  //TODO: expand functionality
  syslog(LOG_NOTICE, "Writing to my Syslog");
}
/* END DINAP */
/*!
** @}
*/
