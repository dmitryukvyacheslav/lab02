#include <fibheap.hpp>
#include <vector>
// Fibbonaci heaps are for _____.
// I was listening to Blood Sugar while doing this
// Please stop my procrastination someone please please please

void FibHeapAddNodeToRootList(fh_node* node, fh_node* h) {
	if (!h)
		return;
	if (h->left == h)
	{	/* Случай 1: список h содержит один корень */
		h->left = node;
		h->right = node;
		node->right = h;
		node->left = h;
	}
	else 
	{
		/* Случай 2: список h содержит более одного корня */
		fh_node* lnode = h->left;
		h->left = node;
		node->right = h;

		node->left = lnode;
		lnode->right = node;
	}
}

fh_node* fibheap_insert(fh_node* heap, int key, char* value) {
	fh_node* node = new fh_node;
	node->key = key;
	node->value = value;
	node->degree = 0;
	node->mark = false;
	node->parent = nullptr;
	node->child = nullptr;
	node->left = node;
	node->right = node;
	node->min == nullptr;
	node->nnodes = 0;

		/* Добавляем узел в список корней heap */
	FibHeapAddNodeToRootList(node, heap);
	if (heap == nullptr) {
		heap = node;
		heap->min = node;
	}
	
	else if ((!heap->min) || (node->key < heap->min->key))
		heap->min = node;
	heap->nnodes = heap->nnodes + 1;
	return heap;
}

fh_node* fibheap_min(fh_node* heap) {
	return heap->min;
}

fh_node* FibHeapLinkLists(fh_node* heap1, fh_node* heap2) {
	if (!heap1 || !heap2) return nullptr;
	fh_node* left1 = heap1->left;
	fh_node* left2 = heap2->left;
	left1->right = heap2;
	heap2->left = left1;
	heap1->left = left2;
	left2->right = heap1;
	return heap1;
}
fh_node* fibheap_union(fh_node* heap1, fh_node* heap2) {
	fh_node* heap = new fh_node;
	heap->min = heap1->min;
	FibHeapLinkLists(heap1->min, heap2->min);
	if ((heap1->min == nullptr) || (heap2->min != nullptr && heap2->min->key < heap->min->key))
		heap->min = heap2->min;
	heap->nnodes = heap1->nnodes + heap2->nnodes;
	delete heap1;
	delete heap2;
	return heap;
}
void FibHeapRemoveNodeFromRootList(fh_node* del, fh_node* heap) {
	/* base case */
	if (heap == NULL || del == NULL)
		return;

	/* Change next only if node to be
	deleted is NOT the last node */
	if (del->right != NULL)
		del->right->left = del->left;

	/* Change prev only if node to be
	deleted is NOT the first node */
	if (del->left != NULL)
		del->left->right = del->right;

	/* If node to be deleted is head node */
	if (heap == del) {
		heap = del->right;
		heap->nnodes = del->nnodes;
	}
	return;
}

int areTheyNeighbours(fh_node* A, fh_node* B) {
	return (A->right == B && B->left == A) || (A->left == B && B->right == A);
}

void refreshOuterPointers(fh_node* A) {
	if (A->left != NULL)
		A->left->right = A;

	if (A->right != NULL)
		A->right->left = A;
}

void FibHeapSwap(fh_node* A, fh_node* B) {
	fh_node* swapperVector[4];
	fh_node* temp;

	if (A == B) return;

	if (B->right == A) {
		temp = A;
		A = B;
		B = temp;
	}

	swapperVector[0] = A->left;
	swapperVector[1] = B->left;
	swapperVector[2] = A->right;
	swapperVector[3] = B->right;

	if (areTheyNeighbours(A, B)) {
		A->left = swapperVector[2];
		B->left = swapperVector[0];
		A->right = swapperVector[3];
		B->right = swapperVector[1];
	}
	else {
		A->left = swapperVector[1];
		B->left = swapperVector[0];
		A->right = swapperVector[3];
		B->right = swapperVector[2];
	}

	refreshOuterPointers(A);
	refreshOuterPointers(B);
}

void FibHeapLink(fh_node* heap, fh_node* y, fh_node* x) {
	x->degree = x->degree + 1;
	/* Делаем y дочерним узлом x */
	FibHeapRemoveNodeFromRootList(y, heap);
	y->parent = x;
	FibHeapAddNodeToRootList(y, x->child);
	y->mark = false;
}
void FibHeapConsolidate(fh_node* heap) {
	int d = (int)floor(log(heap->nnodes));
	fh_node** A = new fh_node* [d];
	for (int i = 0; i < d; i++)
		A[i] = 0;

	std::vector<fh_node*> rootlist;
	fh_node* w = heap->min;
	int m = 0;
	do {
		rootlist.push_back(w);
		w = w->right;
		m++;
	} while (w != heap->min);

	for (int i = 0; i < m; i++) {
		fh_node* x = rootlist[i];
		int d = x->degree;
		while (A[d]) {
			fh_node* y = A[d];
			if (x->key > y->key)
				FibHeapSwap(x, y);
			FibHeapLink(heap, y, x);
			A[d] = 0;
			d++;
		}
		A[d] = x;
	}
	heap->min = NULL;
	for (int i = 0; i < d; i++) {
		if (A[i]) {
			FibHeapAddNodeToRootList(A[i], heap);
			if (!heap->min || A[i]->key < heap->min->key)
				heap->min = A[i];
		}
	}
}

fh_node* fibheap_delete_min(fh_node* heap) {
	fh_node* z = heap->min;
	if (!z) return nullptr;
	fh_node* child_ll = z->child;	// beat drops
	
	do {
		if (!child_ll) break;
		FibHeapAddNodeToRootList(child_ll, heap); /* Добавляем дочерний узел x в список корней */
		child_ll->parent = nullptr;
		child_ll = child_ll->right;
	} while (child_ll != z->child);
	FibHeapRemoveNodeFromRootList(z, heap); /* Удаляем z из списка корней */
	heap->nnodes = heap->nnodes - 1;
	if (z == z->right)
		heap->min = nullptr;
	else {
		heap->min = z->right;
		FibHeapConsolidate(heap);
	}
	return z;
}
void FibHeapCut(fh_node* heap, fh_node* x, fh_node* y) {
	/* Удаляем x из списка дочерних узлов y */
	FibHeapRemoveNodeFromRootList(x, y);
	y->degree = y->degree - 1;
		/* Добавляем x в список корней кучи heap */
	FibHeapAddNodeToRootList(x, heap);
	x->parent = NULL;
	x->mark = false;
}

void FibHeapCascadingCut(fh_node* heap, fh_node* y) {
	fh_node* z = y->parent;
	if (z == NULL)
		return;
	if (!y->mark)
		y->mark = true;
	else {
		FibHeapCut(heap, y, z);
		FibHeapCascadingCut(heap, z);
	}
}

void fibheap_decrease_key(fh_node* heap, fh_node* x, int newkey) {
	if (newkey > x->key) return; /* Новый ключ больше текущего значения ключа */
	x->key = newkey;
	fh_node* y = x->parent;
	if (y != NULL && x->key < y->key) {
		/* Нарушены свойства min-heap: ключ родителя больше */
		/* Вырезаем x и переносим его в список корней */
		FibHeapCut(heap, x, y);
		FibHeapCascadingCut(heap, y);
	}
	/* Корректируем указатель на минимальный узел */
	if (x->key < heap->min->key)
		heap->min = x;
}

void fibheap_delete(fh_node* heap, fh_node* x) {
	fibheap_decrease_key(heap, x, INT_MIN);
	fibheap_delete_min(heap);
}