#ifndef EncNet_h
#define EncNet_h

#include <inttypes.h>
#include "EncClient.h"
#include "EncServer.h"
extern "C" {
    #include "utility/socket.h"
}

class EncNet {
private:
public:
  static uint8_t _state[MAX_SOCK_NUM];
  static uint16_t _server_port[MAX_SOCK_NUM];
  void begin(uint8_t *, uint8_t *);
  void begin(uint8_t *, uint8_t *, uint8_t *);
  void begin(uint8_t *, uint8_t *, uint8_t *, uint8_t *);
#ifdef ETHERSHIELD_DEBUG
  uint8_t *returnDebug();
  void clearDebug();
#endif
  friend class EncClient;
  friend class EncServer;
};

extern EncNet Ethernet;

#ifdef ETHERSHIELD_DEBUG
char *debug2str(uint8_t debugCode);
#endif
#endif
