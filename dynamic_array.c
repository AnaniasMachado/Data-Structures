#include "dynamic_array.h"

// Initializes a Dynamic Array e sets a initial size
DynamicArray* DynamicArray_Init(size_t size) {
    DynamicArray* arr = malloc(sizeof(DynamicArray));
    arr->data = malloc(size * sizeof(int));
    arr->size  = size;
    arr->unused = 0;
}

// Adds a value at the first unused index of the Dynamic Array
DynamicArray* DynamicArray_Push(DynamicArray* arr, int val) {
    if (arr->data != NULL) {
        if (arr->unused < arr->size) {
            arr->data[arr->unused] = val;
            arr->unused += 1;
            return arr;
        } else {
            arr = DynamicArray_Resize(arr);
            DynamicArray_Push(arr, val);
            return arr;
        }
    } else {
        size_t size = 50;
        arr = DynamicArray_Init(size);
        DynamicArray_Push(arr, val);
        return arr;
    }
}

// Sets a value at a specific index
void DynamicArray_Set(DynamicArray* arr, int val, int idx) {
    if (idx >= arr->unused) {
        fprintf(stderr, "DynamicArray_Set: Index out of range.\n");
        abort();
    } else {
        arr->data[idx] = val;
    }
}

// Gets the value at a specific index
int DynamicArray_Get(DynamicArray* arr, int idx) {
    if (idx >= arr->unused) {
        fprintf(stderr, "DynamicArray_Get: Index out of range.\n");
        abort();
    } else {
        return arr->data[idx];
    }
}

// Deletes the element at index idx
void DynamicArray_Delete(DynamicArray* arr, int idx) {
    if (idx >= arr->unused) {
        fprintf(stderr, "DynamicArray_Delete: Index out of range.\n");
        abort();
    } else {
        for (size_t i = idx; i < arr->unused-1; ++i)
            arr->data[idx] = arr->data[idx+1];
        arr->unused = arr->unused - 1;
    }
}

// Resizes the size of the given DynamicArray
DynamicArray* DynamicArray_Resize(DynamicArray* arr) {
    size_t new_size  = 2 * arr->size;
    DynamicArray* new_arr = DynamicArray_Init(new_size);
    for (size_t i = 0; i < arr->unused; ++i)
        new_arr = DynamicArray_Push(new_arr, arr->data[i]);
    return new_arr;
}

// Prints all the inserted elements
void DynamicArray_Print(DynamicArray* arr) {
    for (size_t i = 0; i < arr->unused; ++i)
        printf("%d ", arr->data[i]);
    printf("\n");
}

// Frees the allocated memory
void DynamicArray_Free(DynamicArray* arr) {
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->unused = 0;
}