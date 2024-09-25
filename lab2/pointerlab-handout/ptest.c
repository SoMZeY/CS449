/*
 * Test harness for pointer.c
 *
 */

#include <stdio.h>
#include "pointer.h"
#include "common.h"

typedef struct test_s {
    char *function_name;
    int (*function_under_test)();
    int expected_result;
} test;

typedef struct withinArray_args_s {
    char * function_name;
    int * base;
    int size;
    int * ptr;
    int expected_result;
} withinArray_args;

typedef struct swapInts_args_s {
    char * function_name;
    int * ptr1;
    int * ptr2;
} swapInts_args;

typedef struct stringLength_args_s {
    char * function_name;
    char * s;
    int expected_result;
} stringLength_args;

typedef struct stringSpan_args_s {
    char * function_name;
    char * str1;
    char * str2;
    int expected_result;
} stringSpan_args;

typedef struct serializationExperiment_args_s {
    char * function_name;
    unsigned char * a;
    int v;
    unsigned char * expected_result;
} serializationExperiment_args;

typedef struct deserializationExperiment_args_s {
    char * function_name;
    int *v;
    unsigned char *a;
    int expected_result;
} deserializationExperiment_args;

typedef struct selectionSort_args_s {
    char * function_name;
    int * arr;
    int * arr_copy;
    int arrLength;
    int * expected_result;
} selectionSort_args;

static test tests[] = {
    { "intSize()",      intSize,        sizeof(int) },
    { NULL, NULL, 0 }
};

static int int1 = 123;
static int int2 = 4324;
static swapInts_args swap_args[] = {
  { "swapInts", &int1, &int2 },
  { NULL, NULL, NULL }
};


static unsigned char BEarr_solution[4]={0};
static unsigned char BEarr_expected[4]={0x12, 0xAD, 0xFE, 0x34};
static serializationExperiment_args serializationexperiment_args[] = {
  { "serializeBE" , BEarr_solution, 0x12ADFE34 , BEarr_expected},
  { NULL, NULL, 0, 0 }
};

static unsigned char BEarr[4] = {0x12, 0xFD, 0x54, 0x44};
static int val;
static deserializationExperiment_args deserializationexperiment_args[] = {
  { "deserializeBE" , &val, BEarr , 0x12FD5444},
  { NULL, NULL, 0, 0 }
};



static withinArray_args arr_args[] = {
  {"withinArray", (int *) 0x35, 4, (int *) 1, 0}, 
  {"withinArray", (int *) 0x35, 4, (int *) 0x34, 0}, 
  {"withinArray", (int *) 0x35, 4, (int *) 0x35, 1}, 
  {"withinArray", (int *) 0x35, 10, (int *) 0x50, 1}, 
  {"withinArray", (int *) 0x0, 4, (int *) 0x0F, 1}, 
  {"withinArray", (int *) 0x0, 4, (int *) 0x10, 0}, 
  {"withinArray", (int *) 0x0, 4, (int *) 0x20, 0}, 
  {"withinArray", (int *) 0x14, 4, (int *) 0xd, 0}, 
  {NULL, NULL, 0, NULL, 0}
};

static char string1[] = "hello";
static char string2[] = "hello world";
static char string3[] = "";
static stringLength_args strlen_args[] = {
  { "stringLength", (char*)string1, 5 },
  { "stringLength", (char*)string2, 11 },
  { "stringLength", (char*)string3, 0 },
  { NULL, NULL, 0 }
};

static stringSpan_args strspn_args[] = {
  { "stringSpan", "abcde", "ac", 1 },
  { "stringSpan", "123456", "ab", 0 },
  { "stringSpan", "hello", "hel", 4 },
  { "stringSpan", "abcdefgh", "abXXcdeZZh", 5 },
  { NULL, NULL, 0 }
};


static int arr1[] = {2, 1};
static int arr1_copy[] = {2, 1};
static int expected_arr1[] = {1, 2};
static int arr2[] = {5, 2, 4, 3, 1};
static int arr2_copy[] = {5, 2, 4, 3, 1};
static int expected_arr2[] = {1, 2, 3, 4, 5};
static selectionSort_args selSort_args[] = {
  { "selectionSort" , arr1 , arr1_copy , 2, expected_arr1 },
  { "selectionSort" , arr2 , arr2_copy , 5, expected_arr2 },
  { NULL, NULL, 0 }
};

