#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "web.h"
#include <Ticker.h>
#include "config.h"
#include "wifipass.h"
#define SPEED_CONTROL_TICKER 10
#define COUNTERS_POLL_TICKER 20
#define STEP_PIN 4
#define DIR_PIN 5
#define WEB_PORT 80
int sign(int t)
{
  if (t < 0) return -1;
  else if (t == 0 )return 0;
  else return 1;

}
int target, counter, targetspeed, mspeed;
int acceleration = 2;
uint32_t m_speed = 100000;
int dir = 0;
int delta = 0;
int maxspeed = 1000;
int maxcounter = 14000;
Ticker speed_control_tckr, counters_poll_tkr;
ESP8266WebServer serverweb(WEB_PORT);
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
  if (speed) m_speed = 5 * 500000 / abs(speed); else m_speed = 5000000;
}
void ICACHE_RAM_ATTR isr_step(void)
{
  noInterrupts();
  timer1_write(m_speed);
  if (dir) {
    bool l = !digitalRead(4);
    digitalWrite(STEP_PIN, l);
    if (l) counter += dir;
  }
  interrupts();
}
void thread_counter(int*)
{ delta = target - counter;
  if (delta > 0) targetspeed = min(maxspeed, delta / 3 + 30) ;
  else if (delta < 0)  targetspeed = max(delta / 3 - 30, -maxspeed); else {
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

  /*IPAddress ip(192, 168, 1, 32);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 254, 0);
    IPAddress DNS(192, 168, 1, 1);
    WiFi.config(ip, gateway, subnet, gateway);*/

  initwebserver();
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  Serial.begin(115200);
  InitInterrupt();
  speed_control_tckr.attach_ms(SPEED_CONTROL_TICKER, speed_up_down, &mspeed);
  counters_poll_tkr.attach_ms(COUNTERS_POLL_TICKER, thread_counter, &counter);
  setspeed(0);
}

void loop() {
  String s;
  uint32_t n;
  if (Serial.available()) {
    s = Serial.readString();
    //  targetspeed=s.toInt();
    // n=s.toInt();setspeed(n);
    target = s.toInt();
  }
  delay(100);  serverweb.handleClient();
  Serial.println(counter);
  Serial.println(dir);
  Serial.println(mspeed);
}
