#ifndef DYNARRAY_H
#define DYNARRAY_H
/*
 * 动态数组 dynamic array
 * 可以对数组进行插入删除操作
*/
#define DYNARRAY_BUFF_SIZE 16
typedef struct _dynarray dynarray_t;
struct _dynarray
{
    void *buff[DYNARRAY_BUFF_SIZE];
    int count;
};
void *dynarray_last(dynarray_t *array);
void *dynarray_at(dynarray_t *array,int index);
int dynarray_erase(dynarray_t *array,int index);
int dynarray_insert(dynarray_t *array,int index,void *p);
int dynarray_push(dynarray_t *array,void *p);
int dynarray_count(dynarray_t *array);
int dynarray_contain(dynarray_t *array,void *p);
dynarray_t *dynarray_init(dynarray_t *array);

#endif
