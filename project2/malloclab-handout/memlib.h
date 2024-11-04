#include <unistd.h>

/*
 * mem_sbrk - simple model of the sbrk function. Extends the heap
 *    by incr bytes and returns the start address of the new area. In
 *    this model, the heap cannot be shrunk.
 */
void *mem_sbrk(size_t incr);

/**************************************************************************/
/**  You don't necessarily need to use the functions below this comment! **/
/**************************************************************************/

/*
 * mem_heap_lo - return address of the first heap byte
 */
void *mem_heap_lo();

/*
 * mem_heap_hi - return address of last heap byte
 */
void *mem_heap_hi();
/*
 * mem_heapsize() - returns the heap size in bytes
 */
size_t mem_heapsize();

/*
 * mem_pagesize() - returns the page size of the system
 */
size_t mem_pagesize();

/*****************************************************************/
/**  You will not need to use the functions below this comment! **/
/*****************************************************************/

/*
 * mem_init - initialize the memory system model
 */
void mem_init(void);

/*
 * mem_deinit - free the storage used by the memory system model
 */
void mem_deinit(void);

/*
 * mem_reset_brk - reset the simulated brk pointer to make an empty heap
 */
void mem_reset_brk(void);
