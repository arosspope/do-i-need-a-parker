/*! @file
 *
 *  @brief Header file containing project types
 *
 *  This module contains types shared between the DINAP modules.
 *
 *  @author APope
 *  @date 24-02-2016
 */
#ifndef TYPES_H
#define TYPES_H

#define EMPTY_STRING ""

typedef enum
{
  //Ensure numbering starts at 1, for easy XML schema parsing
  STATE_NSW = 1,    
  STATE_VIC,
  STATE_QLD,
  STATE_WA,
  STATE_SA,
  STATE_TAS,
  STATE_ACT,
  STATE_NT
} TSTATE; /*!< TSTATE lists the states of Australia */

typedef enum
{
  //Ensure numbering starts at 2, for easy XML schema parsing
  INFO_Temp = 2,
  INFO_Rain = 3
} TINFO;  /*!< TINFO lists the available weather information that can be obtained from the XML structure */

#endif //TYPES_H
