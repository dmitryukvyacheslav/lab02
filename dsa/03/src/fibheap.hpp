typedef struct fh_node {
	int key;			// Priority
	char* value;		// Value
	fh_node* parent;	// Parent node
	fh_node* child;		// Child nodes C2LL
	fh_node* left;		// C2LL left brother node
	fh_node* right;		// C2LL right brother node
	int degree;			// #child nodes
	bool mark;			// Deletion flag
	fh_node* min;	// Min in C2LL ROOT LIST
	int nnodes;		// Root count
} fh_node;


struct fh_node* fibheap_insert(struct fh_node* heap, int key, char* value);
struct fh_node* fibheap_min(struct fh_node* heap);
struct fh_node* fibheap_union(struct fh_node* heap1, struct fh_node* heap2);
struct fh_node* fibheap_delete_min(struct fh_node* heap);
void fibheap_decrease_key(struct fh_node* heap, fh_node* x, int newkey);
void fibheap_delete(struct fh_node* heap, fh_node* x);