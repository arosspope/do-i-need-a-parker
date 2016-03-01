# Do I Need A Parker

This weather service periodically checks temperature in your location (currently limited to the state capitals of Australia), based on inforamtion gained from the Bureau of Meteorology `www.bom.gov.au`. Depending on user temperature specifications - the daemon will send push notifiactions to the desktop indicating if warmer (or cooler) clothing may be required.

Pop-up notifications can be GTK+ or Qt based, utilising the c/c++ 'libnotify' development library. This project is currently being developed on a RHEL based system (CentOS 7). More information regarding project requirements can be found in the TODO file.

# Dependencies

> yum install libnotify-devel

> yum install libxml2-devel

> yum install libcurl-devel

> yum install libxml++-devel

# Building the Project

When building the project it is vital that the 'dinapd.exe' is created in the root directory of the project as that is where the daemon scripts are configured to look.
I use the below compiler options to build the project:

> g++ -I/usr/include/gdk-pixbuf-2.0 -lcurl \`pkg-config --cflags glib-2.0 gtk+-2.0 libxml++-2.6\` [insert source files here] -g -pg \`pkg-config --libs glib-2.0 gtk+-2.0 libnotify libxml+-2.6\` \`xml2-config --cflags --libs\` --std=c++0x -o [path to root dir of proj]/dinapd

# Controlling the Service

Controlling the 'Do I need a Parker?' weather service is facilitated through the `dinapd_init.sh` script located in the scripts folder.

Starting the service:
>$./dinapd_init.sh start

Stopping the service:
>$./dinapd_init.sh stop

Restarting the service:
>$./dinapd_init.sh restart

Obtaining the status of the service:
>$./dinapd_init.sh status

>dinapd (pid 2910) is running...

Logging and debugging information can be found in `/var/log/messages`

# Why another Weather Service?

It was my intention to create the 'Do I Need A Parker' project as a method in learning how to create daemons in the context of a Linux based OS, and using XPATH to parse XML documents.

