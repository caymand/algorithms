// Nodes of a binary tree.
struct Node 
{
    Node *left;
    Node *right;
    int id;
};

// Binary trees are then a pointer to the root node of the tree.
typedef Node * BinaryTree;

// Items of a Queue
struct Item
{
    Node *node;
    Item *next;
};

// Queue is represented as a singly linked list of "Item"s and a pointer to
// the head of the list and the last element.
struct Queue
{
    Item *head;
    Item *last;
};

struct Scratch
{
    char *mem;
    unsigned long count; // In bytes
    unsigned long capacity; // In Bytes
};
