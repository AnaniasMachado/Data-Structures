#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Node for a doubly linked list
typedef struct dll_node {
    char *text;
    struct dll_node *prev;
    struct dll_node *next;
} dll_Node;

// Initializes a dll_Head node
void dll_Init(dll_Node *head);
// Frees the allocated memory
void dll_Free(dll_Node *head);
// Add a new dll_Node to the end of the doubly linked list
void dll_Insert(dll_Node *head, char *new_text);
// Deletes the last node of the doubly linked list
void dll_Remove(dll_Node *head);
// Splits the text in a given index
void dll_SplitIndex(dll_Node *head, size_t idx);
// Joins the text of two consecutive nodes
void dll_JoinNext(dll_Node *head);
// Prints the text value of all nodes
void dll_Print(dll_Node *head);
// Prints one line of text of all nodes
void dll_PrintOneLine(dll_Node *head);