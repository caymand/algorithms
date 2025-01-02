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
    // Mark the explored node by the explore depth
    int depth = 0; // parent depth
    Queue Q = {};
    Node *root = tree;
    root->explored = 0;

    enqueue(scratch, &Q, root);
    for (Node *node = dequeue(&Q); node != nullptr; node = dequeue(&Q))
    {
        if (depth < node->explored)
        {
            puts("\n");
            depth++;
        }                
        printf("%d ", node->id);        
        if (node->left != nullptr && node->left->explored < 0)            
        {
            enqueue(scratch, &Q, node->left);
            node->left->explored = depth + 1;
        }
        if (node->right != nullptr && node->right->explored < 0)
        {
            enqueue(scratch, &Q, node->right);
            node->right->explored = depth + 1;
        }
    }
    puts("\n");
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

    /*
      Input:
        4
      3   2
     1 0 0

      Output:
        4
        3 2
        1 0
     */

    Node child0 = {nullptr, nullptr, 0, -1};
    Node child1 = {nullptr, nullptr, 1, -1};
    Node child2 = {nullptr, nullptr, 2, -1};
    Node child3 = {nullptr, &child0, 3, -1};
    Node child4 = {&child2, &child1, 4, -1};
    Node root   = {&child4, &child3, 5, -1};
    BinaryTree tree = &root;
    
    printf("This is a test\n");

    bft(&scratch_mem, tree);

    return 0;
}

