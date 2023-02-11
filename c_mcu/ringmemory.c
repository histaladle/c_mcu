#include "ringmemory.h"
#include "string.h"
#define RINGMEMORY_PRESS()
#define RINGMEMORY_RELEASE()

/*读环形缓冲区 返回实际读到的长度*/
int ringmemory_read(ringmemory_t *mem, uint8_t *dst, int max_count)
{
    int res=0;
    int rem_cnt=0;
    int block=0;
    if(max_count<=0)
    {
        return 0;
    }
    RINGMEMORY_PRESS();
    res=mem->_readablecount;
    res=res<max_count? res:max_count;
    rem_cnt=res;
    while(rem_cnt>0)
    {
        block=rem_cnt<(mem->buffersize-mem->rdindex)? rem_cnt:(mem->buffersize-mem->rdindex);
        if(dst!=NULL) memcpy(dst+res-rem_cnt, mem->buffer+mem->rdindex, (size_t)block);
        mem->rdindex=(mem->rdindex+block)%mem->buffersize;
        rem_cnt-=block;
        mem->_readablecount-=block;
    }
    if(mem->_readablecount>0)
    {
        mem->_status=RINGMEMORY_STATUS_READBLE;
    }
    else
    {
        mem->_status=RINGMEMORY_STATUS_EMPTY;
    }
    RINGMEMORY_RELEASE();
    res=res-rem_cnt;
    return res;
}

/*丢弃环形缓冲区前max_count个数据 如果实际数据小于max_count 则返回实际丢弃的数据*/
int ringmemory_dump(ringmemory_t *mem, int max_count)
{
    int res=0;
    int rem_cnt=0;
    int block=0;
    if(max_count<=0)
    {
        return 0;
    }
    RINGMEMORY_PRESS();
    res=mem->_readablecount;
    res=res<max_count? res:max_count;
    rem_cnt=res;
    while(rem_cnt>0)
    {
        block=rem_cnt<(mem->buffersize-mem->rdindex)? rem_cnt:(mem->buffersize-mem->rdindex);
        mem->rdindex=(mem->rdindex+block)%mem->buffersize;
        rem_cnt-=block;
        mem->_readablecount-=block;
    }
    if(mem->_readablecount>0)
    {
        mem->_status=RINGMEMORY_STATUS_READBLE;
    }
    else
    {
        mem->_status=RINGMEMORY_STATUS_EMPTY;
    }
    RINGMEMORY_RELEASE();
    res=res-rem_cnt;
    return res;
}

/*写弃环形缓冲区*/
int ringmemory_write(ringmemory_t *mem, uint8_t *src, int count)
{
    int res=0;
    int rem_cnt=count;
    int wrprobe=(mem->rdindex+mem->_readablecount)%mem->buffersize;
    int block=0;
    if(count<=0)
    {
        return res;
    }
    RINGMEMORY_PRESS();
    while(rem_cnt>0)
    {
        block=rem_cnt<(mem->buffersize-wrprobe)?rem_cnt:(mem->buffersize-wrprobe);
        if(block>mem->buffersize-mem->_readablecount)
        {
            mem->rdindex=(wrprobe+block)%mem->buffersize;
            mem->_status=RINGMEMORY_STATUS_WRITEOVERLAPPED;
        }
        else
        {
            mem->_status=RINGMEMORY_STATUS_READBLE;
        }
        memcpy(mem->buffer+wrprobe, src+count-rem_cnt, (size_t)block);
        wrprobe=(wrprobe+block)%mem->buffersize;
        rem_cnt-=block;
        (mem->_readablecount+block)<=mem->buffersize? (mem->_readablecount+=block):(mem->_readablecount=mem->buffersize);
    }
    RINGMEMORY_RELEASE();
    res=count-rem_cnt;
    return res;
}

/*环形缓冲区剩余空间*/
int ringmemory_wirtablecount(ringmemory_t *mem)
{
    int res=0;
    RINGMEMORY_PRESS();
    res=mem->buffersize-mem->_readablecount;
    RINGMEMORY_RELEASE();
    return res;
}

/*复制环形缓冲区在偏移offset处的size个数据 offset=0是第一个数据*/
int ringmemory_copy(ringmemory_t *mem, uint8_t *dst, int size, int offset)
{
    int res=size;
    int i=0;
    RINGMEMORY_PRESS();
    for(i=0;i<size;i++)
    {
        dst[i]=mem->buffer[(mem->rdindex+offset+i)%mem->buffersize];
    }
    RINGMEMORY_RELEASE();
    return res;
}

/*返回环形缓冲区在index的数据 index=0是第一个数据*/
uint8_t ringmemory_at(ringmemory_t *mem, int index)
{
    uint8_t d=0;
    RINGMEMORY_PRESS();
    d=mem->buffer[(mem->rdindex+index)%mem->buffersize];
    RINGMEMORY_RELEASE();
    return d;
}

int ringmemory_status(ringmemory_t *mem)
{
    int s=0;
    RINGMEMORY_PRESS();
    s=mem->_status;
    RINGMEMORY_RELEASE();
    return s;
}

void ringmemory_init(ringmemory_t *mem)
{
    mem->rdindex=0;
    mem->_readablecount=0;
    mem->_status=RINGMEMORY_STATUS_EMPTY;
    mem->buffersize=RINGMEMORY_BUFFERSIZE;
    memset(mem->buffer, 0, (size_t)mem->buffersize);
}
