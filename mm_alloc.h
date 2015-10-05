
//mm_alloc.h
//Liam Leibrandt 814078

#pragma once

#ifndef _malloc_H_
#define _malloc_H_

#define BLOCK_SIZE 40

#ifdef __cplusplus
extern "C" 
{
#endif

#include <stdlib.h>


void *base;

void* mm_realloc(void* ptr, size_t size);
void mm_free(void* ptr);
void* mm_malloc(size_t size);


typedef struct s_block *s_block_ptr;

struct s_block 
{
    size_t size;
    struct s_block *next;
    struct s_block *prev;
    int free;
    void *ptr;

    char data [0];
 };

void split_block(s_block_ptr b, size_t s)
{

	s_block_ptr newptr;

	newptr = b->data + s;
	newptr->size = b->size - s - BLOCK_SIZE;
	newptr->next = b->next;
	newptr->free = 1;

	b->size = s;
	b->next = newptr;

}

s_block_ptr find_block(s_block_ptr *last, size_t size)
{

	base = NULL;
	s_block_ptr baseptr = base;

	while(baseptr && !(baseptr->free && baseptr->size >= size)) {

		*last = baseptr;
		baseptr = baseptr->next;

	}

return (baseptr);

}

s_block_ptr fusion(s_block_ptr baseptr)
{

	if(baseptr->next && baseptr->next->free){
		baseptr->size += BLOCK_SIZE + baseptr->next->size;
		baseptr->next = baseptr->next->next;

		if (baseptr->next)
			baseptr->next->prev = baseptr;

	}

return (baseptr);

}

s_block_ptr get_block(void *l)
{

	char *tempptr;
	tempptr = l;
	return (l = tempptr -= BLOCK_SIZE);

}

void copy_block(s_block_ptr src, s_block_ptr dst)
{

	int *dynamicdata,*staticdata;
	size_t j;

	dynamicdata = dst->ptr;
	staticdata = src->ptr;


	for(j=0; j*4<src->size && j*4<dst->size; j++)
		dynamicdata[j] = staticdata[j];

}

s_block_ptr extend_heap(s_block_ptr last, size_t size)
{

	s_block_ptr baseptr;
	baseptr = sbrk(0);

	if(sbrk(BLOCK_SIZE + size) == (void*)-1)
		return (NULL);

	baseptr->size = size;
	baseptr->next = NULL;

	if (last)
		last->next = baseptr;

	baseptr->free = 0;

	return (baseptr);

}

int valid_addr(void *l)
{

	base = NULL;
	if(base)
	{
		if( l>base && l<sbrk(0))
			{
				return(l == (get_block(l))->ptr);

			}

	}

	return (0);
}

	#ifdef __cplusplus

}
#endif
#endif
