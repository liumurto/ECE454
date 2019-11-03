/*
 * This implementation replicates the implicit list implementation
 * provided in the textbook
 * "Computer Systems - A Programmer's Perspective"
 * Blocks are never coalesced or reused.
 * Realloc is implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "mm.h"
#include "memlib.h"

//--------------------------------------------------------------------------------------------------------------------------------------------

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "mm_free(me)",
    /* First member's full name */
    "Murton Liu",
    /* First member's email address */
    "murton.liu@mail.utoronto.ca",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/*************************************************************************
 * Basic Constants and Macros
 * You are not required to use these macros but may find them helpful.
*************************************************************************/
#define WSIZE       sizeof(void *)            /* word size (bytes) */
#define DSIZE       (2 * WSIZE)            /* doubleword size (bytes) */
#define CHUNKSIZE   (1<<7)      /* initial heap size (bytes) */

#define MAX(x,y) ((x) > (y)?(x) :(y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)          (*(uintptr_t *)(p))
#define PUT(p,val)      (*(uintptr_t *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)     (GET(p) & ~(DSIZE - 1))
#define GET_ALLOC(p)    (GET(p) & 0x1)

/* Get the next and prev pointer to free block given pointer to header of a free block */
#define GET_NEXT_PTR(p)  (GET((char*)(p)+WSIZE))
#define GET_PREV_PTR(p)  (GET((char*)(p)+DSIZE))

/* Set the next and prev pointer to free block given pointer to header of a free block */
#define SET_NEXT_PTR(p,val)  (PUT((char*)(p)+WSIZE, val))
#define SET_PREV_PTR(p,val)  (PUT((char*)(p)+DSIZE, val))

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)        ((char *)(bp) - WSIZE)
#define FTRP(bp)        ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* Added definitions for segregated free list impl. */
#define MIN_BLOCK_SIZE (WSIZE * 4)
#define MAX_LISTS 32

void *heap_listp = NULL;
void *epilogue = NULL;
void *segmented_free_list[MAX_LISTS];

/**********************************************************
 * mm_init
 * Initialize the heap, including "allocation" of the
 * prologue and epilogue
 **********************************************************/
int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1)
        return -1;
    PUT(heap_listp, 0);                         // alignment padding
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));   // prologue header
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));   // prologue footer
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));    // epilogue header

    epilogue = heap_listp + 3 * WSIZE - WSIZE;

    heap_listp += DSIZE;

    for(int i = 0; i < MAX_LISTS; i++)
    {
    	segmented_free_list[i] = NULL;    
    }

    return 0;
}

void *split(void *bp, size_t req_size)
{
	size_t size = GET_SIZE(bp);
	remove_from_list(bp);

	size_t frag = size - req_size;

	/* Check if size after allocation of req. block size is splittable (i.e. leftover is size of min. allocation) */
	if (frag >= MIN_BLOCK_SIZE)
	{
		uintptr_t block_head = bp;
		uintptr_t block_foot = bp + req_size - WSIZE;

		PUT(block_head, PACK(req_size, 0));
		PUT(block_foot, PACK(req_size, 0));

		block_head = bp + req_size;
		block_foot = bp + size - WSIZE;

		PUT(block_head, PACK(frag, 0));
		PUT(block_foot, PACK(frag, 0));

		add_to_list(block_head);
	}

	uintptr_t new_block = bp + WSIZE;

	return new_block;
}

/**********************************************************
 * coalesce
 * Covers the 4 cases discussed in the text:
 * - both neighbours are allocated
 * - the next block is available for coalescing
 * - the previous block is available for coalescing
 * - both neighbours are available for coalescing
 **********************************************************/
void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));
    
    if (prev_alloc && next_alloc) {       /* Case 1 */
    	return bp;
    }

    else if (prev_alloc && !next_alloc) { /* Case 2 */
        remove_from_list(HDRP(NEXT_BLKP(bp)));

        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        return (bp);
    }

    else if (!prev_alloc && next_alloc) { /* Case 3 */
        remove_from_list(HDRP(PREV_BLKP(bp)));
        
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        return (PREV_BLKP(bp));
    }

    else {            /* Case 4 */
        remove_from_list(HDRP(NEXT_BLKP(bp)));
        remove_from_list(HDRP(PREV_BLKP(bp)));

        size += GET_SIZE(HDRP(PREV_BLKP(bp)))  +
            GET_SIZE(FTRP(NEXT_BLKP(bp)))  ;
        PUT(HDRP(PREV_BLKP(bp)), PACK(size,0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size,0));
        return (PREV_BLKP(bp));
    }
}

/**********************************************************
 * extend_heap
 * Extend the heap by "words" words, maintaining alignment
 * requirements of course. Free the former epilogue block
 * and reallocate its new header
 **********************************************************/
void *extend_heap(size_t words)
{
    char *bp;

    /* Check for last block free -> if yes, extend by requested_words - last_block_size; otherwise extend by size requested */
    int last_block_free = GET_ALLOC(epilogue);

    if (!last_block_free)
	    words -= GET_SIZE(epilogue);

    if ((bp = mem_sbrk(words)) == (void *)-1)
        return NULL;

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(words, 0));                // free block header
    PUT(FTRP(bp), PACK(words, 0));                // free block footer
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));        // new epilogue header

    epilogue = HDRP(NEXT_BLKP(bp)) - WSIZE;

    /* Coalesce if the previous block was free */
    return coalesce(bp);
}

