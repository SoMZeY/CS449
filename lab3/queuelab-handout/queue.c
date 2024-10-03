/* >​< >‌‌‌<
 * Developed by R. E. Bryant, 2017
 * Extended to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if (!q)
    {
      return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
    {
      return;
    }

    // Mark current list element
    list_ele_t* currentElement = q->head;
    list_ele_t* nextElement;

    // Loop over each linked list
    while (currentElement)
    {
      // Update the next element, even if its a tail and its null
      nextElement = currentElement->next;

      // Free the string and the element
      free(currentElement->value);
      free(currentElement);

      // Update the current element after the freeing
      currentElement = nextElement;
    }

    /* Free queue structure */
    free(q);
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    /* What should you do if the q is NULL? */
    if (!q || !s)
    {
      return false;
    }

    list_ele_t *newh = malloc(sizeof(list_ele_t));

    if (!newh)
    {
      return false;
    }
    /* Don't forget to allocate space for the string and copy it */
    char *str = malloc((strlen(s) + 1) * sizeof(char));

    if (!str)
    {
      free(newh);
      return false;
    }

    // Copy the string
    strcpy(str, s);

    /* You must do the cleanup of anything left behind */
    newh->value = str;
    newh->next = q->head;
    q->head = newh;

    if (q->size == 0)
    {
      q->tail = newh;
    }

    // After everything is done update the size
    q->size++;

    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    if (!q || !s)
    {
      return false;
    }

    list_ele_t* newt = malloc(sizeof(list_ele_t));

    if (!newt)
    {
      return false;
    }

    char* str = malloc((strlen(s) + 1) * sizeof(char));

    if (!str)
    {
      free(newt);
      return false;
    }

    strcpy(str, s);
    newt->next = NULL; // Even though this might be unnecessary make that next of the new tail is NULL
    newt->value = str;

    if (!q->head)
    {
      q->head = newt;
      q->tail = newt;
    }
    else 
    {
      q->tail->next = newt;
      q->tail = newt;
    }

    // After everything is completed, update the size
    q->size++;

    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
    {
      return false;
    }

    /* You need to fix up this code. */\      
    // Copy the string
    if (sp)
    {
      strncpy(sp, q->head->value, bufsize - 1);        
      sp[bufsize - 1] = '\0';
    }
    
    // Save the next element
    list_ele_t* nextHeader = q->head->next;

    // Remove current header
    free(q->head->value);
    free(q->head);

    q->head = nextHeader;
    
    // If head was the only element, make tail false
    if (!q->head)
    {
      q->tail = NULL;
    }

    // After everything is completed, update the size of the queue
    q->size--;

    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    if(!q)
    {
      return 0;
    }

    return q->size;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head || !q->head->next)
    {
        // No need to reverse if queue is NULL, empty, or has only one element
        return;
    }

    list_ele_t *prev = NULL;
    list_ele_t *current = q->head;
    list_ele_t *next = NULL;

    // Update tail to be the current head (will become the tail after reversal)
    q->tail = q->head;

    // Reverse
    while (current != NULL)
    {
        next = current->next;   // Store the next node
        current->next = prev;   // Reverse the link
        prev = current;         // Move prev to current
        current = next;         // Move to next node
    }

    q->head = prev;
}

