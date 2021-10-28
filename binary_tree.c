#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stddef.h>
#include<string.h>
#include<time.h>

/* A binary tree node */
typedef struct bt_node {
    int val;
    struct bt_node* left;
    struct bt_node* right;
} bt_node_t;

/* A binary tree root */
typedef struct bt_root {
    bt_node_t* root;
} bt_root;

/* Frees the memory occupied by each binary tree node */
void free_bt_internal(bt_node_t* node) {
    if (node->left)
        free_bt_internal(node->left);
    if (node->right)
        free_bt_internal(node->right);
    free(node);
}

/* Frees the memory ocuppied by a binary tree */
void free_bt(bt_root* head) {
    if (head->root)
        free_bt_internal(head->root);
    free(head);
}

/* Gets a pointer to a binary tree node which has the required 
   value. If not possible, returns what would be its father */
bt_node_t* get_node(bt_node_t* root, int val) {
    bt_node_t* current = root;
    while (true) {
        if (val > current->val) {
            if (current->right)
                current = current->right;
            else
                return current;
        } else if (val < current->val) {
            if (current->left)
                current = current->left;
            else
                return current;
        } else
            return current;
    }
}

/* Creates a pointer to a new bt node */
bt_node_t* create_node(int val) {
    bt_node_t* new_node = malloc(sizeof(bt_node_t));
    new_node->val = val;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

/* Inserts a new node in the binary tree, if not duplicated */
void insert(bt_root* head, int val) {
    /* First, we verify if the binary tree is null, if so then
       we create its first node. Else, then we verify if already
       exists a node with the given value, if so we return error,
       else we insert the new node in the right place */
    if (head->root == NULL) {
        head->root = create_node(val);
    } else {
        bt_node_t* node = get_node(head->root, val);
        if (val == node->val) {
            fprintf(stderr, "Duplicate value\n");
            abort();
        } else if (val > node->val)
            node->right = create_node(val);
        else
            node->left = create_node(val);
    }
}

/* Finds a replacement node which can be either the successor or
   the predecessor inorder. Which it will be is decided by the
   time in clock cycles. If it is pair, then successor, else
   predecessor */
bt_node_t* replacement(bt_node_t* node) {
    int decision = clock() % 2;
    bt_node_t* current;
    if (decision) {
        current = node->left;
        while (current->right)
            current = current->right;
        return current;
    } else {
        current = node->right;
        while (current->left)
            current = current->left;
        return current;
    }
}

/* Deletes a node of a specific value if it exists */
bt_node_t* delete_internal(bt_node_t* node, int val) {
    if (val == node->val) {
        if (node->left == NULL && node->right == NULL) {
            free(node);
            return NULL;
        }
        else if (node->left == NULL || node->right == NULL) {
            if (node->left)
                return node->left;
            return node->right;
        } else {
            bt_node_t* rep = replacement(node);
            if (rep->val > node->val) {
                node->val = rep->val;
                node->right = delete_internal(node->right, rep->val);
            } else {
                node->val = rep->val;
                node->left = delete_internal(node->left, rep->val);
            }
            return node;
        }
    } else if (val > node->val && node->right) {
        node->right = delete_internal(node->right, val);
        return node;
    }
    else if (val < node->val && node->left) {
        node->left = delete_internal(node->left, val);
        return node;
    }
    else {
        fprintf(stderr, "Value not found\n");
        abort();
    }
}

/* If there is a binary tree, then deletes a node of a specific
   value if this node exists */
void delete(bt_root* head, int val) {
    if (head->root == NULL) {
        fprintf(stderr, "There is no binary tree to delete a node from\n");
        abort();
    }
    head->root = delete_internal(head->root, val);
}

int idx = 0;

/* Inserts a new relationship in a .gv file */
void graph_insert(FILE* graph, bt_node_t* node) {
    if (node == NULL)
        return;
    if (node->left != NULL) {
        fprintf(graph, "%d -> %d\n\t", node->val, node->left->val);
        graph_insert(graph, node->left);
    }
    if (node->right != NULL) {
        fprintf(graph, "%d -> %d\n\t", node->val, node->right->val);
        graph_insert(graph, node->right);
    }
}

/* Modifies a .gv file to represents a given binary tree. Each
   time this function is called it will create a different .gv file,
   it is useful to see if the binary tree is working as supposed to */
void graph_representation(bt_root* head) {
    char filename[20];
    sprintf(filename, "graph%d.gv", idx);
    ++idx;

    FILE* graphptr;
    graphptr = fopen(filename, "w");

    if (graphptr == NULL) {
        fprintf(stderr, "Error in the graphptr\n");
        abort();
    }

    const char begin[] = "digraph graphname {\n\tnode[shape=ellipse];\n\tedge[color=blue,\
    ];\n\tlayout=dot;\n\t";
    const char end[] = "\n}";

    fprintf(graphptr, "%s", begin);

    bt_node_t* root = head->root;

    if (root != NULL) {
        graph_insert(graphptr, root);
    }

    fprintf(graphptr, "%s", end);
    fclose(graphptr);
}

int main(int argc, char* argv[argc+1]) {
    clock_t start = clock();
    bt_root* pseudoroot = malloc(sizeof(bt_root));
    int A[] = {6, 4, 9, 2, 8, 3, 1, 5};
    for (int i = 0; i < 8; ++i) {
        printf("val = %d\n", A[i]);
        insert(pseudoroot, A[i]);
    }
    /* The delete function is not working as it should, take a look later */
    graph_representation(pseudoroot);
    delete(pseudoroot, 2);
    delete(pseudoroot, 4);
    graph_representation(pseudoroot);
    free(pseudoroot);
    printf("Time taken: %.8fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);
    return EXIT_SUCCESS;
}