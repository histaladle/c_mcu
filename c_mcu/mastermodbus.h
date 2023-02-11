#ifndef MASTERMODBUS_H
#define MASTERMODBUS_H

#include "stdint.h"
#include "ringmemory.h"
#include "dynarray.h"
#define MASTERMODBUS_MAX_TXLEN 16
#define MASTERMODBUS_MAX_RXLEN 16
typedef struct _mastermodbussession 
{
    uint8_t txbytes[MASTERMODBUS_MAX_TXLEN];//已发送的帧
    int txlen;//已发送的帧长度
    uint8_t rxbytes[MASTERMODBUS_MAX_RXLEN];//已接收的帧
    int rxlen;//已接收的帧长度
    int transmitted;//是否发送 0:未发送; 1:已发送
    int received;//是否回复 0:未回复; 1:已回复
    int retransmission;//重发次数
    int waitclock;//等待时间
} mastermodbussession_t;
typedef struct _mastermodbus
{
    ringmemory_t rxmem;
    dynarray_t sessions;
    int rxfinclock;
} mastermodbus_t;
extern void mastermodbus_call(mastermodbus_t *protocol);
extern uint16_t mastermodbus_getcrc(uint8_t *bytes, int len);
#endif
