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
/*!
**  @addtogroup BOM_PARSER_module BOM_PARSER module documentation
**  @{
*/
/* MODULE BOM_PARSER */
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>
#include <curl/curl.h>

#include <iostream>
#include <string>
#include <syslog.h>
#include <stdlib.h>

#include "types.h"
#include "bomparser.h"

/*! @brief Call back function for curl.
 *
 */
static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string BomParser::constructXPathSelector(TSTATE state, TINFO info)
{
  return "//table/tbody/tr[" + std::to_string(state) + "]/td[" + std::to_string(info) + "]/text()";
}

std::string BomParser::curlBomXML(CURLcode *curlOK)
{
  CURL *curl;
  std::string bomXML;

  curl = curl_easy_init();
  
  if(curl) 
  {
    //Curl the XML document from the BOM website    
    curl_easy_setopt(curl, CURLOPT_URL, "ftp://ftp2.bom.gov.au/anon/gen/fwo/IDA00101.html");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &bomXML);
    
    //Get the Curl return code and perform cleanup
    *curlOK = curl_easy_perform(curl);  
    curl_easy_cleanup(curl);
  }
  
  return bomXML;
}

std::string BomParser::getInfoFromXML(TSTATE state, TINFO info, std::string bomXML)
{
  std::string foundInformation;
  
  // Parse HTML and create a DOM tree
  xmlDoc* doc = htmlReadDoc((xmlChar*)bomXML.c_str(), NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

  // Encapsulate raw libxml document in a libxml++ wrapper
  xmlNode* r = xmlDocGetRootElement(doc);
  xmlpp::Element* root = new xmlpp::Element(r);

  // Grab the weather inforamtion using xpath expression
  std::string xpath = constructXPathSelector(state, info);
  auto elements = root->find(xpath);
  
  //Check that elements made a match with xpath expression
  if (elements.size() > 0)
  {
	  foundInformation = dynamic_cast<xmlpp::ContentNode*>(elements[0])->get_content(); //We are only interested in the first match
  }
  else
  {
    syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_ERR), "Could not Parse XML doc.");
    foundInformation.clear();
  }

  //Perform the XML cleanup
  delete root;
  xmlFreeDoc(doc);

  return foundInformation;
}

std::string BomParser::GetWeatherInfo(TSTATE state, TINFO info)
{
  CURLcode curlRC;
  std::string bomXML = curlBomXML(&curlRC);

  if (curlRC == CURLE_OK)
  {
    return getInfoFromXML(state, info, bomXML);
  }
  else
  {
    syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_ERR), "Curl to BOM website failed");
    return ""; //In this case returning an empty string indicates that no information was available to be scraped
  }
}
/* END BOM_PARSER */
/*!
** @}
*/
