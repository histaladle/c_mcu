#ifndef RINGMEMORY_H
#define RINGMEMORY_H

#include "stdint.h"
#include "stdlib.h"
#define RINGMEMORY_BUFFERSIZE 8*1024

#define RINGMEMORY_STATUS_ERROR -1
#define RINGMEMORY_STATUS_EMPTY 0
#define RINGMEMORY_STATUS_READBLE 1
#define RINGMEMORY_STATUS_FULL 2
#define RINGMEMORY_STATUS_WRITEOVERLAPPED 3

typedef struct _ringmemory
{
	int rdindex;
	int _readablecount;
	int _status;
    int buffersize;
    uint8_t *buffer;
} ringmemory_t;

extern int ringmemory_read(ringmemory_t *mem, uint8_t *dst,int max_count);
extern int ringmemory_dump(ringmemory_t *mem, int max_count);
extern int ringmemory_write(ringmemory_t *mem, uint8_t *src, int count);
extern int ringmemory_wirtablecount(ringmemory_t *mem);
extern int ringmemory_copy(ringmemory_t *mem, uint8_t *dst, int size, int offset);
extern uint8_t ringmemory_at(ringmemory_t *mem, int index);
extern int ringmemory_status(ringmemory_t *mem);

extern void ringmemory_init(ringmemory_t *mem);

#endif
