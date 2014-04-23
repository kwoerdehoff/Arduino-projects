extern "C" {
  #include "socket.h"
  #include "string.h"
}

#include "EncNet.h"
#include "EncClient.h"
#include "EncServer.h"

EncServer::EncServer(uint16_t port)
{
  _port = port;
}

void EncServer::begin()
{
  for (int sock = 0; sock < MAX_SOCK_NUM; sock++) {
    EncClient client(sock);
    if (client.status() == SOCK_CLOSED) {
      socket(sock, Sn_MR_TCP, _port, 0);
      listen(sock);
      EncNet::_server_port[sock] = _port;
      break;
    }
  }  
}

void EncServer::accept()
{
  int listening = 0;
  
  for (int sock = 0; sock < MAX_SOCK_NUM; sock++) {
    EncClient client(sock);
    
    if (EncNet::_server_port[sock] == _port) {
      if (client.status() == SOCK_LISTEN) {
        listening = 1;
      } else if (client.status() == SOCK_CLOSE_WAIT && !client.available()) {
        client.stop();
      }
    } 
  }
  
  if (!listening) {      
    begin();
  }
}

EncClient EncServer::available()
{
  accept();
  
  for (int sock = 0; sock < MAX_SOCK_NUM; sock++) {
    EncClient client(sock);
    if (EncNet::_server_port[sock] == _port &&
        client.status() == SOCK_ESTABLISHED) {
      if (client.available()) {
        // XXX: don't always pick the lowest numbered socket.
        return client;
      }
    }
  }
  
  return EncClient(255);
}

void EncServer::write(uint8_t b)
{
  write(&b, 1);
}

void EncServer::write(const char *str)
{
  write((const uint8_t *)str, strlen(str));
}

void EncServer::write(const uint8_t *buffer, size_t size)
{
  accept();
  
  for (int sock = 0; sock < MAX_SOCK_NUM; sock++) {
    EncClient client(sock);
    
    if (EncNet::_server_port[sock] == _port &&
        client.status() == SOCK_ESTABLISHED) {
      client.write(buffer, size);
    }
  }
}
