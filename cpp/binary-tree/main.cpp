#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "binarytree.h"

// My library dependencies
#include "arena/arena_inc.h"
#include "arena/arena_inc.cpp"

void enqueue(Arena *scratch, Queue *Q, Node *node)
{    
    Item *item = ArenaPushStruct(scratch, Item);
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
void bft(Arena *scratch, BinaryTree tree)
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

    Arena *scratch = ArenaAlloc(Gigabytes(1));

    /*
      Input:
        5
      4   3
     2 1   0

      Output:
        5
        4 3
        2 1 0
     */

    Node child0 = {nullptr, nullptr, 0, -1};
    Node child1 = {nullptr, nullptr, 1, -1};
    Node child2 = {nullptr, nullptr, 2, -1};
    Node child3 = {nullptr, &child0, 3, -1};
    Node child4 = {&child2, &child1, 4, -1};
    Node root   = {&child4, &child3, 5, -1};
    BinaryTree tree = &root;
        

    bft(scratch, tree);

    return 0;
}

