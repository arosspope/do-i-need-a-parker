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

  //Create a DINAP object
  DINAP userSpecs = DINAP(STATE_NSW, 18, 25);   //TODO: Read these specs from command line arguments

  //Execute the main process
  while(true)
  {
    userSpecs.CheckWeatherInfo();   //Execute the process
    sleep((MINUTE * 1));            //Execute it every 1 minute (debug purposes)- TODO: Change to hourly rate
  }

  closelog();               //Close the log
  return(EXIT_SUCCESS);
}
