#include <stdio.h>

/**
 * All these functions are called by the tester
 *   (they are the basis of malloc)
 **/
extern int mm_init(void);
extern void *mm_malloc(size_t size);
extern void mm_free(void *ptr);

/**
 * This function helps with detecting errors on the heap
 *   (also called by the tester)
 **/
extern void examine_heap();
extern int check_heap();
