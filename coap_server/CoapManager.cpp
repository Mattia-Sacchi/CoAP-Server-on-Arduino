#include "CoapManager.hpp"
#include <coap-simple.h>
#include <WiFiUdp.h>


CoAPServerManager::CoAPServerManager()
: m_ap(nullptr),m_coap(nullptr)
{
//coap.server(callback_light, "light");

  // client response callback.
  // this endpoint is single callback.
  //coap.response(callback_response);

  // start coap server/client
}

/*

  // send GET or PUT coap request to CoAP server.
  // To test, use libcoap, microcoap server...etc
  // int msgid = coap.put(IPAddress(10, 0, 0, 1), 5683, "light", "1");

  // Serial.println("Send Request");
  // int msgid = coap.get(IPAddress(1, XXX, XXX, XXX), 5683, "time");
if you change LED, req/res test with coap-client(libcoap), run following.
coap-client -m get coap://(arduino ip addr)/light
coap-client -e "1" -m put coap://(arduino ip addr)/light
coap-client -e "0" -m put coap://(arduino ip addr)/light
*/
bool CoAPServerManager::init(UdpAccessPoint & ap)
{
  m_ap = &ap;
  WiFiUDP udp = *m_ap->udp();
  m_coap = new Coap(udp);
  return m_initialized = true;

}

bool CoAPServerManager::addResource()
{
  if(!m_initialized) // Resource not initialized
    return false;

  // if there is at least one resource is ready
  return m_ready = true ;
}

bool CoAPServerManager::start()
{
  if(!m_ready)
    return false;
  
  return m_running = true;

}

void CoAPServerManager::loop()
{
  if(!m_running || !m_ready)
    return;
  m_coap->loop();
}
