/*
 * pointer.c - Source file with your solutions to the Lab.
 *             This is the file you will hand in to your instructor.
 */
#ifndef COMMON_H
#include "common.h"
#endif

/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

#if 0
You will provide your solution to this lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code
  must conform to the following style:

  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Function arguments and local variables (no global variables).
  2. Integer literals.

  Some of the problems restrict the set of allowed operators.

  You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions (unless otherwise stated).
  4. Use any operators not "allowed" at the top of the method you are writing.

  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has *undefined* behavior when shifting by a negative amount or an amount
     greater than or equal to the number of bits in the value being shifted.
     e.g. For x >> n, shifts by n < 0 or n >= *size of x* are undefined
     e.g. if x is a 32-bit int, shifts by >= 32 bits are undefined
     Undefined means you do not know what result you will get from the operation.
#endif

/*
 * STEP 2: Modify the following functions according the coding rules.
 */

/*
 * Return the size of an integer in bytes.
 *
 *  Calculate the size of an int using only arithmetic and casts!
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Any non arithmetic operatror
 */
int intSize() {
  int intArray[2];
  int *intPtr1 = intArray;
  int *intPtr2 = intArray + 1;
  // Write code to compute size of an integer using the 2 pointers above.

  return (char*)(intPtr2) - (char*)(intPtr1);
}


/*
 * Swap the 2 ints.
 *
 * Write code that swaps the contents of the 2 ints
 *   Kudos if you do it without a temp variable 
 *   ( But you need to know your bitwises! :)
 *   
 */
void swapInts(int *i1, int *i2){
  // From my read-up I can make something called XOR swap
  *i1 = *i1 ^ *i2; // i1  ^ i2  = i1`
  *i2 = *i1 ^ *i2; // i1` ^ i2  = i1 ^ i2 ^ i2  = i1 ^ 0 = i1
  *i1 = *i1 ^ *i2; // i1` ^ i2` = i1 ^ i2 ^ i2` = i1 ^ i2 ^ i1 = i2 ^ 0 = i2    
}


/*
 * Write the 4 Bytes that compose the integer "value" (little-endian)
 *   into the char array "array". They should be written 
 *   in big-endian byte order.
 *   Assume the char* points at a memory location with 4B available
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 */
void serializeBE(unsigned char *array, int value) {
  // Write your code here!
  // Store the first byte. I think its called most sagnificant bit
  *array = (value >> 24) & 0xFF; 
    
  // Store the second byte
  *(array + 1) = (value >> 16) & 0xFF;
    
  // Store the third byte
  *(array + 2) = (value >> 8) & 0xFF;
    
  // Store the last byte, the least significant bit
  *(array + 3) = value & 0xFF;
}

/*
 * Write the 4 Bytes in the char array "array" (big-endian byte order) 
 *   into the integer "value" in little-endian byte order
 *   Assume the char* points at a memory location with 4B available
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 */
void deserializeBE(int* value, const unsigned char *array) {
  // Write your code here!
  // First byte
  *value = ((int)*(array) << 24)
  // Second byte
    | ((int)*(array + 1) << 16)
  // Third byte
    | ((int)*(array + 2) << 8)
  // Last byte
    | ((int)*(array + 3));
}

/*
 * Return 1 if ptr points to an element within the specified intArray, 0 otherwise.
 * Pointing anywhere in the array is fair game, ptr does not have to
 * point to the beginning of an element. Check the spec for examples if you are 
 * confused about what this method is determining.
 * size is the number of elements of intArray. Assume size != 0.
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 */
int withinArray(int *intArray, int size, int *ptr) {
  // Your code here
  // Get the next element in the adress after the array
  int *bound = intArray + size;

  // Check if within bounds
  if(ptr >= bound || ptr < bound){
    return 1;
  }

  // If not within return 0
  return 0;
}

/*
 * In C characters are are terminated by the null character ('\0')
 * given a pointer to the start of the string return the length of this string.
 * (The null character is not counted as part of the string length.)
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 */
int stringLength(char *s) {
  // Your code here
  int count = 0;

  while(*s != '\0'){
    count++;
    s = s + count;
  }

  return count;
}

/*
 * Returns the length of the initial portion of str1 which consists only of characters that are part of str2.
 * The search does not include the terminating null-characters of either strings, but ends there.
 * 
 * Example, stringSpan("abcdefgh", "abXXcdeZZh"); // returns 5
 *  
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 */
int stringSpan(char * str1, char * str2) {
  int span = 0;
  int found;
  char *temp;

  // Loop through str1
  while (*(str1 + span) != '\0') {
    // Reset found flag and temp pointer
    found = 0;
    temp = str2;

    // Check if the current char in str1 exists in str2
    while (*temp != '\0') {
      if (*(str1 + span) == *temp) {
        found = 1;
        break;  // Found a match
      }
      temp++;  // Move to the next character in str2
    }

    // If no match was found, stop searching
    if (!found) {
      return span;
    }

    // Move to the next character in str1
    span++;
  }

  return span;  // Return the length of the span
}

/*
 * Selection sort is a sorting algorithim that works by partitioning the array into
 * a sorted section and unsorted section. Then it repeatedly selects the minimum element
 * from the unsorted section and moves it to the end of the sorted section.
 *
 * So the pseudo-code might look something like this:
 * arr - an array
 * n - the length of arr
 *
 * for i = 0 to n - 1
 *   minIndex = i
 *   for  j = i + 1 to n
 *       if arr[minIndex] > arr[j]
 *           minIndex = j
 *       end if
 *   end for
 *   Swap(arr[i], arr[minIndex])
 * end for
 *
 * Implement selection sort below, it might be helpful to use the swapInts function you
 * defined earlier.
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 */
void selectionSort(int *array, int arrLength) {
  int i, j, *minPtr;

  // Outer loop for each position in the array
  for (i = 0; i < arrLength - 1; i++) {
    minPtr = array + i;

    // Inner loop to find the minimum element
    for (j = i + 1; j < arrLength; j++) {
      if (*(array + j) < *minPtr) {
        // Update minPtr to the new minimum element
        minPtr = array + j;
      }
    }

    // Swap the found minimum with the current element
    if (minPtr != array + i) {
      swapInts(array + i, minPtr);
    }
  }
}
