/*
  mm_alloc.c
 
  Liam Leibrandt 814078

	*!! Please note that some of my code is in mm_alloc.h !!*
 
  Stub implementations of the mm_* routines.

	Summary:

	The first thing the malloc does is to test base , if it NULL then this the first time its called.
	The malloc follow these steps:

	• First align the requested size
	• If base is initialized:
		– Search for a free chunk wide enough;
		– If we found a chunk:
			* Try to split the block (the diff erence between the requested size and the size of
			the block is enough to store the meta-data and a minimal block (4 bytes;)
			* Mark the chunk as used (b->free=0;)
	 	Otherwise: we extend the heap.
	 	The use of the last: find block put the pointer to the last visited chunk in last
		, so we can access it during the extension.
	• Otherwise: extend the heap.

	THe realloc method:
	•Allocate a new bloc of the given size using malloc;
	•Copy data from the old one to the new one;
	•Free the old block;
	•Return the new pointer.
	 
	I don’t want a new allocation if I have enough room where we are.
	 The different cases are thus:
	•If the size doesn’t change, or the extra-available size 
	(do to alignement constraint, or if the ramainning size was to small to split) is sufficient, we do nothing;
	•If it shrinks the block, it tries a split;
	•If the next block is free and provide enough space, it fuses and try to split if necessary.

	The call realloc(NULL,s) is valid and should be replaced by malloc(s).

	The free method:
	Verifies the pointer and get the corresponding chunk, we mark
	it free and fusion it if possible. It also try to release memory if its at the end of the heap.
	Releasing memory: if it at the end of the heap, it just have to put the
	break at the chunk position with a call to brk(2).

	It follows this structure:
	• If the pointer is valid:
		–it gets the block address
		–it marks it free
		–if the previous exists and is free, it steps backward in the block list and fusion the
			two blocks.
		–it also tries to fuse with then next block
		–if the last block it releases memory.
		–if there’s no more block, it goes back to the original state (set base to NULL.)
	•If the pointer is not valid, it does nothing

 */

#include "mm_alloc.h"

#include <stdlib.h>
void *base;

void* mm_malloc(size_t size)
{
	#ifdef MM_USE_STUBS  
	#else

	base = NULL;

	#define align4(size)

	size_t s;
	s = size;

	s_block_ptr baseptr,last;


	if (base) {
		last = base;

		baseptr = find_block(&last,s);

		if (baseptr) {
			if ((baseptr->size - s) >= (BLOCK_SIZE + 4))
				split_block(baseptr,s);

		baseptr->free=0;
		}
		else
		{
			baseptr = extend_heap(last,s);

			if (!baseptr)
				return (NULL);
		}
	}
	else
	{
	baseptr = extend_heap(NULL,s);

	if (!baseptr)
		return (NULL);

	base = baseptr;
	}

	return calloc(1, size);
	#endif
}

void* mm_realloc(void* ptr, size_t size)
{

	#ifdef MM_USE_STUBS
	#else

	void *newp;
	s_block_ptr baseptr, new;
	size_t sSize;

	if (!ptr)
		return (malloc(size));

	if (valid_addr(ptr))
	{
		#define align4(size) //(((((size)-1)>>2)<<2)+4)
		sSize = align4(size) (((((size)-1)>>2)<<2)+4);

		baseptr = get_block(ptr);

		if (baseptr->size >= sSize)
		{
			if (baseptr->size - sSize >= (BLOCK_SIZE + 4))
				split_block(baseptr,sSize);
		}

		else
		{
			if (baseptr->next && baseptr->next->free && (baseptr->size + BLOCK_SIZE + baseptr->next->size) >= sSize)
			{
				fusion(baseptr);
				if (baseptr->size - sSize >= (BLOCK_SIZE + 4))
					split_block(baseptr,sSize);
			}
			else
			{
				newp = malloc(sSize);
					if (!newp)
						return (NULL);

					new = get_block(newp);
					copy_block(baseptr,new);
					free(ptr);
					return (newp);

			}

		}

		return (ptr);

	}

	return (NULL);
	return realloc(ptr, size);

	#endif
}

void mm_free(void* ptr)
{
	#ifdef MM_USE_STUBS
	#else

	base=NULL;

	s_block_ptr baseptr;

	if (valid_addr(ptr))
	{
		baseptr = get_block(ptr);
		baseptr->free = 1;
			if (baseptr->prev && baseptr->prev->free)
				baseptr = fusion(baseptr->prev);
			if (baseptr->next)
				fusion(baseptr);
			else
			{
				if (baseptr->prev)
					baseptr->prev->next = NULL;
				else
					base = NULL;

			brk(baseptr);

			}

	}

	free(ptr);
	#endif
}
