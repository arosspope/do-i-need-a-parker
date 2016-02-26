#!/bin/bash
#
# Daemon Name: dinapd
#
# chkconfig - 58 74
# description: Do I need a Parker (weather daemon)
# requires: mysqld

# Source function library
. /etc/init.d/functions

# DINAP config file (should be with the script)
. dinap_config

prog=dinapd
lockfile=/tmp/$prog.lock
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )" #The current working directory of the init script

start() {
  # Make some checks for requirements before continuing
  [ -x ../$prog ] || exit 5      #The Daemon executable should be within the directory above

  # Enforce that only one instance of the daemon is running
  if ! mkdir $lockfile 2>/dev/null; then
    echo "$prog is already running."
    exit 1
  fi  
  
  # Start our daemon
  echo -n $"Starting $prog: "
  
  #Create my process and create the PID file
  daemon --pidfile /var/run/${proc}.pid ../$prog
  RETVAL=$?
  echo
  
  return $RETVAL
}

stop() {
  echo -n $"Shutting down $prog: "
  
  #Kill the process and remove the prog file  
  killproc $prog
  RETVAL=$?
  echo

  # If all is well remove the lockfile
  [ $RETVAL -eq 0 ] && rm -r $lockfile 2>/dev/null
  return $RETVAL
}

# See how we were called.
case "$1" in
  start)
    start
    ;;
  stop)
    stop
    ;;
  status)
    status $prog
    ;;
  restart)
    stop
    start
    ;;
  *)
    echo $"Usage: $0 {start|stop|status|restart}"
    exit 2
esac


