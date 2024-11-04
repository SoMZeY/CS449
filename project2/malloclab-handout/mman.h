#if 0
/************************* LIST IMPLEMENTATION *************************/
/* A BlockInfo_t contains information about a block, including the size
    as well as pointers to the previous block in the linked list.
    This is similar to the "implicit list" structure discussed in
    the lecture.
*/
typedef struct _BlockInfo
{
    // Size of the block and whether or not the block is in use or free.
    // When the size is negative, the block is currently free.
    long int size;
    // Pointer to the previous block in the list.
    struct _Block *prev;
} BlockInfo_t;

/* This is what an allocated node looks like!
    All nodes contain information about the size and previous
      element in the list

    +--------------+
    |     size     |  <-  Block pointers point here
    |              |
    |   (header)   |
    |              |
    |     prev     |
    +--------------+
    |              |  <-  Pointers returned by mm_malloc point here
    |              |
    |  space and   |
    |   padding    |
    |     ...      |
    |     ...      |
    +--------------+
*/

/******** FREE LIST IMPLEMENTATION ***********************************/

/* A FreeBlockInfo_t structure contains metadata just for free blocks.
 * When you are ready, you can improve your naive implementation by
 * using these to maintain a separate list of free blocks.
 *
 * These are "kept" in the region of memory that is normally used by
 * the program when the block is allocated. That is, since that space
 * is free anyway, we can make good use of it to improve our malloc.
 */
typedef struct _FreeBlockInfo
{
    // Pointer to the next free block in the list.
    struct _Block *next_free;
    // Pointer to the previous free block in the list.
    struct _Block *prev_free;
} FreeBlockInfo_t;

/* A Free Block, in addition to the header present in ALL blocks also
   contains information about a separate linked-list, Namely, pointers
   to the next and previous blocks in the free list.
   This is similar to the "explicit free list" structure illustrated in
   the lecture slides.

   Note that the FreeBlockInfo_t is only needed when the block is free. To
   achieve better utilization, mm_malloc should use the space as part of
   the space it returns to the program.

   +--------------+
   |     size     |  <-  Block pointers in free list point here
   |              |
   |   (header)   |
   |              |
   |     prev     |
   +--------------+
   |   next_free  |  <-  Pointers returned by mm_malloc point here
   |   prev_free  |
   +--------------+      (allocated blocks do not have a 'next_free' field)
   |  space and   |      (this is a space optimization...)
   |   padding    |
   |     ...      |      Free blocks write their next_free/prev_free pointers in
   |     ...      |      this space.
   +--------------+

*/

/* This is a structure that can serve as all kinds of nodes.
  Don't forget that the freeNode cannot be accessed if the node is not free!
 */
typedef struct _Block
{
    BlockInfo_t info;
    FreeBlockInfo_t free_node;
} Block_t;

#endif
