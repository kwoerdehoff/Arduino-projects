#ifndef EncClient_h
#define EncClient_h

#include "Print.h"
#include "utility/socket.h"

class EncClient : public Print {
private:
  static uint16_t _srcport;
  uint8_t _sock;
  uint8_t *_ip;
  uint16_t _port;
#ifdef ETHERSHIELD_DEBUG
    char _DEBUG[80];
#endif

public:
  EncClient(uint8_t);
  EncClient(uint8_t *, uint16_t);
  uint8_t status();
  uint8_t connect();
  virtual void write(uint8_t);
  virtual void write(const char *str);
  virtual void write(const uint8_t *buf, size_t size);
  int available();
  int read();
  void flush();
  void stop();
#ifdef ETHERSHIELD_DEBUG
    char *debug();
#endif
  uint8_t connected();
  uint8_t operator==(int);
  uint8_t operator!=(int);
  operator bool();
  friend class EncServer;
};

#endif
