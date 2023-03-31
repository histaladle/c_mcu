#ifndef VPTRARRAY_H
#define VPTRARRAY_H
/*
 * 动态数组
 * 可以对数组进行插入删除操作
*/
#define VPTRARRAY_BUFF_SIZE 16
typedef struct _vptrarray vptrarray_t;
struct _vptrarray
{
    void *buff[VPTRARRAY_BUFF_SIZE];
    int count;
};
void *vptrarray_last(vptrarray_t *array);
void *vptrarray_at(vptrarray_t *array,int index);
int vptrarray_erase(vptrarray_t *array,int index);
int vptrarray_insert(vptrarray_t *array,int index,void *p);
int vptrarray_push(vptrarray_t *array,void *p);
int vptrarray_count(vptrarray_t *array);
int vptrarray_contain(vptrarray_t *array,void *p);
vptrarray_t *vptrarray_init(vptrarray_t *array);

#endif
