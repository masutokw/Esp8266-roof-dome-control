#include "web.h"
#include "config.h"
#include <FS.h>
#ifdef DHTTYPE
#include <DHT.h>
DHT dht(DHTPIN, DHTTYPE);
float humidity, temperature;
#endif
extern   int target, counter, maxcounter, maxspeed, acceleration, mspeed, lowspeed;
extern uint32_t truecode;
void handleconfig(void)
{
#ifdef DHTTYPE

if (mspeed==0)  {temperature = dht.readTemperature();
  humidity = dht.readHumidity();}

#endif
  String content =  "<html><head> <meta http-equiv='refresh' content='2'><style>" + String(BUTT) + String(TEXTT) + "</style>" + String(AUTO_SIZE) + " </head><body  bgcolor=\"#000000\" text=\"#FF6000\"><h2>Techo</h2>";
  content += "<fieldset style=\"width:15%;border-radius:15px\"><legend>Estado</legend>";
  content += "<h3>Counter : " + String(counter) + "<br>Speed: " + String(mspeed) + "stp/s</h3>";
#ifdef DHTTYPE
  content += "<h3>Temp: " + String(temperature) + "&deg; C <br>Hum: " + String(humidity) + " % </h3>";
#endif
  content += "Status: " + String(counter > 0 ? "Open" : "Close") + "<br>";
  content += "Home : " + String(digitalRead(HOME_SW)  ? "OFF" : "ON") + "<br>";
  content += "End : " + String(digitalRead(FULL_SW)  ? "OFF" : "ON") + "<br>";
  content += "IR code : " + String(truecode) + "<br><table style='width:200px'>";
  content += "<button onclick=\"location.href='/move_to?DEST=" + String(maxcounter) + "'\" class=\"button_red\" type=\"button\">Open</button>";
  content += "<button onclick=\"location.href='/move_to?DEST=0'\" class=\"button_red\" type=\"button\">Close</button>";
  content += "<button onclick=\"location.href='/dome'\" class=\"button_red\" type=\"button\">Dome</button>";
  content += "<button onclick=\"location.href='/stop?SOFT=15'\" class=\"button_red\" type=\"button\">stop</button>";
  content += "<button onclick=\"location.href='/stop'\" class=\"button_red\" type=\"button\">PANIC!</button>";
  content += "<button onclick=\"location.href='/home'\" class=\"button_red\" type=\"button\">Find Home</button></table></fieldset>";
  content += "<fieldset style=\"width:15%;border-radius:15px\"><legend>Tools</legend>";
  content += "<button onclick=\"location.href='/config'\" class=\"button_red\" type=\"button\">Config</button>";
  content += "<button onclick=\"location.href='/netcfg'\" class=\"button_red\" type=\"button\">Network</button>";
  content += "<button onclick=\"location.href='/update'\" class=\"button_red\" type=\"button\">Firmware</button>";
  content += "<button onclick=\"location.href='/restart'\" class=\"button_red\" type=\"button\">Restart</button><br></fieldset>";
  content += "</body></html>";
  serverweb.send(200, "text/html", content);
}
void handleHome(void)
{
  find_home();
  String content =  "<html><head> <meta http-equiv='refresh' content=\"1;url=/\"><style>" + String(BUTT) + String(TEXTT) + "</style>" + String(AUTO_SIZE) + "</head><body  bgcolor=\"#000000\" text=\"#FF6000\"><h2>Apertura</h2><br>";
  content += "Status : " + String(target) + "<br>";
  content += "<button onclick=\"location.href='/'\" class=\"button_red\" type=\"button\">close</button><br><br>";
  content += "</body></html>";

  serverweb.send(200, "text/html", content);
}
void handleStop(void)
{ if (serverweb.hasArg("SOFT")) {
    String net = serverweb.arg("SOFT");
    int k = net.toInt();
    soft_stop(k);
  } else
    hard_stop();

  String content =  "<html><head> <meta http-equiv='refresh' content=\"1;url=/\"><style>" + String(BUTT) + String(TEXTT) + "</style>" + String(AUTO_SIZE) + "</head><body  bgcolor=\"#000000\" text=\"#FF6000\"><h2>Apertura</h2><br>";
  content += "Estado : " + String(target) + "<br>";
  content += "<button onclick=\"location.href='/'\" class=\"button_red\" type=\"button\">close</button><br><br>";
  content += "</body></html>";

  serverweb.send(200, "text/html", content);
}
void handleMove(void)
{ if (serverweb.hasArg("DEST")) {
    String net = serverweb.arg("DEST");
    target = net.toInt();
  }
  String content =  "<html><head> <meta http-equiv='refresh' content=\"1;url=/\"><style>" + String(BUTT) + String(TEXTT) + "</style>" + String(AUTO_SIZE) + "</head><body  bgcolor=\"#000000\" text=\"#FF6000\"><h2>Techo cierre</h2><br>";
  content += "Estado : " + String(target) + "<br>";
  content += "<button onclick=\"location.href='/open'\" class=\"button_red\" type=\"button\">open</button><br>";
  content += "</body></html>";
  serverweb.send(200, "text/html", content);
}
void handleDome(void) {
  if (serverweb.hasArg("DOME")) {
    String net = serverweb.arg("DOME");
    target = net.toInt();
  }
  String content =  "<html><head><style>" + String(BUTT) + String(TEXTT) + "</style>" + String(AUTO_SIZE) + "</head><body  bgcolor=\"#000000\" text=\"#FF6000\"><h2>Cupula</h2><br>";
  content += "Estado : " + String(target) + "<br>" + "<form action='/dome' method='POST'>";

  content += "<td><input type='number' step='0.01' name='DOME' class=\"text_red\" value='" + String(target) + "'></td></tr>";
  content += "<input type='submit' name='SUBMIT'  class=\"button_red\" value='Save'></form>"  "<br>";
  content += "<button onclick=\"location.href='/'\" class=\"button_red\" type=\"button\">Home</button><br>";
  content += "</body></html>";
  serverweb.send(200, "text/html", content);
}
String getContentType(String filename)
{
  if (serverweb.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path)
{

  if (path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
  {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = serverweb.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}
void handleNetwork( void) {
  String msg, ip, mask, gate, dns;
  if (serverweb.hasArg("IP") && serverweb.hasArg("MASK") && serverweb.hasArg("GATEWAY") && serverweb.hasArg("DNS") && serverweb.hasArg("SSID") && serverweb.hasArg("PASSWD") )
  {
    String net = serverweb.arg("IP") + "\n" + serverweb.arg("MASK") + "\n" + serverweb.arg("GATEWAY") + "\n" +
                 serverweb.arg("DNS") + "\n" + serverweb.arg("SSID") + "\n" + serverweb.arg("PASSWD") + "\n";
    File f = SPIFFS.open("/network.config", "w");
    if (!f)
    {
      net = ("file open failed");
    }
    else
      f.println(net);
    f.close ();
    msg = serverweb.arg("IP");
    msg += "\n" + serverweb.arg("MASK");
    msg += "\n" + serverweb.arg("GATEWAY");
    msg += "\n" + serverweb.arg("DNS") + "\n";
    msg += "\n" + serverweb.arg("SSID") + "\n";
    msg += "\n" + serverweb.arg("PASSWD") + "\n";
  }
  String content = "<html><style>" + String(BUTT) + String(TEXTT) + "</style>" + String(AUTO_SIZE) + "<body  bgcolor=\"#000000\" text=\"#FF6000\"><form action='/netcfg' method='POST'><h2>Network Config</h2>";
  content += "<fieldset style=\"width:15%;border-radius:15px\"><legend>Network</legend><table style='width:200px'>";
  content += "<tr><td>SSID</td><td><input type='text' name='SSID' class=\"text_red\" value='" + ssid + "'></td></td>";
  content += "<td><td>PASS</td><td><input type='password' name='PASSWD'class=\"text_red\" value='" + password + "'></td></tr>";
  content += "<tr><td>IP</td><td><input type='text' name='IP' class=\"text_red\" value='" + WiFi.localIP().toString() + "'></td></td>";
  content += "<td><td>MASK</td><td><input type='text' name='MASK'class=\"text_red\"  value='" + WiFi.subnetMask().toString() + "'></td></tr>";
  content += "<tr><td>Gateway</td><td><input type='text' name='GATEWAY' class=\"text_red\" value='" + WiFi.gatewayIP().toString() + "'></td></td>";
  content += "<td><td>DNS</td><td><input type='text' name='DNS' class=\"text_red\"  value='" + WiFi.gatewayIP().toString() + "'></td></tr></table>";
  content += "<input type='submit' name='SUBMIT'  class=\"button_red\" value='Save'></fieldset></form>" + msg + "<br>";
  content += "<button onclick=\"location.href='/'\"class=\"button_red\" type=\"button\">Home</button><br>";
  content += "You must restart device for network changes to take effect";
  content += "</body></html>";

  serverweb.send(200, "text/html", content);

}

void handleConf( void) {
  String  ip, mask, gate, dns, net;
  if (serverweb.hasArg("MAXCOUNT") && serverweb.hasArg("ACCEL") && serverweb.hasArg("MAX_SPEED") && serverweb.hasArg("LOW_SPEED") )
  {
    net = serverweb.arg("MAXCOUNT") + "\n" + serverweb.arg("MAX_SPEED") + "\n" + serverweb.arg("ACCEL") + "\n" + serverweb.arg("LOW_SPEED") + "\n";
    File f = SPIFFS.open("/roof.cfg", "w");
    if (!f)
    {
      net = ("file open failed");
    }
    else {
      f.println(net);
      f.close ();
    }

  }
  read_config();
  String content = "<html><style>" + String(BUTT) + String(TEXTT) + "</style>" + String(AUTO_SIZE) ;
  content += "<body  bgcolor=\"#000000\" text=\"#FF6000\"><form action='/config'  method='POST' ><h2>Config</h2>";
  content += "<fieldset style=\"width:15%;border-radius:15px\"><legend>Configuration</legend><table style='width:200px'>";
  content += "<tr><td>Top Count</td><td><input type='number' name='MAXCOUNT' class=\"text_red\" value='" + String(maxcounter) + "'></td></tr>";
  content += "<tr><td>Top Speed</td><td><input type='number' name='MAX_SPEED'class=\"text_red\" value='" + String(maxspeed) + "'></td></tr>";
  content += "<tr><td>Acceleration</td><td><input type='number' name='ACCEL' class=\"text_red\" value='" + String(acceleration) + "'></td></tr> ";
  content += "<tr><td>Find Speed</td><td><input type='number' name='LOW_SPEED'class=\"text_red\" value='" + String(lowspeed) + "'></td></tr></table>";
  content += "<input type='submit' name='SUBMIT'  class=\"button_red\" value='Save'></fieldset></form>"  "<br>";
  content += "<button onclick=\"location.href='/'\"class=\"button_red\" type=\"button\">Home</button><br>";
  content +=  net;
  content += "</body></html>";

  serverweb.send(200, "text/html", content);

}
void handleRestart(void)
{

  String content =   "<html><body  bgcolor=\"#000000\" text=\"#FFFFFF\"><h2>ROOF restarted</h2><br>";
  content += "Position saved on EEPROM.<br>";
  content += "<button onclick=\"location.href='/'\"class=\"button_red\" type=\"button\">Home</button><br>";
  content += "</body></html>";
  serverweb.send(200, "text/html", content);
  delay(1000);
  ESP.restart();
}

void initwebserver(void)

{
  serverweb.on("/", handleconfig);
  serverweb.on("/home", handleHome);
  serverweb.on("/move_to", handleMove);
  serverweb.on("/dome", handleDome);
  serverweb.on("/netcfg", handleNetwork);
  serverweb.on("/config", handleConf);
  serverweb.on("/restart", handleRestart);
  serverweb.on("/stop", handleStop);
  serverweb.onNotFound([]()
  {
    if (!handleFileRead(serverweb.uri()))
      serverweb.send(404, "text/plain", "FileNotFound");
  });
  serverweb.begin();
 #ifdef DHTTYPE
   dht.begin();
 #endif 
}
