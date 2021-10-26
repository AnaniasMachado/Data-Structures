#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stddef.h>
#include<string.h>
#include<time.h>

/* A linked list node */
typedef struct ll_node {
    int val;
    struct ll_node* next;
} ll_node_t;

/* A linked list head */
typedef struct ll_head {
    ll_node_t* next;
} ll_head_t;

/* Prints the value of all the linked list nodes */
void print_ll(ll_head_t* head) {
    ll_node_t* current = head->next;

    while (current) {
        printf("Node val = %d\n", current->val);
        current = current->next;
    }
}

/* Inserts a new node at the end of the linked list */
void insert(ll_head_t* head, int val) {
    /* We first verify if the ll is empty, if so then we
       insert a new node next to the head. If not, then
       we advance in the ll until we find a node that
       points to a null pointer. When found, we insert
       a new node in the address of this null pointer */
    if (head->next == NULL) {
        head->next = malloc(sizeof(ll_head_t));
        head->next->val = val;
        head->next->next = NULL;
    } else {
        ll_node_t* current = head->next;
        while (current->next)
            current = current->next;
        current->next = malloc(sizeof(ll_node_t));
        current->next->val = val;
        current->next->next = NULL;
    }
}

/* Inserts a new node at the begin of the linked list */
void enqueue(ll_head_t* head, int val) {
    /* First, we verify if the ll is empty, if so then
       head now points to a new node and that is it.
       Else, we create a new node and make it points to
       the node that head points. Then we make head
       points to this new node */
    if (head->next == NULL) {
        head->next = malloc(sizeof(ll_node_t));
        head->next->val = val;
        head->next->next = NULL;
    } else {
        ll_node_t* tmp = malloc(sizeof(ll_node_t));
        tmp->val = val;
        tmp->next = head->next;
        head->next = tmp;
    }
}

/* Deletes a node with a specific value */
void delete(ll_head_t* head, int val) {
    /* First, we verify if the ll is empty, if so then
       we return a error. Else, we advance in the ll
       until we either find the value we want to delete
       or we reach the end of the ll. If we do not find
       the value we want to delete, we return a error.
       Else, we free the nodes which contains the value
       that we want to delete and makes the precedessor
       points to its successor if it exists, else we
       create a null pointer */
    if (head->next == NULL) {
        fprintf(stderr, "Value not found\n");
        abort();
    }
    ll_node_t* current = head->next;

    while (current->next && current->next->val != val)
        current = current->next;
    
    if (current->next == NULL || current->next->val != val) {
        fprintf(stderr, "Value not found\n");
        abort();
    } else {
        if (current->next->next) {
            ll_node_t* tmp = current->next->next;
            free(current->next);
            current->next = tmp;
        } else {
            free(current->next);
            current->next = NULL;
        }
    }
}

/* Deletes the last node of the linked list */
void pop(ll_head_t* head) {
    /* First we verify if the ll is empty, if so we 
       do nothing and return. If not, then we advance
       until we find a node that points to a node that
       points to a null pointer. When found, we free
       this node that points to a null pointer and
       we create a null pointer in its place */
    if (head->next == NULL)
        return;
    else {
        ll_node_t* current = head->next;
        while (current->next->next)
            current = current->next;
        free(current->next);
        current->next = NULL;
    }
}

/* Modifies a .gv file to represents a given linked list */
void graph_representation(ll_head_t* head) {
    FILE* graphptr;
    graphptr = fopen("graph.gv", "w");

    if (graphptr == NULL) {
        fprintf(stderr, "Error in the graphptr");
        abort();
    }

    const char begin[] = "digraph graphname {\n\tnode[shape=box];\n\tedge[color=blue,\
    ];\n\tlayout=circo;\n\t";
    const char end[] = "\n}";

    fprintf(graphptr, "%s", begin);
    
    ll_node_t* current = head->next;

    while (current) {
        fprintf(graphptr, "%d", (current->val));
        current = current->next;
        if (current)
            fprintf(graphptr, "%s", " -> ");
    }

    fprintf(graphptr, "%s", end);
    fclose(graphptr);
}

int main(int argc, char* argv[argc+1]) {
    clock_t start = clock();
    ll_head_t* head = malloc(sizeof(ll_head_t));
    for (int i = 0; i < 12; ++i)
        insert(head, i);
    enqueue(head, 41);
    print_ll(head);
    graph_representation(head);
    printf("Time taken: %.8fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);
    return EXIT_SUCCESS;
}
