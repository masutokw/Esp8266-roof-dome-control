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
#define DHTTYPE DHT11 // DHT Shield uses DHT 11
#define DHTPIN D4 // DHT Shield uses pin D4
extern  int maxspeed ,maxcounter,acceleration,lowspeed,sync_home,select_speed;
extern String ssid;
extern String password ;
void initFS(void);
void read_config(void);
void save_config(void);
void read_net(void);
void hard_stop(void);
void soft_stop(int k);
void find_home(void);
#endif
