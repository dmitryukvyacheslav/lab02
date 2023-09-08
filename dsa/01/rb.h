#pragma once
typedef enum treecolor {
     black;     
     red;
};

struct rb_tree_node {
       treecolor color;
       int key;
       rb_tree_node* left_child;
       rb_tree_node* right_child;
       rb_tree_node* parent;
       rb_tree_node* root;       
};

void rb_add (rb_tree_node* tree, int key);
void rb_remove (rb_tree_node* tree, int key);
void rb_print_dfs (rb_tree_node* tree, int depth);
void rb_destroy (rb_tree_node* tree);
int rb_min (rb_tree_node* tree);
int rb_max (rb_tree_node* tree);
