#include <fibheap.hpp>
#include <cstdio>
int main(void) {
	fibheap* heap = new fibheap;
	heap->min = 0;
	heap->nnodes = 0;

	fibheap* heap2 = new fibheap;
	heap2->min = 0;
	heap2->nnodes = 0;

	char* strings[] = { "zero", "one", "two", "three"};
	char* strings2[] = { "I", "II", "III", "IV" };
	for (int i = 0; i < 4; i++) {
		insert(heap, i, strings[i]);	// Correct
		insert(heap2, i, strings2[i]);	// Correct
	}

	heap = join(heap, heap2);

	for (int i = 0; i < 8; i++) {
		fh_node* min = heap->min;
		printf("%s[%d]\n", min->value, min->key);
		remove(heap, min);
	}

	// gg no re
	// i want to have nothing to do with this anymore
	return 0;
}