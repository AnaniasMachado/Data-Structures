#include <stdio.h>
#include <stdlib.h>

// Defines a struct for a Dynamic Array
typedef struct DynamicArray {
    int* data;
    size_t size;
    size_t unused;
} DynamicArray;

// Initializes a Dynamic Array e sets a initial size
DynamicArray* DynamicArray_Init(size_t size);
// Adds a value at the first unused index of the Dynamic Array
DynamicArray* DynamicArray_Push(DynamicArray* arr, int val);
// Sets a value at a specific index
void DynamicArray_Set(DynamicArray* arr, int val, int idx);
// Gets the value at a specific index
int DynamicArray_Get(DynamicArray* arr, int idx);
// Deletes the element at index idx
void DynamicArray_Delete(DynamicArray* arr, int idx);
// Resizes the size of the given DynamicArray
DynamicArray* DynamicArray_Resize(DynamicArray* arr);
// Prints all the inserted elements
void DynamicArray_Print(DynamicArray* arr);
// Frees the allocated memory
void DynamicArray_Free(DynamicArray* arr);