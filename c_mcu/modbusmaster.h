#ifndef MODBUSMASTER_H
#define MODBUSMASTER_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "dynarray.h"
#include "ringmemory.h"
#include "objectpool.h"

#define MODBUSMASTER_TRANSMISSION_BUFFER_LEN 16
#define MODBUSMASTER_RECEPTION_BUFFER_LEN 16

typedef struct _modbusmaster_transmission {
    uint8_t buffer[MODBUSMASTER_TRANSMISSION_BUFFER_LEN];
    int bufferlen;
    int transmitted;
    int reported;
    int waitclock;
    int id;
} modbusmaster_transmission_t;
void modbusmaster_transmission_init(modbusmaster_transmission_t *selv);

typedef struct _modbusmaster_reception {
    uint8_t buffer[MODBUSMASTER_RECEPTION_BUFFER_LEN];
    int bufferlen;
    int received;
    int reported;
    int waitclock;
    int id;
} modbusmaster_reception_t;
void modbusmaster_reception_init(modbusmaster_reception_t *selv);

OBJECTPOOL_DECL(modbusmaster_transmission,modbusmaster_transmission_t)
OBJECTPOOL_DECL(modbusmaster_reception,modbusmaster_reception_t)

typedef struct _modbusmaster {
    dynarray_t txframes;
    dynarray_t rxframes;
    int rxfinclock;
    ringmemory_t rxmem;
} modbusmaster_t;
extern void modbusmaster_call(modbusmaster_t *selv);
extern uint16_t modbusmaster_getcrc(uint8_t *bytes,int len);
extern void modbusmaster_init(modbusmaster_t *selv);
#endif
