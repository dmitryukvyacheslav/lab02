#include <math.h>
#include <limits.h>
#include <cstdlib>
typedef struct fh_node {
	int key;			// Priority
	char* value;		// Value
	fh_node* parent;	// Parent node
	fh_node* child;		// Child nodes C2LL
	fh_node* left;		// C2LL left brother node
	fh_node* right;		// C2LL right brother node
	int degree;			// #child nodes
	bool mark;			// Deletion flag
} fh_node;

typedef struct fibheap {
    int nnodes;
    fh_node* min;
} fibheap;

void insert(fibheap* heap, int key, char* value)
{
    fh_node* new_node = new fh_node();
    new_node->key = key;
    new_node->value = value;
    new_node->degree = 0;
    new_node->mark = 0;
    new_node->parent = 0;
    new_node->child = 0;
    new_node->left = new_node;
    new_node->right = new_node;
    if (heap->min != 0) {
        (heap->min->left)->right = new_node;
        new_node->right = heap->min;
        new_node->left = heap->min->left;
        heap->min->left = new_node;
        if (new_node->key < heap->min->key)
            heap->min = new_node;
    }
    else {
        heap->min = new_node;
    }
    heap->nnodes++;
}
void link(fibheap* heap, fh_node* ptr2, fh_node* ptr1)
{
    (ptr2->left)->right = ptr2->right;
    (ptr2->right)->left = ptr2->left;
    if (ptr1->right == ptr1)
        heap->min = ptr1;
    ptr2->left = ptr2;
    ptr2->right = ptr2;
    ptr2->parent = ptr1;
    if (ptr1->child == 0)
        ptr1->child = ptr2;
    ptr2->right = ptr1->child;
    ptr2->left = (ptr1->child)->left;
    ((ptr1->child)->left)->right = ptr2;
    (ptr1->child)->left = ptr2;
    if (ptr2->key < (ptr1->child)->key)
        ptr1->child = ptr2;
    ptr1->degree++;
}
void consolidate(fibheap* heap)
{
    int temp1;
    float temp2 = (log(heap->nnodes)) / (log(2));
    int temp3 = temp2;
    fh_node** arr = new fh_node* [temp3 + 1];
    for (int i = 0; i <= temp3; i++)
        arr[i] = NULL;
    fh_node* ptr1 = heap->min;
    fh_node* ptr2;
    fh_node* ptr3;
    fh_node* ptr4 = ptr1;
    do {
        ptr4 = ptr4->right;
        temp1 = ptr1->degree;
        while (arr[temp1] != NULL) {
            ptr2 = arr[temp1];
            if (ptr1->key > ptr2->key) {
                ptr3 = ptr1;
                ptr1 = ptr2;
                ptr2 = ptr3;
            }
            if (ptr2 == heap->min)
                heap->min = ptr1;
            link(heap, ptr2, ptr1);
            if (ptr1->right == ptr1)
                heap->min = ptr1;
            arr[temp1] = NULL;
            temp1++;
        }
        arr[temp1] = ptr1;
        ptr1 = ptr1->right;
    } while (ptr1 != heap->min);
    heap->min = NULL;
    for (int j = 0; j <= temp3; j++) {
        if (arr[j] != NULL) {
            arr[j]->left = arr[j];
            arr[j]->right = arr[j];
            if (heap->min != NULL) {
                (heap->min->left)->right = arr[j];
                arr[j]->right = heap->min;
                arr[j]->left = heap->min->left;
                heap->min->left = arr[j];
                if (arr[j]->key < heap->min->key)
                    heap->min = arr[j];
            }
            else {
                heap->min = arr[j];
            }
            if (heap->min == NULL)
                heap->min = arr[j];
            else if (arr[j]->key < heap->min->key)
                heap->min = arr[j];
        }
    }
}
fh_node* extractMin(fibheap* heap)
{
    if (!heap->min)
        return 0;
    else {
        fh_node* temp = heap->min;
        fh_node* pntr;
        pntr = temp;
        fh_node* x = 0;
        if (temp->child) {

            x = temp->child;
            do {
                pntr = x->right;
                (heap->min->left)->right = x;
                x->right = heap->min;
                x->left = heap->min->left;
                heap->min->left = x;
                if (x->key < heap->min->key)
                    heap->min = x;
                x->parent = 0;
                x = pntr;
            } while (pntr != temp->child);
        }
        (temp->left)->right = temp->right;
        (temp->right)->left = temp->left;
        heap->min = temp->right;
        if (temp == temp->right && !temp->child)
            heap->min = 0;
        else {
            heap->min = temp->right;
            consolidate(heap);
        }
        heap->nnodes--;
        return heap->min;
    }
}
void cut(fibheap* heap, fh_node* found, fh_node* temp)
{
    if (found == found->right)
        temp->child = NULL;

    (found->left)->right = found->right;
    (found->right)->left = found->left;
    if (found == temp->child)
        temp->child = found->right;

    temp->degree = temp->degree - 1;
    found->right = found;
    found->left = found;
    (heap->min->left)->right = found;
    found->right = heap->min;
    found->left = heap->min->left;
    heap->min->left = found;
    found->parent = NULL;
    found->mark = 1;
}
void cascadeCut(fibheap* heap, fh_node* temp)
{
    fh_node* ptr5 = temp->parent;
    if (ptr5 != NULL) {
        if (temp->mark == 0) {
            temp->mark = 1;
        }
        else {
            cut(heap, temp, ptr5);
            cascadeCut(heap, ptr5);
        }
    }
}
void decreaseKey(fibheap* heap, fh_node* x, int newkey) {
    if (newkey > x->key)
        return; /* Новый ключ больше текущего значения ключа */
    x->key = newkey;
    fh_node* y = x->parent;
    if (y != NULL && x->key < y->key) {
        /* Нарушены свойства min-heap: ключ родителя больше */
        /* Вырезаем x и переносим его в список корней */
        cut(heap, x, y);
        cascadeCut(heap, y);
    }
    /* Корректируем указатель на минимальный узел */
    if (x->key < heap->min->key)
        heap->min = x;
}
void remove(fibheap* heap, fh_node* x)
{
    decreaseKey(heap, x, INT_MIN);
    extractMin(heap);
}
void joinLists(fh_node* heap1, fh_node* heap2) {
    if (!heap1 || !heap2) 
        return;
    fh_node* left1 = heap1->left;
    fh_node* left2 = heap2->left;
    left1->right = heap2;
    heap2->left = left1;
    heap1->left = left2; 
    left2->right = heap1;
    return;

}
fibheap* join(fibheap* heap1, fibheap* heap2) {
    fibheap* heap = new fibheap;
    heap->min = heap1->min;
    joinLists(heap1->min, heap2->min);
    if ((heap1->min) || (heap2->min != NULL && heap2->min->key < heap->min->key))
        heap->min = heap2->min;
    heap->nnodes = heap1->nnodes + heap2->nnodes;
    free(heap1);
    free(heap2);
    return heap;
}