#include <rb.h>
     // was made in drunken haze pls revise when you come home!!!!!
     
static void rb_fixup(rb_tree_node* node) {
       rb_tree_node* parent = node.parent;
       rb_tree_node* grandfather = node.parent.parent;
       rb_tree_node* uncle;
       if (grandfather.left_child == parent) uncle = grandfather.right_child
          else uncle == grandfather.left_child;
       
       
}
static rb_tree_node* rb_create_node(int key) {
       rb_tree_node* node = malloc(sizeof(rb_tree_node));
       node.color = red;
       node.key = key;
       node.left_child = NULL;
       node.right_child = NULL;
              
}
void rb_add(rb_tree_node* T, int key) {
     rb_tree_node* tree = T.root;
     rb_tree_node* parent;
     while (tree) {
           parent = tree;
           if (key < tree.key)
              tree = tree.left;
           else if (key > tree.key)
              tree = tree.right;
           else
               return;
     }
     rb_tree_node* node = rb_create_node(key);
     if (node) tree = node                    
     else printf("rb_add: failed to allocate node[%d]\n", key);
     if (!T.root) T.root = node;             
     tree.parent = parent;
     if (key < tree.parent.key)
        tree.parent.left_child = node;
     else tree.parent.right_child = node;
}
