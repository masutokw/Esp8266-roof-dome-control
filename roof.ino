#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "config.h"
#define OTA
#ifdef OTA
#include <ArduinoOTA.h>
#include "OTA_helper.hpp"
#endif
#ifdef   IR_CONTROL
#include "ir_control.h"
#endif
#include "web.h"
#include <Ticker.h>
#include "wifipass.h"
#define SPEED_CONTROL_TICKER 10
#define COUNTERS_POLL_TICKER 20
#define WEB_PORT 80
int sign(int t)
{
  if (t < 0) return -1;
  else if (t == 0 )return 0;
  else return 1;

}
int idle = 0;
int target, counter, targetspeed, mspeed;
int acceleration = 2;
int accdiv = 15;
int lowspeed = 500;
uint32_t m_speed = 100000;
int dir = 0;
int delta = 0;
int maxspeed = 1000;
int select_speed = 1000;
int min_speed = 500;
int maxcounter = 44000;
int sync_home = 0;
Ticker speed_control_tckr, counters_poll_tkr;
ESP8266WebServer serverweb(WEB_PORT);
ESP8266HTTPUpdateServer httpUpdater;

//String ssid = "MyWifi";
//String password = "Mypass";
uint32_t usToTicks(uint32_t us)
{
  return (5 * us);     // converts microseconds to tick
}
uint32_t ticksToUs(uint32_t ticks)
{
  return (ticks / clockCyclesPerMicrosecond() ); // converts from ticks back to microseconds
}

void  speed_up_down(int* m)
{
  if  (mspeed != targetspeed)
  {
    if  (abs(targetspeed - mspeed) < acceleration)
    {
      mspeed = targetspeed;
    } else {
      if (mspeed < targetspeed) mspeed = mspeed + acceleration;
      else if (mspeed > targetspeed)   mspeed = mspeed - acceleration;
    }
    setspeed(mspeed);
  }
}


void setspeed(int speed) {
  dir = sign(speed);
  digitalWrite(DIR_PIN, dir > 0);
  if (speed) {
    m_speed = 5 * 500000 / abs(speed);
    digitalWrite(ENABLE_PIN, 0);
  }
  else m_speed = 5000000;
}
void find_home(void)
{ if (digitalRead(HOME_SW)) {
    target = counter - maxcounter;
    select_speed = lowspeed;
  }
  target = counter - maxcounter;
}
void setpos(int pos)
{ if (dir == 0) target = counter = pos;
}
void get_switch(int pos)
{ counter = pos;
}
void  hard_stop()
{ target = counter;
  mspeed = targetspeed = 0; setspeed(0);
}
void  ICACHE_RAM_ATTR hard_stop_home(void)
{ target = counter = 0;
  mspeed = targetspeed = 0; setspeed(0);
  select_speed = maxspeed;
}
void  ICACHE_RAM_ATTR hard_stop_full(void)
{ target = counter = maxcounter;
  mspeed = targetspeed = 0; setspeed(0);
  select_speed = maxspeed;
}
void ICACHE_RAM_ATTR isr_step(void)
{
  noInterrupts();
  timer1_write(m_speed);
  if (dir) {
    bool l = !digitalRead(STEP_PIN);
    digitalWrite(STEP_PIN, l);
    if (l) counter += dir;
  }
  interrupts();
}
void thread_counter(int*)
{ delta = target - counter;
  if (delta > 0) targetspeed = min(select_speed, ((delta * 10) / accdiv) + 30) ;
  else if (delta < 0)  targetspeed = max(((delta * 10) / accdiv) - 30, -select_speed);
  else {
    targetspeed = mspeed = 0;
    setspeed(mspeed);
  }
}
void InitInterrupt(void)
{
  timer1_disable();
  timer1_attachInterrupt(isr_step);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  timer1_write(900);
}

void setup() {
  initFS();
  read_config();
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("ROOF", "boquerones");
  read_net();
  httpUpdater.setup(&serverweb);
  initwebserver();
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(HOME_SW, INPUT_PULLUP);
  pinMode(FULL_SW, INPUT_PULLUP);
  pinMode(IR_INPUT, INPUT_PULLUP);
  Serial.begin(115200);
  delay(500);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if  (WiFi.status() != WL_CONNECTED) WiFi.disconnect(true);
#ifdef OTA
  delay(3000);
  InitOTA();
#endif
#ifdef IR_CONTROL
  ir_init();
#endif
  InitInterrupt();
  speed_control_tckr.attach_ms(SPEED_CONTROL_TICKER, speed_up_down, &mspeed);
  counters_poll_tkr.attach_ms(COUNTERS_POLL_TICKER, thread_counter, &counter);
  attachInterrupt(HOME_SW, hard_stop_home, FALLING);
  attachInterrupt(FULL_SW, hard_stop_full, FALLING);
  setspeed(0);

  if (sync_home = !digitalRead(HOME_SW)) select_speed = maxspeed;
  else select_speed = lowspeed;
}

void loop() {
  String s;
  uint32_t n;
  if (Serial.available()) {
    s = Serial.readString();
    //  targetspeed = s.toInt();
    //  n = s.toInt(); setspeed(n);
    //  target = s.toInt();
  }
  delay(100);
  serverweb.handleClient();
  if ((idle += (dir == 0)) > 50) digitalWrite(ENABLE_PIN, idle = 1);

#ifdef OTA
  ArduinoOTA.handle();
#endif

#ifdef IR_CONTROL
  ir_read();
#endif
  // Serial.println(counter);
  // Serial.println(dir);
  //  Serial.println(mspeed);
}
