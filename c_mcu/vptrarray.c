#include <stdio.h>
#include "vptrarray.h"
void *vptrarray_last(vptrarray_t *array)
{
    if(array==NULL)
    {
        return NULL;
    }
    if(array->count<=0)
    {
        return NULL;
    }
    return array->buff[array->count-1];
}

void *vptrarray_at(vptrarray_t *array,int index)
{
    if(array==NULL)
    {
        return NULL;
    }
    if(array->count<=0||index>=VPTRARRAY_BUFF_SIZE)
    {
        return NULL;
    }
    return array->buff[index];
}

int vptrarray_erase(vptrarray_t *array,int index)
{
    int i=0;
    if(array==NULL)
    {
        return -1;
    }
    if(array->count<=0||index<0||index>=array->count||index>=VPTRARRAY_BUFF_SIZE)
    {
        return -1;
    }
    array->buff[index]=NULL;
    for(i=index;i<array->count-1;i++)
    {
        array->buff[i]=array->buff[i+1];
    }
    for(i=array->count-1;i<VPTRARRAY_BUFF_SIZE;i++)
    {
        array->buff[i]=NULL;
    }
    array->count=array->count-1;
    return 0;
}

int vptrarray_insert(vptrarray_t *array,int index,void *p)
{
    int i=0;
    if(array==NULL)
    {
        return -1;
    }
    if(array->count<=0||index<0||index>array->count||index>=VPTRARRAY_BUFF_SIZE||array->count>VPTRARRAY_BUFF_SIZE)
    {
        return -1;
    }
    for(i=array->count;i>index;i--)
    {
        array->buff[i]=array->buff[i-1];
    }
    array->buff[index]=p;
    array->count=array->count+1;
    return 0;
}

int vptrarray_push(vptrarray_t *array,void *p)
{
    if(array==NULL)
    {
        return -1;
    }
    if(array->count<=0||array->count>VPTRARRAY_BUFF_SIZE)
    {
        array->count=0;
    }
    array->buff[array->count]=p;
    array->count=array->count+1;
    return 0;
}

int vptrarray_count(vptrarray_t *array)
{
    return array->count;
}

int vptrarray_contain(vptrarray_t *array, void *p)
{
    int i=0,t=-1;
    for(i=0;i<array->count;i++)
    {
        if(array->buff[i]==p)
        {
            t=i;
            break;
        }
    }
    return t;
}

vptrarray_t *vptrarray_init(vptrarray_t *array)
{
    int i=0;
    if(array==NULL)
    {
        return NULL;
    }
    array->count=0;
    for(i=0;i<VPTRARRAY_BUFF_SIZE;i++)
    {
        array->buff[i]=NULL;
    }
    return array;
}
