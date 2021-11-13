#include "doubly_linked_list.h"

#include <sys/ioctl.h>
#include <unistd.h>

// Initializes a dll_Head node
void dll_Init(dll_Node *head) {
    head->text = NULL;
    head->prev = NULL;
    head->next = NULL;
}

// Frees the allocated memory
void dll_Free(dll_Node *head) {
    if (head == NULL) {
        fprintf(stderr, "It was not possible to effectuate a Free: No doubly linked list\n");
        abort();
    } else {
        dll_Node *current = head->next;
        dll_Node *tmp;

        while (current != NULL && current != head) {
            tmp = current;
            current = current->next;

            free(tmp->text);
            free(tmp);
        }

        free(head->text);
        free(head);
    }
}

// Add a new dll_Node to the end of the doubly linked list
void dll_Insert(dll_Node *head, char *new_text) {
    // There is three cases. In the first case, the head's text is null and with it we
    // can say that this is the first node. So we insert the text in this node. In the
    // second case, the next node is null. So we create a new node, we insert the text
    // in this new node and we rearranje all the pointers to make it become a doubly
    // linked list. The third case is very similar to the second case, however, at this
    // point we have enough pointers to just change the pointers from the tail, head
    // and the new node.
    if (head->text == NULL) {
        head->text = (char*) malloc(strlen(new_text) * sizeof(char));
        strcpy(head->text, new_text);
    } else if (head->next == NULL) {
        dll_Node *new_node = (dll_Node*) malloc(sizeof(dll_Node));
        new_node->text = (char*) malloc(strlen(new_text) * sizeof(char));
        strcpy(new_node->text, new_text);
        new_node->prev = head;
        new_node->next = head;
        head->prev = new_node;
        head->next = new_node;
    } else {
        dll_Node *new_node = (dll_Node*) malloc(sizeof(dll_Node));
        new_node->text = (char*) malloc(strlen(new_text) * sizeof(char));
        strcpy(new_node->text, new_text);
        new_node->prev = head->prev;
        new_node->next = head;
        head->prev->next = new_node;
        head->prev = new_node;
    }
}

// Deletes the last node of the doubly linked list
void dll_Remove(dll_Node *head) {
    // Removes the last node if possible and rearranges the pointers of head and penult
    if (head->text == NULL) {
        fprintf(stderr, "There is no doubly linked list to delete the last node\n");
        abort();
    } else {
        dll_Node *tmp = head->prev;

        if (head->next == tmp) {
            free(tmp->text);
            free(tmp);
            head->prev = NULL;
            head->next = NULL;
        } else {
            head->prev->prev->next = head;
            head->prev = head->prev->prev;
            free(tmp->text);
            free(tmp);
        }
    }
}

// Splits the text in a given index
void dll_SplitIndex(dll_Node *head, size_t idx) {
    // Basically, if possible it creates a new node next to head with a slice of head's text
    // and rearranges all the necessary pointers
    if (head == NULL || head->text == NULL) {
        fprintf(stderr, "It was not possible to effectuate a SplitIndex: No node or text\n");
        abort();
    } else {
        size_t size = strlen(head->text);
        
        if (idx >= size) {
            fprintf(stderr, "It was not possible to effectuate a SplitIndex: Index out of \
range\n");
            abort();
        }

        dll_Node *new_node = (dll_Node*) malloc(sizeof(dll_Node));
        new_node->text = malloc(size - idx);
        strcpy(new_node->text, &head->text[idx]);

        char *tmp = (char*) malloc(sizeof(idx));
        for (int i = 0; i < idx; ++i) {
            tmp[i] = head->text[i];
        }

        free(head->text);
        head->text = (char*) malloc(sizeof(idx));
        strcpy(head->text, tmp);

        new_node->prev = head;
        new_node->next = head->next;

        head->next->prev = new_node;
        head->next = new_node;

        free(tmp);
    }
}

// Joins the text of two consecutive nodes
void dll_JoinNext(dll_Node *head) {
    // If possible, it merges the text of the head with the next node and deletes the next node.
    // Then we rearrange the pointers related to the nodes changed
    if (head == NULL || head->text == NULL || head->next->text == NULL) {
        fprintf(stderr, "It was not possible to effectuate a JoinNext: No head, head's text\
 or next's text\n");
        abort();
    } else {
        if (head->next == head->prev) {
            strcat(head->text, " ");
            strcat(head->text, head->next->text);
            free(head->next->text);
            free(head->next);
            head->prev = NULL;
            head->next = NULL;
        } else {
            strcat(head->text, head->next->text);
            dll_Node *tmp = head->next;
            head->next = head->next->next;
            free(tmp->text);
            free(tmp);
        }
    }
}

// Prints the text value of all nodes
void dll_Print(dll_Node *head) {
    if (head->text != NULL)
        printf("%s\n", head->text);
    
    dll_Node *current = head->next;

    while (current != NULL && current != head) {
        printf("%s\n", current->text);
        current = current->next;
    }
}

// Prints one line of text of all nodes
void dll_PrintOneLine(dll_Node *head) {
    // If possible, we print as much text as possible that fits in one line terminal's line.
    // The terminal's line is obtained with a code from stackoverflow. Then I calculate how
    // much space I have to print and before print each node's text, I check if it overflow or
    // not. If it overflows, I check if I can print a slice of this text. Otherwise I stop.
    if (head == NULL || head->text == NULL) {
        fprintf(stderr, "It was not possible to effectuate a PrintOneLine: There is no doubly\
 linked list or there is no text to print.\n");
        abort();
    } else {
        // How to get terminal's width: https://stackoverflow.com/a/1022961
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        size_t max_chars = w.ws_col;
        size_t count = 0;

        dll_Node *current = head;

        do {
            if (strlen(current->text) <= max_chars-count) {
                printf("%s", current->text);
                count += strlen(current->text);
                current = current->next;
            } else if (max_chars - count != 0) {
                size_t size = max_chars - count;
                char *tmp = malloc(sizeof(size));

                for (int i = 0; i < size; ++i)
                    tmp[i] = current->text[i];
                
                printf("%s", tmp);
                break;
            } else {
                break;
            }
        } while (current != head);

        printf("\n");
    }
}