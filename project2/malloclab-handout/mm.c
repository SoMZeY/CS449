/**_**_*****_*_*********************
 *   Student ID: lun8              *
 *   Student name: Luis Oliveira   *
 ***********************************/

/*-------------------------------------------------------------------
 *
 *  Malloc Project Starter code:
 *        A (bad) malloc - always fails!
 *
 * Project stage 1:
 *        A malloc - modify the code to only request more memory
 *            if we don't have free blocks that can fit the request.
 *            We need to free! And keep track of the memory blocks.
 *            Implement a data structure on each block that allows you
 *            to traverse the list. Search free blocks. And be more
 *            efficient. Don't split or coalesce blocks!
 *
 * Project stage 2:
 *        A more efficient malloc - modify the code to split blocks
 *            when overallocation is imminent. Don't forget to write
 *            new metadata, and to update existing metadata to match
 *            the new state of the linked list. When freeing, coalesce
 *            blocks. If freeing creates a sequence of 2 or 3 free
 *            blocks, merge them into a single one; again, modify all the
 *            relevant metadata to make the linked list consistent
 *
 * Project stage 3:
 *        A faster malloc - modify the code to keep track of a second
 *            linked-list composed of free blocks. To do that, we take
 *            some of the block space reserved for program allocations
 *            but ONLY when the block is free! This is a more traditional
 *            linked list where nodes can be in any order, and thus
 *            requires an explicit node.
*
 *-------------------------------------------------------------------- */

// C libraries for C things
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

// This file provides the memory management functions we need for our implementation
#include "memlib.h"
// This file just contains some function declarations
#include "mm.h"

/* Macros for unscaled pointer arithmetic to keep other code cleaner.
     Casting to a char* has the effect that pointer arithmetic happens at
     the byte granularity (i.e. POINTER_ADD(0x1, 1) would be 0x2).    (By
     default, incrementing a pointer in C has the effect of incrementing
     it by the size of the type to which it points (e.g. Block).)
     We cast the result to void* to force you to cast back to the
     appropriate type and ensure you don't accidentally use the resulting
     pointer as a char* implicitly. Also avoids compiler complaints.
*/
#define UNSCALED_POINTER_ADD(p, x) ((void *)((char *)(p) + (x)))
#define UNSCALED_POINTER_SUB(p, x) ((void *)((char *)(p) - (x)))

/**********************************************************************
 *
 * For a more detailed explanation of these structs, check file mman.h
 * and the project handout
 *
 ***********************************************************************/
typedef struct _BlockInfo
{
    long int size;          // Size of block
    struct _Block *prev;    // Explicit pointer to previous Block
} BlockInfo_t;

typedef struct _FreeBlockInfo
{
    struct _Block *nextFree;    // Explicit pointer to next free Block (stage 3)
    struct _Block *prevFree;    // Explicit pointer to previous free Block (stage 3)
} FreeBlockInfo_t;

typedef struct _Block
{
    BlockInfo_t info;           // Composing both infos into a single struct
    FreeBlockInfo_t freeNode;   //  Think: What does this mean in terms of memory?
} Block_t;

typedef struct
{
    Block_t *malloc_list_tail;  // Use keep track of the tail node
    Block_t *free_list_head;    // Pointer to the first FreeBlockInfo_t in the free list, use in stage 3.
} malloc_info_t;

/* Variable to keep malloc information tidy */
static malloc_info_t malloc_info = {
    .free_list_head = NULL,
    .malloc_list_tail = NULL,
};

/* Alignment of blocks returned by mm_malloc.
 * We'll align to 16 bytes to ensure proper alignment for FreeBlockInfo_t.
 */
#define ALIGNMENT 16
#define align_size(size) (((size) + (ALIGNMENT - 1)) & ~0xF)

/* Overhead size of each block, includes BlockInfo_t and FreeBlockInfo_t */
#define OVERHEAD (sizeof(Block_t))

/* Minimum block size to hold overhead and alignment */
#define MIN_BLOCK_SIZE (OVERHEAD)

/************************************************************************
 * Memory management/navigation functions
 ************************************************************************/

// Declarations
Block_t *first_block();
Block_t *next_block(Block_t *block);
void *request_more_space(size_t request_size);
size_t heap_size();

// Function prototypes for Stage 3 functions
void insert_free_node(Block_t *block);
void remove_free_node(Block_t *block);

