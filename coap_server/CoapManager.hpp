#ifndef _COAP_SERVER_MANAGER_HPP_
#define _COAP_SERVER_MANAGER_HPP_

#include "UdpAccessPoint.hpp"
class Coap;

class CoapResource;


class CoAPServerManager
{
public:
  CoAPServerManager();
  bool init(UdpAccessPoint & ap);
  void loop();
  bool addResource(CoapResource resource);
  bool start();


private:
  UdpAccessPoint * m_ap;
  Coap *m_coap;
  bool m_ready;
  bool m_running;
  bool m_initialized;
};

#endif