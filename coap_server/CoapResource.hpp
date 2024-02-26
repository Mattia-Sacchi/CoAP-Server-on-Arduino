#ifndef _COAP_RESOURCE_HPP_
#define _COAP_RESOURCE_HPP_

#include <Arduino.h>

class CoapPacket;



typedef void EesourceCallback(CoapPacket &packet, IPAddress ip, int port)

class CoAPResource{
public:
  
  static CoAPResource initResoure(String name, EesourceCallback );
private:
  CoAPResource(String name,);
  bool m_isValid;

};

#endif