// Helper functions

/**
 * This function should get the first block or returns NULL if there is not one.
 * You can use this to start your through search for a block.
*/
Block_t *first_block()
{
    void *firstHeapByte = mem_heap_lo();

    if (mem_heapsize() == 0)
    {
        return NULL;
    }

    return (Block_t *)firstHeapByte;
}

/**
 * This function will get the adjacent block or returns NULL if there is not one.
 * You can use this to move along your malloc list one block at a time.
*/
Block_t *next_block(Block_t *block)
{
    if (block == NULL)
    {
        return NULL;
    }

    size_t blockSize = (block->info.size < 0) ? -block->info.size : block->info.size;

    // Move to the next block by adding the current block's total size
    Block_t *next = (Block_t *)UNSCALED_POINTER_ADD(block, OVERHEAD + blockSize);

    // Check if we've reached the end of the heap
    if ((void *)next >= mem_heap_hi() || (void *)next < mem_heap_lo())
    {
        return NULL;
    }

    return next;
}

/**
 * Requests more space from the OS and adds it to the heap.
 */
void *request_more_space(size_t request_size)
{
    // Total size includes payload and overhead
    size_t sizeTotal = request_size + OVERHEAD;

    // Align the total size
    sizeTotal = align_size(sizeTotal);

    // Use mem_sbrk to get more memory
    void *ret = mem_sbrk(sizeTotal);
    if (ret == (void *)-1)
    {
        // mem_sbrk failed
        return NULL;
    }

    // Initialize the new block's metadata
    Block_t *newBlock = (Block_t *)ret;
    newBlock->info.size = 0; // Initially free
    newBlock->info.prev = malloc_info.malloc_list_tail;

    // Update malloc_list_tail
    malloc_info.malloc_list_tail = newBlock;

    return newBlock;
}

/* Returns the size of the heap */
size_t heap_size()
{
    return mem_heapsize();
}

/************************************************************************
 * Heap management/navigation functions
 ************************************************************************/

/******************************* Stage 0 ********************************/
/* Find a free block of at least the requested size in the heap.
   Returns NULL if no free block is large enough. */
Block_t *search_list(size_t request_size)
{
    Block_t *ptr_block = first_block();

    while (ptr_block != NULL)
    {
        if (ptr_block->info.size < 0 && (-ptr_block->info.size) >= request_size)
        {
            // Suitable free block found
            return ptr_block;
        }

        ptr_block = next_block(ptr_block);
    }

    // No suitable block found
    return NULL;
}

/******************************* Stage 2 ********************************/
/* Shrink block to size, and create a new block with remaining space. */
// Not implemented in Stage 0
void split(Block_t *block, size_t size)
{
    // Stage 0 does not implement splitting
}

/* Merge together consecutive free blocks. */
// Not implemented in Stage 0
void coalesce(Block_t *block)
{
    // Stage 0 does not implement coalescing
}

/******************************* Stage 3 ********************************/
/* Insert free block into the free_list.
   Not used in Stage 0 */
void insert_free_node(Block_t *block)
{
    // Stage 0 does not use an explicit free list
}

/* Remove free block from the free_list.
   Not used in Stage 0 */
void remove_free_node(Block_t *block)
{
    // Stage 0 does not use an explicit free list
}

/******************************* Stage 3 ********************************/
/* Find a free block of at least the requested size in the free list.
   Returns NULL if no free block is large enough. */
// Not used in Stage 0
Block_t *search_free_list(size_t request_size)
{
    // Stage 0 does not use an explicit free list
    return NULL;
}

// TOP-LEVEL ALLOCATOR INTERFACE ------------------------------------

/* Initialize the allocator. */
int mm_init()
{
    // Initialize the malloc_info structure
    malloc_info.free_list_head = NULL;
    malloc_info.malloc_list_tail = NULL;

    return 0;
}

/* Allocate a block of size bytes and return a pointer to it. If size is zero,
   returns NULL.
 */
