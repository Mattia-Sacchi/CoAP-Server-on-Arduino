#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <WiFi.h>
#include <coap-simple.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiServer.h>

#define LEDP 9


// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port);

// CoAP server endpoint url callback
void callback_light(CoapPacket &packet, IPAddress ip, int port);

// UDP and CoAP class
WiFiUDP Udp;
Coap coap(Udp);
WiFiServer server(5683);

// LED STATE
bool LEDSTATE;

// CoAP server endpoint URL
void callback_light(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Light] ON/OFF");
  
  // send response
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  String message(p);

  if (message.equals("0"))
    LEDSTATE = false;
  else if(message.equals("1"))
    LEDSTATE = true;
      
  if (LEDSTATE) {
    digitalWrite(LEDP, HIGH) ; 
    coap.sendResponse(ip, port, packet.messageid, "1");
  } else { 
    digitalWrite(LEDP, LOW) ; 
    coap.sendResponse(ip, port, packet.messageid, "0");
  }
}

// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Coap Response got]");
  
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  Serial.println(p);
}

int status = WL_IDLE_STATUS;
char ssid[] = "yourNetwork"; //  your network SSID (name)
char pass[] = "secretPassword";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}

void setup() {
  Serial.begin(9600);

   // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(1000);

  Serial.println("Ready");

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();

  // LED State
  pinMode(LEDP, OUTPUT);
  digitalWrite(LEDP, HIGH);
  LEDSTATE = true;
  
  // add server url endpoints.
  // can add multiple endpoint urls.
  // exp) coap.server(callback_switch, "switch");
  //      coap.server(callback_env, "env/temp");
  //      coap.server(callback_env, "env/humidity");
  Serial.println("Setup Callback Light");
  coap.server(callback_light, "light");

  // client response callback.
  // this endpoint is single callback.
  Serial.println("Setup Response Callback");
  coap.response(callback_response);

  // start coap server/client
  coap.start();
}

void loop() {
  // send GET or PUT coap request to CoAP server.
  // To test, use libcoap, microcoap server...etc
  // int msgid = coap.put(IPAddress(10, 0, 0, 1), 5683, "light", "1");
  
  
  //Serial.println("Send Request");
  //int msgid = coap.get(IPAddress(1, XXX, XXX, XXX), 5683, "time");

  delay(100);
  coap.loop();
}
/*
if you change LED, req/res test with coap-client(libcoap), run following.
coap-client -m get coap://(arduino ip addr)/light
coap-client -e "1" -m put coap://(arduino ip addr)/light
coap-client -e "0" -m put coap://(arduino ip addr)/light
*/