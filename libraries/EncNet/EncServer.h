#ifndef EncServer_h
#define EncServer_h

#include "Print.h"

class EncClient;

class EncServer : public Print {
private:
  uint16_t _port;
  void accept();
public:
  EncServer(uint16_t);
  EncClient available();
  void begin();
  virtual void write(uint8_t);
  virtual void write(const char *str);
  virtual void write(const uint8_t *buf, size_t size);
};

#endif
