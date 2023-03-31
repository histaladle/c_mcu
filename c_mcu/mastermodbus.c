#include "mastermodbus.h"
#include "string.h"
#include "objectpool.h"

OBJECTPOOL_IMPL(mastermodbussession,mastermodbussession_t,4)

void mastermodbus_call(mastermodbus_t *protocol)
{
    int i=0;
	int ok=0;
	uint16_t crc=0;
	unsigned char crch=0,crcl=0;
	int startindex=0,endindex=0;
	//int rqindex=0,rpindex=0;
	uint8_t bytes[RINGMEMORY_BUFFERSIZE];
    mastermodbussession_t *sess=NULL;
	ringmemory_copy(&protocol->rxmem,bytes,protocol->rxmem._readablecount,0);
	for(startindex=0;startindex<protocol->rxmem._readablecount-2;startindex++)
    {
		for(endindex=startindex+2;endindex<protocol->rxmem._readablecount;endindex++)
		{
			crc=mastermodbus_getcrc(bytes+startindex,endindex-startindex+1-2);
			crch=(crc>>8)&0xff;
            crcl=crc&0xff;
			if(crch==bytes[endindex-1] && crcl==bytes[endindex])
			{
                if(endindex-startindex+1<4) continue;
                if(vptrarray_count(&protocol->sessions)==0) 
                {
                    do{
                        sess=mastermodbussession_alloc();
                    }while(sess==NULL);
                    sess->txlen=0;
                    sess->transmitted=0;
                    sess->rxlen=endindex-startindex+1;
                    memcpy(sess->rxbytes,bytes+startindex,(size_t)(endindex-startindex+1));
                    sess->txlen=0;
                    sess->transmitted=0;
                    sess->rxlen=endindex-startindex+1;
                    memcpy(sess->rxbytes,bytes+startindex,(size_t)(endindex-startindex+1));
                    sess->received=1;
                    vptrarray_push(&protocol->sessions,sess);
                }
                else 
                {
                    ok=0;
                    for(i=0;i<vptrarray_count(&protocol->sessions);i++)
                    {
                        sess=(mastermodbussession_t*)vptrarray_at(&protocol->sessions,i);
                        ok|=sess->transmitted;
                    }
                    if(ok)
                    {
                        for(i=0;i<vptrarray_count(&protocol->sessions);i++)
                        {
                            sess=(mastermodbussession_t*)vptrarray_at(&protocol->sessions,i);
                            if(sess->transmitted && !sess->received)
                            {
                                if(bytes[startindex]!=sess->txbytes[0]) break;
                                if(bytes[1]&0x80)
                                {
                                    ok=((bytes[1]&(~0x80))==sess->txbytes[1]);
                                    if(!ok) break;
                                    memcpy(sess->rxbytes,bytes+startindex,(size_t)(endindex-startindex+1));
                                    sess->txlen=endindex-startindex+1;
                                    sess->received=1;
                                }
                                else 
                                {
                                    ok=(bytes[1]==sess->txbytes[1]);
                                    if(!ok) continue;
                                    memcpy(sess->rxbytes,bytes+startindex,(size_t)(endindex-startindex+1));
                                    sess->rxlen=endindex-startindex+1;
                                    sess->received=1;
                                }
                            }
                        }
                    }
                    else 
                    {
                        do{
                            sess=mastermodbussession_alloc();
                        }while(sess==NULL);
                        sess->txlen=0;
                        sess->transmitted=0;
                        sess->rxlen=endindex-startindex+1;
                        memcpy(sess->rxbytes,bytes+startindex,(size_t)(endindex-startindex+1));
                        sess->received=1;
                        vptrarray_push(&protocol->sessions,sess);
                    }
                }
			}
		}
	}
}

uint16_t mastermodbus_getcrc(uint8_t *bytes, int len)
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
    res = ((res & 0xff) << 8) | (res >> 8);
    return res;
}
