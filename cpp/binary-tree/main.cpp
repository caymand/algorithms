#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include "types.h"

#define MB 1000000

static int node_id = 0;

char* scratch_alloc(Scratch *scratch, unsigned long bytes)
{
    char *mem = scratch->mem;
    
    scratch->count += bytes;
    assert(scratch->count < scratch->capacity);
    scratch->mem += bytes;

    return mem;
}

#define alloc_type(T, scratch) ((T*) scratch_alloc((scratch), sizeof(T)))

void enqueue(Scratch *scratch, Queue *Q, Node *node)
{    
    Item *item = alloc_type(Item, scratch);
    item->node = node;
    item->next = nullptr;    
    
    if (Q->head == nullptr)
    {
        Q->head = item;
        Q->last = item;
        return;
    }
    // Invariant that a Queue with a single item has a last also.
    assert(Q->last != nullptr);
    
    Item *last = Q->last;
    last->next = item;
    Q->last = item;    
}

// Currently memory is not freed.
// Could also be two functions: 1. for dequeuing, 2. for removing the item.
Node* dequeue(Queue *Q)
{
    if (Q->head == nullptr)
    {
        return nullptr;
    }
    
    Item *cur_head = Q->head;
    Item *new_head = cur_head->next;

    Q->head = new_head;
    
    return cur_head->node;
}

// Breadth first traversal of the tree
void bft(Scratch *scratch, BinaryTree tree)
{
    int depth = 0;
    Queue Q = {};
    Node *root = tree;

    enqueue(scratch, &Q, root);
    for (Node *node = dequeue(&Q); node != nullptr; node = dequeue(&Q))
    {
        printf("Node: %d\n", node->id);
        
    }    
}
    
int main(int argc, char* argv[]) 
{
    constexpr size_t max_size = 10 * MB;
    //char scratch_buf[max_size] = {0};
    char *scratch_buf = (char *) malloc(max_size);
    assert(scratch_buf);

    Scratch scratch_mem = {
        .mem = scratch_buf,
        .count = 0,
        .capacity = max_size
    };
    
    Node child1 = {};
    child1.id = 1;
    Node child2 = {};
    child2.id = 2;
    Node child3 = {&child1, nullptr};
    child3.id = 3;
    Node root = {&child3, &child2};
    root.id = 4;
    BinaryTree tree = &root;
    
    printf("This is a test\n");

    bft(&scratch_mem, tree);

    return 0;
}

