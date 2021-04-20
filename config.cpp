#include "config.h"

void initFS(void) {
  SPIFFS.begin();
}
void read_config(void)
{
  int n = 0;
  File f = SPIFFS.open("/roof.cfg", "r");
  if (f)
  { String  s = f.readStringUntil('\n');
   s.trim();
    n = s.toInt();
    Serial.println(s);
    
    maxcounter = n > 100 ? n : 14900;
    Serial.println(maxcounter);
    s = f.readStringUntil('\n');
    s.trim();
    n = s.toInt();
    maxspeed = n > 100 ? n : 1000;
    s = f.readStringUntil('\n');
    s.trim();
    n = s.toInt();
    acceleration = n > 1 ? n :1;
    s = f.readStringUntil('\n');
    s.trim();
    n = s.toInt();
    lowspeed = n > 100 ? n : 500;
    f.close();
   if (sync_home) select_speed = maxspeed;
  }
  else {
    //SPIFFS.format();
  }
}
void save_config(void)
{ File f = SPIFFS.open("/roof.cfg", "w");
  f.println(maxcounter);
  f.println(maxspeed);
  f.close ();
}
void read_net(void) {
  File  f = SPIFFS.open("/network.config", "r");
  if (f)
  { IPAddress ip;
    IPAddress gateway;
    IPAddress subnet;
    IPAddress dns;
    if (ip.fromString(f.readStringUntil('\n')) && subnet.fromString(f.readStringUntil('\n')) && gateway.fromString(f.readStringUntil('\n')) + dns.fromString(f.readStringUntil('\n'))) {
      ssid = f.readStringUntil('\n'); password = f.readStringUntil('\n');
      ssid.trim(); password.trim();
      WiFi.begin(ssid.c_str(), password.c_str());
      WiFi.config(ip, gateway, subnet, dns);
    }

    f.close();
  }
}
