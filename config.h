#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED
#include <FS.h>
#include <ESP8266WiFi.h>
#define IR_CONTROL
#define STEP_PIN D2
#define DIR_PIN D1
#define ENABLE_PIN D5
#define HOME_SW D6
#define FULL_SW D7
#define IR_INPUT D3
extern  int maxspeed ,maxcounter,acceleration,lowspeed;
extern String ssid;
extern String password ;
void initFS(void);
void read_config(void);
void save_config(void);
void read_net(void);
void hard_stop(void);
void find_home(void);
#endif
