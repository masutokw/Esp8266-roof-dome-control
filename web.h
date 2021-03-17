#ifndef WEBSERVER_H_INCLUDED
#define WEBSERVER_H_INCLUDED
#include <ESP8266WebServer.h>


#include "config.h"
extern String ssid;
extern String password ;
#define BUTT ".button_red {text-align: right; background-color:#100000;color:yellow;border-color:red; border-radius:5px}"
#define TEXTT  ".text_red {text-align: right;height:20px; width:95px; background-color:black;color: red;border:0}"
#define AUTO_SIZE "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
extern ESP8266WebServer serverweb;
extern String ssi;
extern String pwd;
void initwebserver(void);

#endif // WEBSERVER_H_INCLUDED//
