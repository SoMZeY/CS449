/***********************************
 *   Student ID: lun8              *
 *   Student name: Luis Oliveira   *
 ***********************************/

/*-------------------------------------------------------------------
 *
 *  Malloc Project Corrected Code
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
    long int size;          // Size of payload
    struct _Block *prev;    // Pointer to previous block
} BlockInfo_t;

typedef struct _FreeBlockInfo
{
    struct _Block *nextFree;    // Pointer to next free block
    struct _Block *prevFree;    // Pointer to previous free block
} FreeBlockInfo_t;

typedef struct _Block
{
    BlockInfo_t info;           // Block header
    FreeBlockInfo_t freeNode;   // Free list pointers (used only when block is free)
    // Payload starts immediately after this struct
} Block_t;

typedef struct
{
    Block_t *malloc_list_tail;  // Pointer to the last block in the heap
    Block_t *free_list_head;    // Pointer to the first block in the free list
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
 * Returns the first block in the heap or NULL if the heap is uninitialized.
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
 * Returns the block immediately after the given block or NULL if there is none.
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
    newBlock->info.size = request_size; // Mark as allocated
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

/******************************* Stage 1 ********************************/
/* Find a free block of at least the requested size in the heap.
   Returns NULL if no free block is large enough. */
Block_t *search_list(size_t request_size)
{
    Block_t *ptr_free_block = first_block();

    while (ptr_free_block != NULL)
    {
        if (ptr_free_block->info.size < 0 && (-ptr_free_block->info.size) >= request_size)
        {
            // Suitable free block found
            return ptr_free_block;
        }

        ptr_free_block = next_block(ptr_free_block);
    }

    // No suitable block found
    return NULL;
}

/******************************* Stage 2 ********************************/
/* Shrink block to size, and create a new block with remaining space. */
void split(Block_t *block, size_t size)
{
    size_t blockSize = -block->info.size; // Current block's payload size

    size_t remaining_payload_size = blockSize - size - OVERHEAD;

    if (remaining_payload_size >= MIN_BLOCK_SIZE)
    {
        // Set the size of the allocated block
        block->info.size = size;

        // Get the address of the new free block after splitting
        Block_t *newBlock = (Block_t *)UNSCALED_POINTER_ADD(block, OVERHEAD + size);

        // Set up the new free block
        newBlock->info.size = -((long int)(remaining_payload_size));
        newBlock->info.prev = block;

        // Update next block's prev pointer
        Block_t *next = next_block(newBlock);
        if (next != NULL)
        {
            next->info.prev = newBlock;
        }
        else
        {
            // Update malloc_list_tail if needed
            malloc_info.malloc_list_tail = newBlock;
        }

        // Insert the new free block into the free list
        insert_free_node(newBlock);
    }
    else
    {
        // Not enough space to split; allocate the entire block
        block->info.size = -block->info.size; // Mark as allocated
        // Remove the block from the free list
        remove_free_node(block);
    }
}

/* Merge together consecutive free blocks. */
void coalesce(Block_t *block)
{
    Block_t *prevBlock = block->info.prev;
    Block_t *nextBlock = next_block(block);

    // Remove current block from free list
    remove_free_node(block);

    size_t totalSize = -block->info.size; // Payload size of the current block

    // Coalesce with previous block if it's free
    if (prevBlock != NULL && prevBlock->info.size < 0)
    {
        remove_free_node(prevBlock);
        totalSize += OVERHEAD + (-prevBlock->info.size); // Add size of previous block
        prevBlock->info.size = -((long int)totalSize);
        block = prevBlock;
    }
    else
    {
        block->info.size = -((long int)totalSize);
    }

    // Coalesce with next block if it's free
    if (nextBlock != NULL && nextBlock->info.size < 0)
    {
        remove_free_node(nextBlock);
        totalSize += OVERHEAD + (-nextBlock->info.size); // Add size of next block
        block->info.size = -((long int)totalSize);

        // Update the next block's prev pointer
        Block_t *nextNextBlock = next_block(nextBlock);
        if (nextNextBlock != NULL)
        {
            nextNextBlock->info.prev = block;
        }
        else
        {
            // Update malloc_list_tail if needed
            malloc_info.malloc_list_tail = block;
        }
    }

    // Insert the coalesced block back into the free list
    insert_free_node(block);
}

/******************************* Stage 3 ********************************/
/* Insert free block into the free_list. */
void insert_free_node(Block_t *block)
{
    block->freeNode.nextFree = malloc_info.free_list_head;
    block->freeNode.prevFree = NULL;

    if (malloc_info.free_list_head != NULL)
    {
        malloc_info.free_list_head->freeNode.prevFree = block;
    }

    malloc_info.free_list_head = block;
}

/* Remove free block from the free_list. */
void remove_free_node(Block_t *block)
{
    if (block->freeNode.prevFree != NULL)
    {
        block->freeNode.prevFree->freeNode.nextFree = block->freeNode.nextFree;
    }
    else
    {
        malloc_info.free_list_head = block->freeNode.nextFree;
    }

    if (block->freeNode.nextFree != NULL)
    {
        block->freeNode.nextFree->freeNode.prevFree = block->freeNode.prevFree;
    }

    // Clear the pointers for safety
    block->freeNode.nextFree = NULL;
    block->freeNode.prevFree = NULL;
}

/* Find a free block of at least the requested size in the free list.
   Returns NULL if no free block is large enough. */
Block_t *search_free_list(size_t request_size)
{
    Block_t *ptr_free_block = malloc_info.free_list_head;

    while (ptr_free_block != NULL)
    {
        size_t block_size = -ptr_free_block->info.size;

        if (block_size >= request_size)
        {
            return ptr_free_block;
        }

        ptr_free_block = ptr_free_block->freeNode.nextFree;
    }

    return NULL;
}

/* TOP-LEVEL ALLOCATOR INTERFACE ------------------------------------ */

/* Initialize the allocator. */
int mm_init()
{
    // Initialize the malloc_info structure
    malloc_info.free_list_head = NULL;
    malloc_info.malloc_list_tail = NULL;

    // Optionally, you can initialize the heap here if needed
    // For example, requesting an initial block

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

    // Search for a suitable free block using the free list
    Block_t *ptr_free_block = search_free_list(request_size);

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
    }
    else
    {
        // Split the block if possible
        split(ptr_free_block, request_size);
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

    // Mark the block as free
    if (block->info.size > 0)
    {
        block->info.size = -block->info.size;
    }

    // Insert the block into the free list
    insert_free_node(block);

    // Coalesce with adjacent free blocks
    coalesce(block);
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
            fprintf(stderr, "FREE\tnextFree: %p, prevFree: %p, prev: %p\n",
                    (void *)curr->freeNode.nextFree,
                    (void *)curr->freeNode.prevFree,
                    (void *)curr->info.prev);
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
            return -1;
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
            return -1;
        }
        last = curr;
        curr = curr->freeNode.nextFree;
        if (free_count == 0)
        {
            fprintf(stderr, "check_heap: Error: free list has more items than expected.\n");
            examine_heap();
            return -1;
        }
        free_count--;
    }

    if (free_count != 0)
    {
        fprintf(stderr, "check_heap: Error: free list has fewer items than expected.\n");
        examine_heap();
        return -1;
    }

    return 0;
}
