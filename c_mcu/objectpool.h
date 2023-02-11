#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H
/*
 * 对象池
 * 可以动态分配对象(alloc)
 * 可以动态释放对象(free)
 * OBJECTPOOL_DECL(NAME,TYPE) 展开成声明(<TYPE> *<NAME>_alloc(); void <NAME>_free(<TYPE> *p);
 * OBJECTPOOL_IMPL(NAME,TYPE,COUNT) 展开成实现(<TYPE> *<NAME>_alloc(); void <NAME>_free(<TYPE> *p);
 * <NAME> 对象池函数名称 对于一个<NAME> OBJECTPOOL_IMPL 全局只能出现一次
 * <TYPE> 经过typedef定义的类型 例如 typedef struct xxx{} xxx_t;
 * <CAPACITY> 预留的对象数量
*/
#include "stdint.h"
#define OBJECTPOOL_DECL(NAME,TYPE) extern TYPE *NAME##_alloc(void); extern void NAME##_free(TYPE *p);

#define OBJECTPOOL_IMPL(NAME,TYPE,CAPACITY) \
    static int NAME##_usedflags[CAPACITY]={0}; \
    static TYPE NAME##_rsvdarr[CAPACITY]; \
    TYPE *NAME##_alloc(){int i=0;for(i=0;i<CAPACITY;i++){if(NAME##_usedflags[i]==0) return &NAME##_rsvdarr[i];}return NULL;} \
    void NAME##_free(TYPE *p){int i=0;for(i=0;i<CAPACITY;i++){if(p==&NAME##_rsvdarr[i]){NAME##_usedflags[i]=0;}}}

#define OBJECTPOOL_ALLOC(NAME) NAME##_alloc()

#define OBJECTPOOL_FREE(NAME,PTR) NAME##_free(PTR)

#endif
