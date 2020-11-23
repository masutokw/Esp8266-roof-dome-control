#include "web.h"
#include <FS.h>
extern int target, counter, maxcounter;
void handleconfig(void)
{
  String content =  "<html><head> <meta http-equiv='refresh' content='3'><style>" + String(BUTT) + String(TEXTT) + "</style>" + String(AUTO_SIZE) + " </head><body  bgcolor=\"#000000\" text=\"#FF6000\"><h2>Techo corredizo</h2><br>";
  content += "Posicion : " + String(counter) + "<br>";
  content += "ESTADO : " + String(counter > 0 ? "Abierto" : "Cerrado") + "<br>";
  content += "<button onclick=\"location.href='/open'\" class=\"button_red\" type=\"button\">Open</button><br>";
  content += "<button onclick=\"location.href='/close'\" class=\"button_red\" type=\"button\">Close</button><br>";
  content += "<button onclick=\"location.href='/netcfg'\" class=\"button_red\" type=\"button\">Network</button><br>";
  content += "</body></html>";

  serverweb.send(200, "text/html", content);
}
void handleOpen(void)
{ target = maxcounter;
  String content =  "<html><head> <meta http-equiv='refresh' content=\"1;url=/\"><style>" + String(BUTT) + String(TEXTT) + "</style>" + String(AUTO_SIZE) + "</head><body  bgcolor=\"#000000\" text=\"#FF6000\"><h2>Apertura</h2><br>";
  content += "Estado : " + String(target) + "<br>";
  content += "<button onclick=\"location.href='/close'\" class=\"button_red\" type=\"button\">close</button><br><br>";
  content += "</body></html>";

  serverweb.send(200, "text/html", content);
}
void handleClose(void)
{ target = 0;
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
  content += "Estado : " + String(target) + "<br>";
  content += "<td><input type='number' step='0.01' name='ANGLE' class=\"text_red\" value='" + String(target) + "'></td></tr>";
  content += "<button onclick=\"location.href='/open'\" class=\"button_red\" type=\"button\">open</button><br>";
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
  if (serverweb.hasArg("IP") && serverweb.hasArg("MASK") && serverweb.hasArg("GATEWAY") && serverweb.hasArg("DNS")&& serverweb.hasArg("SSID")&& serverweb.hasArg("PASSWD") )
  {
    String net = serverweb.arg("IP") + "\n" + serverweb.arg("MASK") + "\n" + serverweb.arg("GATEWAY") +"\n" +
    serverweb.arg("DNS") + "\n"+ serverweb.arg("SSID") + "\n"+ serverweb.arg("PASSWD") + "\n";
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
  content += "<tr><td>SSID</td><td><input type='text' name='SSID' class=\"text_red\" value='"+ssid+"'></td></td>";
  content += "<td><td>PASS</td><td><input type='password' name='PASSWD'class=\"text_red\" value='"+password+"'></td></tr>";
  content += "<tr><td>IP</td><td><input type='text' name='IP' class=\"text_red\" value='" + WiFi.localIP().toString() + "'></td></td>";
  content += "<td><td>MASK</td><td><input type='text' name='MASK'class=\"text_red\"  value='" + WiFi.subnetMask().toString() + "'></td></tr>";
  content += "<tr><td>Gateway</td><td><input type='text' name='GATEWAY' class=\"text_red\" value='" + WiFi.gatewayIP().toString() + "'></td></td>";
  content += "<td><td>DNS</td><td><input type='text' name='DNS' class=\"text_red\"  value='" + WiFi.gatewayIP().toString() + "'></td></tr></table>";
  content += "<input type='submit' name='SUBMIT'  class=\"button_red\" value='Save'></fieldset></form>" + msg + "<br>";
  content += "<button onclick=\"location.href='/'\"class=\"button_red\" type=\"button\">Back</button><br>";
  content += "You must restart de ice for network changes to take effect";
  content += "</body></html>";

  serverweb.send(200, "text/html", content);

}

void initwebserver(void)

{
  serverweb.on("/", handleconfig);
  serverweb.on("/open", handleOpen);
  serverweb.on("/close", handleClose);
  serverweb.on("/dome", handleDome);
  serverweb.on("/netcfg", handleNetwork);
  serverweb.onNotFound([]()
  {
    if (!handleFileRead(serverweb.uri()))
      serverweb.send(404, "text/plain", "FileNotFound");
  });
  serverweb.begin();
}
