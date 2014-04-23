extern "C" {
    #include "socket.h"
}

#include "EncNet.h"

// XXX: don't make assumptions about the value of MAX_SOCK_NUM.
uint8_t EncNet::_state[MAX_SOCK_NUM] = { 0, 0, 0, 0 };
uint16_t EncNet::_server_port[MAX_SOCK_NUM] = { 0, 0, 0, 0 };

void EncNet::begin(uint8_t *mac, uint8_t *ip)
{
    uint8_t gateway[4];
    gateway[0] = ip[0];
    gateway[1] = ip[1];
    gateway[2] = ip[2];
    gateway[3] = 1;
    begin(mac, ip, gateway);
}

void EncNet::begin(uint8_t *mac, uint8_t *ip, uint8_t *gateway)
{
    uint8_t subnet[] = { 255, 255, 255, 0 };
    begin(mac, ip, gateway, subnet);
}

void EncNet::begin(uint8_t *mac, uint8_t *ip, uint8_t *gateway, uint8_t *subnet)
{
    iinchip_init();
    sysinit(0x55, 0x55);
    setSHAR(mac);
    setSIPR(ip);
    setGAR(gateway);
    setSUBR(subnet);
}

#ifdef ETHERSHIELD_DEBUG
uint8_t *EncNet::returnDebug() {
    return socketDebug();
}
void EncNet::clearDebug() {
    socketClearDebug();
}

char *debug2str(uint8_t debugCode) {
    return debugCode2String(debugCode);
}
#endif

EncNet Ethernet;