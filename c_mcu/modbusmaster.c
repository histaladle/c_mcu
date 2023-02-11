#include "modbusmaster.h"

OBJECTPOOL_IMPL(modbusmaster_transmission,modbusmaster_transmission_t,4)
OBJECTPOOL_IMPL(modbusmaster_reception,modbusmaster_reception_t,4)

static int modbusmaster_transmission_nextid=0;
void modbusmaster_transmission_init(modbusmaster_transmission_t *selv)
{
    selv->bufferlen=0;
    selv->transmitted=0;
    selv->reported=0;
    selv->waitclock=0;
    if(modbusmaster_transmission_nextid<0 || modbusmaster_transmission_nextid>0x1000) {
        modbusmaster_transmission_nextid=0;
    }
    selv->id=modbusmaster_transmission_nextid++;
}

static int modbusmaster_reception_nextid=0;
void modbusmaster_reception_init(modbusmaster_reception_t *selv)
{
    selv->bufferlen=0;
    selv->received=0;
    selv->reported=0;
    selv->waitclock=0;
    if(modbusmaster_reception_nextid<0x1000 || modbusmaster_reception_nextid>0x2000) {
        modbusmaster_reception_nextid=0;
    }
    selv->id=modbusmaster_reception_nextid++;
}

void modbusmaster_call(modbusmaster_t *selv)
{
    int i=0;
    int ok=0,matched=0;
    uint16_t crc=0;
    uint8_t crch=0,crcl=0;
    int startindex=0,endindex=0;
    uint8_t bytes[RINGMEMORY_BUFFERSIZE];
    modbusmaster_transmission_t *txfrm=NULL;
    modbusmaster_reception_t *rxfrm=NULL;
    ringmemory_copy(&selv->rxmem,bytes,selv->rxmem._readablecount,0);
    for(startindex=0;startindex<selv->rxmem._readablecount-2;++startindex) {
        for(endindex=startindex+2;endindex<selv->rxmem._readablecount;++endindex) {
            crc=modbusmaster_getcrc(bytes+startindex,endindex-startindex+1-2);
            crch=(crc>>8)&0xff;
            crcl=crc&0xff;
            if(crch==bytes[endindex-1] && crcl==bytes[endindex]) {
                if(endindex-startindex+1<4) continue;
                matched=0;
                for(i=0;i<dynarray_count(&selv->txframes);++i) {
                    txfrm=dynarray_at(&selv->txframes,i);
                    ok=1;
                    ok&=(txfrm->buffer[0]==bytes[startindex]);
                    ok&=((txfrm->buffer[1]&(~0x80)&0xff)==(bytes[startindex+1]&(~0x80)&0xff));
                    if(ok) {
                        do{
                            rxfrm=OBJECTPOOL_ALLOC(modbusmaster_reception);
                        }while(rxfrm==NULL);
                        rxfrm->id=txfrm->id;
                        memcpy(rxfrm->buffer,bytes+startindex,(size_t)(endindex-startindex+1));
                        rxfrm->bufferlen=endindex-startindex+1;
                        rxfrm->received=1;
                        dynarray_push(&selv->rxframes,rxfrm);
                        matched=1;
                        break;
                    }
                }
                if(!matched) {
                    do{
                        rxfrm=OBJECTPOOL_ALLOC(modbusmaster_reception);
                    }while(rxfrm==NULL);
                    modbusmaster_reception_init(rxfrm);
                    memcpy(rxfrm->buffer,bytes+startindex,(size_t)(endindex-startindex+1));
                    rxfrm->bufferlen=endindex-startindex+1;
                    rxfrm->received=1;
                    dynarray_push(&selv->rxframes,rxfrm);
                }
            }
        }
    }
}

void modbusmaster_init(modbusmaster_t *selv)
{
    dynarray_init(&selv->rxframes);
    dynarray_init(&selv->txframes);
    selv->rxfinclock=0;
    ringmemory_init(&selv->rxmem);
}

uint16_t modbusmaster_getcrc(uint8_t *bytes,int len)
{
    uint16_t res = 0xffff;
    while(len --)
    {
        res ^= *bytes ++;
        for(int i = 0; i < 8; i ++)
        {
            if(res & 0x01)
            {
                res = (res >> 1) ^ 0xa001;
            }
            else
            {
                res = res >> 1;
            }
        }
    }
    res = (uint16_t)(((res & 0xff) << 8) | (res >> 8));
    return res;
}
