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

#define MINUTE 60 //60 seconds per minute

using namespace std;

int main(int argc, char *argv[])
{
  pid_t pid, sid;
  pid = fork();         //Fork the parent process

  if (pid < 0)
    exit(EXIT_FAILURE); //Fork has failed - exit with error

  if (pid > 0)
    exit(EXIT_SUCCESS); //Fork worked - close the parent process

  umask(0);             //Change the file mask

  //Setup Logging
  setlogmask(LOG_UPTO(LOG_NOTICE));
  openlog("dinapd", LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
  syslog(LOG_INFO, "Entering Daemon");

  //Create a new signature id for our child
  sid = setsid();
  if (sid < 0)
  {
    syslog(LOG_ERR, "Unable to create SID");
    exit(EXIT_FAILURE);
  }

  //Change Directory
  if ((chdir("/")) < 0)
  {
    syslog(LOG_ERR, "Unable to change working directory");
    exit(EXIT_FAILURE);     //If we cant find the directory then we exit with error
  }

  //Close Standard File Descriptors
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  //Create a DINAP object
  DINAP userSpecs = DINAP(STATE_NSW, 18, 25);   //TODO: Read these specs from command line arguments

  //Execute the main process
  while(true)
  {
    userSpecs.GetWeatherInfo();   //Execute the process
    sleep((MINUTE * 1));          //Execute it every 1 minute (debug purposes)- TODO: Change to hourly rate
  }

  closelog();               //Close the log
  return(EXIT_SUCCESS);
}
