#pragma once
int intSize();
void swapInts(int *i1, int *i2);
void serializeBE(unsigned char *array, int value);
void deserializeBE(int* value, const unsigned char *array);
int withinArray(int *intArray, int size, int *ptr);
int stringLength(char *s);
int stringSpan(char * str1, char * str2);
void selectionSort(int *array, int arrLength);