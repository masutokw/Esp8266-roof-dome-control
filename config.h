#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED
#include <FS.h>
#include <ESP8266WiFi.h>
extern int maxspeed ,maxcounter;
extern String ssid;
extern String password ;
void initFS(void);
void read_config(void);
void save_config(void);
void read_net(void);
#endif
