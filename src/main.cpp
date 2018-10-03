#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <stdlib.h>
#include "DINAP.h"
#include "types.h"

#define MINUTE 60 //60 seconds per minute

const state_collection_t statesOfAus[] = {
  {"NSW", STATE_NSW },
  {"VIC", STATE_VIC },
  {"QLD", STATE_QLD },
  {"WA",  STATE_WA  },
  {"SA",  STATE_SA  },
  {"TAS", STATE_TAS },
  {"ACT", STATE_ACT },
  {"NT",  STATE_NT  }
};

/*! @brief Verifies and parses the command line arguments sent to the daemon.
 *  
 *  @param argc     - Number of arguments that daemon was called with.
 *  @param argv     - Argument vectors.
 *  @param stateID  - A pointer to a stateID, set via user config.
 *  @param pTemp    - A pointer to optimal user parker temp, set via user config.
 *  @param sTemp    - A pointer to optimal user shorts temp, set via user config.
 *  
 *  @return - True if all arguments were parsed correctly.
 */
bool verifyArguments(int argc, char *argv[], TSTATE * const stateID, int * const pTemp, int * const sTemp)
{
  int i;
  bool rc = false;
  int bufferSize = 5;
  
  if (argc == 4)
  {
    /* Loop through the states of australia and 
     * attempt to get a match between the known 
     * states and what is in the config file.
     */
    for(i = 0; i < sizeof(statesOfAus) / sizeof(state_collection_t); i++)
    {
      if (strncmp(statesOfAus[i].stateName, argv[1], bufferSize) == 0)
      {
        syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_INFO), "Setting Location to: %s", argv[1]);      
        *stateID = statesOfAus[i].stateID;
        rc = true;
        break;
      }
    }

    //Attempt to convert the specified temps in the config file from string to int.
    try {
      *pTemp = std::stoi(argv[2], nullptr, 10); //Convert to decimal integer
    } catch(const std::exception& e) {
      syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_ERR), "Unable to parse parker temp: %s.", argv[2]);
      rc &= false;
    }

    try {
      *sTemp = std::stoi(argv[3], nullptr, 10); //Convert to decimal integer
    } catch(const std::exception& e) {
      syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_ERR), "Unable to parse shorts temp: %s.", argv[3]);
      rc &= false;
    }
  }
  
  return rc;
}

int main(int argc, char *argv[])
{
  TSTATE stateID;
  int parker_temp, shorts_temp; 
  pid_t pid, sid;
  pid = fork();         //Fork the parent process

  if (pid < 0)
    exit(EXIT_FAILURE); //Fork has failed - exit with error

  if (pid > 0)
    exit(EXIT_SUCCESS); //Fork worked - close the parent process

  umask(0);             //Change the file mask

  //Setup Logging
  openlog("dinapd", LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_DAEMON);

  //Create a new signature id for our child
  sid = setsid();
  if (sid < 0)
  {
    syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_ERR), "Unable to create SID");
    exit(EXIT_FAILURE);
  }
  
  //Drop superuser privileges in correct order
  if (setgid(getgid()) == -1) 
  {
    syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_ERR), "Unable to set group id");
    exit(EXIT_FAILURE);
  }

  if (setuid(getuid()) == -1)
  {
    syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_ERR), "Unable to set user id");
    exit(EXIT_FAILURE);
  }  
  
  //Change to common working directory
  if ((chdir("/")) < 0)
  {
    syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_ERR), "Unable to change working directory");
    exit(EXIT_FAILURE);
  }

  //Close Standard File Descriptors
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  
  //Validate command line arguments
  if(verifyArguments(argc, argv, &stateID, &parker_temp, &shorts_temp))
  {  
    //Create a DINAP object 
    DINAP userSpecs = DINAP(stateID, parker_temp, shorts_temp);

    //Execute the main process
    while(true)
    {
      userSpecs.CheckWeatherInfo();   //Execute the process
      sleep((MINUTE * 60));           //Execute it every hour
    }
  }
  else
  {
    syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_ERR), "Unable to verify DINAPD config. Terminating.");
    exit(EXIT_FAILURE);
  }

  closelog();               //Close the log
  return(EXIT_SUCCESS);
}