/**********************************************************
 * find_fit
 * Traverse the heap searching for a block to fit asize
 * Return NULL if no free blocks can handle that size
 * Assumed that asize is aligned
 **********************************************************/
void *find_fit(size_t asize)
{
	//Find first list of size >= asize
	int list_entry = get_list_entry(asize);
	void *curr_block;

	//Basic first-fit implementation, and if size found then check if we can split found block to reduce fragmentation
	for(list_entry; list_entry < MAX_LISTS; list_entry++)
	{
		curr_block = segmented_free_list[list_entry];
		while (curr_block != NULL)
		{
			if (GET_SIZE(curr_block) >= asize)
				return split(curr_block, asize);

			curr_block = GET_NEXT_PTR(curr_block);
		}
	}

	return NULL;
}

/**********************************************************
 * place
 * Mark the block as allocated
 **********************************************************/
void place(void* bp, size_t asize)
{
    /* Get the current block size */
    size_t bsize = GET_SIZE(HDRP(bp));

    PUT(HDRP(bp), PACK(bsize, 1));
    PUT(FTRP(bp), PACK(bsize, 1));
}

/**********************************************************
 * mm_free
 * Free the block and coalesce with neighbouring blocks
 **********************************************************/
void mm_free(void *bp)
{
	if(bp == NULL)
      return;
    //Mark block as freed
    size_t size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size,0));
    PUT(FTRP(bp), PACK(size,0));

    bp = coalesce(bp);

    add_to_list(HDRP(bp));
}


/**********************************************************
 * mm_malloc
 * Allocate a block of size bytes.
 * The type of search is determined by find_fit
 * The decision of splitting the block, or not is determined
 *   in place(..)
 * If no block satisfies the request, the heap is extended
 **********************************************************/
void *mm_malloc(size_t size)
{
	size_t asize; /* adjusted block size */
    size_t extendsize; /* amount to extend heap if no fit */
    char *bp;

    /* Ignore spurious requests */
    if (size == 0)
        return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= DSIZE)
        asize = 2 * DSIZE;
    else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);

    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = asize;
    if ((bp = extend_heap(extendsize)) == NULL)
        return NULL;
    place(bp, asize);
    return bp;

}

/**********************************************************
 * mm_realloc
 * Implemented simply in terms of mm_malloc and mm_free
 *********************************************************/
void *mm_realloc(void *ptr, size_t size)
{
    /* If size == 0 then this is just free, and we return NULL. */
    if(size == 0)
    {
      mm_free(ptr);
      return NULL;
    }
    /* If oldptr is NULL, then this is just malloc. */
    if (ptr == NULL)
      return (mm_malloc(size));

    size_t copy_size = GET_SIZE(HDRP(ptr));
    size_t asize;

    /* Adjust requested block size to include overhead and alignment reqs. Same as malloc section */
    if (size <= DSIZE)
        asize = 2 * DSIZE;
    else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);

    /* Check requested allocation size adjusted for overhead and algnment rqs against size of block to see if work needs to be done */
    if (asize < copy_size)
    	return ptr;

    mm_free(ptr);
    void *realloc_ptr = mm_malloc(size * 2);

    if (realloc_ptr == NULL)
        return NULL;

    if (size < copy_size)
        copy_size = size;

    memcpy(realloc_ptr, ptr, copy_size);

    return realloc_ptr;
}

int get_list_entry(int key)
{
    int i = 0;
    int temp = 1;

    //Basic log hashing scheme to determine which segregated list (power of 2) to use
    for (i; i < MAX_LISTS ; i++)
    {
        if (key <= temp)
        	break;

        temp = temp << 1;
    }

    if (i < MAX_LISTS)
    	return i;
   	else
   		return MAX_LISTS - 1;
}

void add_to_list(void *bp)
{
	int list_entry = get_list_entry(GET_SIZE(bp));
	void *old_bp = segmented_free_list[list_entry];
	segmented_free_list[list_entry] = bp;

	//Insert new block to head of list
	SET_NEXT_PTR(bp, old_bp);
	SET_PREV_PTR(bp, NULL);

	if (old_bp != NULL)
		SET_PREV_PTR(old_bp, bp);
}

void remove_from_list(void *bp)
{
	uintptr_t removed_next_ptr = GET_NEXT_PTR(bp);
	uintptr_t removed_prev_ptr = GET_PREV_PTR(bp);

	if (removed_next_ptr != NULL)
		SET_PREV_PTR(removed_next_ptr, removed_prev_ptr);
	if (removed_prev_ptr != NULL)
		SET_NEXT_PTR(removed_prev_ptr, removed_next_ptr);
	else
	{
		int list_entry = get_list_entry(GET_SIZE(bp));
		segmented_free_list[list_entry] = removed_next_ptr;
	}
}

/**********************************************************
 * mm_check
 * Check the consistency of the memory heap
 * Return nonzero if the heap is consistant.
 *********************************************************/
int mm_check(void){
  return 1;
}