int main(int argc, char **argv) {
    int result;
    int score = 0;
    test * t;

    for (t = tests; t->function_name; t++) {
        result = t->function_under_test();

        printf("%s = %d", t->function_name, result);

        if (result == t->expected_result) {
            printf("\t[ OK ]\n");
            score ++;
        } else {
            printf("\t[ fail ] (expected %d)\n", t->expected_result);
        }
    }


    swapInts_args * args1;

    for(args1 = swap_args; args1->function_name != NULL; args1++) {
      int ptr1PrevVal = *(args1->ptr1);
      int ptr2PrevVal = *(args1->ptr2);
      printf("%s(*ptr1=%d, *ptr2=%d) = ", args1->function_name, *args1->ptr1, *args1->ptr2);
      swapInts(args1->ptr1, args1->ptr2);
      printf("*ptr1=%d, *ptr2=%d", *args1->ptr1, *args1->ptr2);
      if (*(args1->ptr1) == ptr2PrevVal && *(args1->ptr2) == ptr1PrevVal) {
        printf("\t[ OK ]\n");
      } else {
        printf("\t[ fail ] (expected *ptr1=%d, *ptr2=%d)\n", ptr2PrevVal, ptr1PrevVal);
      }
    }

    serializationExperiment_args * args2;

    for(args2 = serializationexperiment_args; args2->function_name != NULL; args2++) {
        serializeBE(args2->a, args2->v);
        printf("%s([0,0,0,0], %08x) -> [%02x,%02x,%02x,%02x]", args2->function_name, args2->v, args2->a[0], args2->a[1], args2->a[2], args2->a[3]);
        if ( args2->a[0] == args2->expected_result[0] &&
        args2->a[1] == args2->expected_result[1] &&
        args2->a[2] == args2->expected_result[2] &&
        args2->a[3] == args2->expected_result[3]) {
          printf("\t[ OK ]\n");
        } else {
          printf("\t[ fail ] (expected [%02x,%02x,%02x,%02x])\n", args2->expected_result[0], args2->expected_result[1], args2->expected_result[2], args2->expected_result[3]);
        }
    }


    deserializationExperiment_args * args3;

    for(args3 = deserializationexperiment_args; args3->function_name != NULL; args3++) {
        deserializeBE(args3->v, args3->a);
        printf("%s (0, [%02x,%02x,%02x,%02x]) -> %08x", args3->function_name, args3->a[0], args3->a[1], args3->a[2], args3->a[3], *args3->v);
        if ( *args3->v == args3->expected_result) {
          printf("\t[ OK ]\n");
        } else {
          printf("\t[ fail ] (expected %08x)\n", args3->expected_result);
        }
    }

    withinArray_args * args4;

    for (args4 = arr_args; args4->function_name != NULL; args4++) {
      result = withinArray((args4->base), args4->size, args4->ptr);
      printf("%s(%p, %d, %p) = %d", args4->function_name, args4->base, args4->size, args4->ptr, result);
      if (result == args4->expected_result) {
        printf("\t[ OK ]\n");
      } else {
        printf("\t[ fail ] (expected %d)\n", args4->expected_result);
      }
    }

    stringLength_args * args5;

    for(args5 = strlen_args; args5->function_name != NULL; args5++) {
        result = stringLength(args5->s);
        printf("%s(%s) = %d", args5->function_name, args5->s, result);
        if (result == args5->expected_result) {
          printf("\t[ OK ]\n");
        } else {
          printf("\t[ fail ] (expected %d)\n", args5->expected_result);
        }
    }

    stringSpan_args * args6;

    for(args6 = strspn_args; args6->function_name != NULL; args6++) {
        result = stringSpan(args6->str1, args6->str2);
        printf("%s(%s, %s) = %d", args6->function_name, args6->str1, args6->str2, result);
        if (result == args6->expected_result) {
          printf("\t[ OK ]\n");
        } else {
          printf("\t[ fail ] (expected %d)\n", args6->expected_result);
        }
    }

     selectionSort_args * args7;

    for(args7 = selSort_args; args7->function_name != NULL; args7++) {
        selectionSort(args7->arr, args7->arrLength);

        printf("%s(", args7->function_name);
        print_array( args7->arr_copy, args7->arrLength);
        printf(", %d) = ", args7->arrLength);
        print_array( args7->arr, args7->arrLength);
        int i;
        result = 1;
        for(i = 0; i < args7->arrLength; i++) {
          if(args7->arr[i] != args7->expected_result[i]) {
            result = !result;
            break;
          }
        }
        if (result) {
          printf("\t[ OK ]\n");
        } else {
          printf("\t[ fail ] (expected ");
          print_array( args7->expected_result, args7->arrLength);
          printf(")\n");
        }
    }   

    return 0;
}
