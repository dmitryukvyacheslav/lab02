#include <fibheap.hpp>
#include <cstdio>
int main(void) {
	fh_node* heap = nullptr;

	char* strings[] = { "shield", "spear", "sword", "bow", "chariot", "potion", "magic", "blaze" };
	for (int i = 0; i < 8; i++) {
		heap = fibheap_insert(heap, i, strings[i]);	// Correct
	}
	for (int i = 0; i < 8; i++) {
		fh_node* a = fibheap_min(heap);				// Correct
		printf("%s[%d]\n", a->value, a->key);
		delete fibheap_delete_min(heap);
	}
	return 0;
}