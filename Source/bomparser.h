/*! @file
 *
 *  @brief Bureau of Meterology (Australia) XML parser
 *
 *  This module contains the routines that, parse out temperature and rain stats (for each state capital)
 *  in the XML document from BOM - which can be found here 'ftp://ftp2.bom.gov.au/anon/gen/fwo/IDA00101.html'.
 *
 *  @author APope
 *  @date 20-02-2016
 */
#ifndef BOM_PARSER_H
#define BOM_PARSER_H

#include <string>
#include <curl/curl.h>

class BomParser
{
private:
  /*! @brief Constructs the XPath selector for parsing the XML document.
   *  
   *  @param state - Indicates which capital city to obtain information about.
   *  @param info  - Indicates what weather information to obtain.
   *  
   *  @return - The Xpath selector as a string.
   */
  std::string constructXPathSelector(TSTATE state, TINFO info);
  
  /*! @brief Will curl the XML document from the BOM website.
   *  
   *  @param curlOK - Used to indicate to the caller whether or not the curl succeded
   *  
   *  @return - The obtained XML.
   */
  std::string curlBomXML(CURLcode *curlOK);
  
  /*! @brief Parses the XML doc for the desired information.
   *  
   *  @param state - Indicates which capital city to obtain information about.
   *  @param info  - Indicates what weather information to obtain.
   *  @param bomXML - The obtained bom XML to parse.
   *  
   *  @return - Any information if found (otherwise returned string will be empty).
   */
  std::string getInfoFromXML(TSTATE state, TINFO info, std::string bomXML);

public:
  /*! @brief Returns the Weather information obtained from the BOM website.
   *  
   *  @param state - Indicates which capital city to obtain information about.
   *  @param info  - Indicates what weather information to obtain.
   *  
   *  @return - Any information if found (otherwise returned string will be empty).
   */
  std::string GetWeatherInfo(TSTATE state, TINFO info);
};

#endif //BOM_PARSER_H