void *mm_malloc(size_t size)
{
    if (size == 0)
    {
        return NULL;
    }

    size_t request_size = align_size(size);

    // Stage 0: Use search_list instead of search_free_list
    Block_t *ptr_free_block = search_list(request_size);

    if (ptr_free_block == NULL)
    {
        // No suitable block found; request more space
        ptr_free_block = request_more_space(request_size);
        if (ptr_free_block == NULL)
        {
            // request_more_space failed
            return NULL;
        }

        // Initialize the new block as allocated
        ptr_free_block->info.size = request_size;
        ptr_free_block->info.prev = malloc_info.malloc_list_tail;
    }
    else
    {
        // Stage 0: Do not split the block
        // Mark the block as allocated by setting size positive
        ptr_free_block->info.size = ptr_free_block->info.size > 0 ? ptr_free_block->info.size : -ptr_free_block->info.size;
    }

    // Update malloc_list_tail if necessary
    if (malloc_info.malloc_list_tail != ptr_free_block)
    {
        malloc_info.malloc_list_tail = ptr_free_block;
    }

    // Return a pointer to the payload area
    return UNSCALED_POINTER_ADD(ptr_free_block, OVERHEAD);
}

/* Free the block referenced by ptr. */
void mm_free(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }

    Block_t *block = (Block_t *)UNSCALED_POINTER_SUB(ptr, OVERHEAD);

    // Stage 0: Mark the block as free by setting size negative
    if (block->info.size > 0)
    {
        block->info.size = -block->info.size;
    }

    // Stage 0: Do not insert into free list or coalesce
}

/**********************************************************************
 * PROVIDED FUNCTIONS
 *
 * You do not need to modify these, but they might be helpful to read
 * over.
 *
 * Note: They will need you to correctly implement functions
 *     first_block and heap_size
 **********************************************************************/

/* Print the heap by iterating through it as an implicit free list. */
void examine_heap()
{
    /* print to stderr so output isn't buffered and not output if we crash */
    Block_t *curr = (Block_t *)first_block();
    Block_t *end = (Block_t *)UNSCALED_POINTER_ADD(first_block(), heap_size());
    fprintf(stderr, "====================================================\n");
    fprintf(stderr, "heap size:\t0x%lx\n", heap_size());
    fprintf(stderr, "heap start:\t%p\n", curr);
    fprintf(stderr, "heap end:\t%p\n", end);

    fprintf(stderr, "free_list_head: %p\n", (void *)malloc_info.free_list_head);

    fprintf(stderr, "malloc_list_tail: %p\n", (void *)malloc_info.malloc_list_tail);

    while (curr && curr < end)
    {
        /* print out common block attributes */
        fprintf(stderr, "%p: %ld\t", (void *)curr, curr->info.size);

        /* and allocated/free specific data */
        if (curr->info.size > 0)
        {
            fprintf(stderr, "ALLOCATED\tprev: %p\n", (void *)curr->info.prev);
        }
        else
        {
            fprintf(stderr, "FREE\tnextFree: %p, prevFree: %p, prev: %p\n", (void *)curr->freeNode.nextFree, (void *)curr->freeNode.prevFree, (void *)curr->info.prev);
        }

        curr = next_block(curr);
    }
    fprintf(stderr, "END OF HEAP\n\n");

    curr = malloc_info.free_list_head;
    fprintf(stderr, "Head ");
    while (curr)
    {
        fprintf(stderr, "-> %p ", curr);
        curr = curr->freeNode.nextFree;
    }
    fprintf(stderr, "\n");
}

/* Checks the heap data structure for consistency. */
int check_heap()
{
    Block_t *curr = (Block_t *)first_block();
    Block_t *end = (Block_t *)UNSCALED_POINTER_ADD(first_block(), heap_size());
    Block_t *last = NULL;
    long int free_count = 0;

    while (curr && curr < end)
    {
        if (curr->info.prev != last)
        {
            fprintf(stderr, "check_heap: Error: previous link not correct.\n");
            examine_heap();
        }

        if (curr->info.size <= 0)
        {
            // Free
            free_count++;
        }

        last = curr;
        curr = next_block(curr);
    }

    curr = malloc_info.free_list_head;
    last = NULL;
    while (curr)
    {
        if (curr == last)
        {
            fprintf(stderr, "check_heap: Error: free list is circular.\n");
            examine_heap();
        }
        last = curr;
        curr = curr->freeNode.nextFree;
        if (free_count == 0)
        {
            fprintf(stderr, "check_heap: Error: free list has more items than expected.\n");
            examine_heap();
        }
        free_count--;
    }

    return 0;
}
