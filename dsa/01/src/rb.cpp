#include "rb.h"
#include <vector>
#include <stack>
#include <graphviz/gvc.h>
#include <cstdio>

rb_node* RedBlackTree::rb_createnode(int key) {
    rb_node* node = (rb_node*)malloc(sizeof(rb_node));
    if (node) {
        node->key = key;
        node->left_child = nullptr;
        node->right_child = nullptr;
        node->parent = nullptr;
        node->color = red;
    }
    return node;
}
void RedBlackTree::rb_leftrotate(rb_node* x) {
    rb_node* y = x->right_child;
    x->right_child = y->left_child;
    if (y->left_child)
        y->left_child->parent = x;
    if (x->parent) {
        y->parent = x->parent;
        if (x == x->parent->left_child)
            x->parent->left_child = y;
        else
            x->parent->right_child = y;
        x->parent = y;
        y->left_child = x;
        
    }
    else {
        x->parent = y;
        m_root = y;
        m_root->left_child = x;
        m_root->parent = nullptr;
    }
}
void RedBlackTree::rb_rightrotate(rb_node* x) {
    rb_node* y = x->left_child;
    x->left_child = y->right_child;
    if (y->right_child != NULL)
        y->right_child->parent = x;
    if (x->parent) {
        y->parent = x->parent;
        if (x == x->parent->left_child)
            x->parent->left_child = y;
        else
            x->parent->right_child = y;
        y->right_child = x;
        x->parent = y;
    }
    else {
        x->parent = y;
        m_root = y;
        m_root->right_child = x;
        m_root->parent = nullptr;
    }
    
}
void RedBlackTree::rb_addfixup(rb_node* node) {
    // When a new node is added, two properties can be violated:
    // * root can't be red;
    // * a red node can only have black children.
    // There are only 6 ways in which these properties can be violated.
    // Latter three mirror the first three.
    
    while (node->parent && node->parent->parent && node->parent->color == red) { // Red node can only have black children
        if ( node->parent == node->parent->parent->left_child) {
            /* z belongs to left subtree of G */
            rb_node* uncle = node->parent->parent->right_child;
            if (uncle && uncle->color == red) {
                /* case 1*/
                node->parent->color = black;
                uncle->color = black;
                node->parent->parent->color = red;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->right_child) {
                    /* case 2 -> case 3*/
                    node = node->parent;
                    rb_leftrotate(node);
                }
                /* case 3*/
                node->parent->color = black;
                node->parent->parent->color = red;
                rb_rightrotate(node->parent->parent);
            }
        }
        else {
            /* node belongs to the right subtree of G*/
            rb_node* uncle = node->parent->parent->left_child;
            if (uncle && uncle->color == red) {
                /* case 4*/
                node->parent->color = black;
                uncle->color = black;
                node->parent->parent->color = red;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->left_child) {
                    /* case 5 -> case 6*/
                    node = node->parent;
                    rb_rightrotate(node);
                }
                /* case 6*/
                node->parent->color = black;
                node->parent->parent->color = red;
                rb_leftrotate(node->parent->parent);
            }
        }
    }
    m_root->color = black;
}
void RedBlackTree::rb_delfixup(rb_node* node) {
    while (node != m_root && node->color == black) {
        if (node == node->parent->left_child) {
            rb_node* brother = node->parent->right_child;
            if (brother->color == red) { /* case 1 */
                brother->color = black;
                node->parent->color = red;
                rb_leftrotate(node->parent);
                brother = node->parent->right_child;
            }
            if (brother->left_child->color == black && brother->right_child->color == black) {
                brother->color = red;   /* case 2 */
                node = node->parent;
            }
            else {
                if (brother->right_child->color == black) { /* case 3*/
                    brother->left_child->color = black;
                    brother->color = red;
                    rb_rightrotate(brother);
                    brother = node->parent->right_child;
                }
                brother->color = node->parent->color;   /* case 4*/
                node->parent->color = black;
                brother->right_child->color = black;
                rb_leftrotate(node->parent);
                node = m_root;
            }
        }
        else {
            rb_node* brother = node->parent->left_child;
            if (brother->color == red) { /* case 5 */
                brother->color = black;
                node->parent->color = red;
                rb_rightrotate(node->parent);
                brother = node->parent->left_child;
            }
            if (brother->right_child->color == black && brother->left_child->color == black) {
                brother->color = red;   /* case 6 */
                node = node->parent;
            }
            else {
                if (brother->left_child->color == black) { /* case 7*/
                    brother->right_child->color = black;
                    brother->color = red;
                    rb_leftrotate(brother);
                    brother = node->parent->left_child;
                }
                brother->color = node->parent->color;   /* case 8*/
                node->parent->color = black;
                brother->left_child->color = black;
                rb_rightrotate(node->parent);
                node = m_root;
            }
        }
    }
    node->color = black;
}
void RedBlackTree::rb_free(rb_node* node) {
    if (node != NULL) {
        rb_free(node->right_child);
        rb_free(node->left_child);
        free(node);
    }
}
RedBlackTree::RedBlackTree() {
    m_root = nullptr;
    m_gvc = gvContext();
    m_requireUpdate = false;
    m_rd_len = 0;
    m_renderdata = nullptr;
    enable_fixup = 1;
};
RedBlackTree::~RedBlackTree() {
    rb_free();
    gvFreeContext(m_gvc);
    free_renderdata();
};
rb_node* RedBlackTree::add(int key) {
            // find what leaf node we can insert the element to
            rb_node* tree = m_root;
            rb_node* parent;
            while (tree != NULL) {
                parent = tree;
                if (key < tree->key)
                    tree = tree->left_child;
                else if (key > tree->key)
                    tree = tree->right_child;
                else
                    return tree;
            }
            // allocate new element
            rb_node* node = rb_createnode(key);
            // if tree is empty, make the new node its root
            if (m_root == nullptr) {
                m_root = node;
                node->parent = nullptr;
            }
            // if node has parents, is it a left or right child?
            else {
                node->color = red; // it's not a root so we have to make it red
                if (key < parent->key)
                    parent->left_child = node;
                else
                    parent->right_child = node;
                node->parent = parent;
            }
            node->color = red;
            if (enable_fixup) rb_addfixup(node); // rebalance the tree
            m_requireUpdate = true;
            return node;
        };
rb_node* RedBlackTree::lookup(int key) {
    rb_node* tree = m_root;
    while (tree != NULL) {
        if (key < tree->key)
            tree = tree->left_child;
        else if (key > tree->key)
            tree = tree->right_child;
        else
            return tree;
    }
    return NULL;
}
void RedBlackTree::rb_transplant(rb_node* node, rb_node* vtx) {
    if (node->parent == NULL)
        m_root = vtx;
    else if (node == node->parent->left_child)
        node->parent->left_child = vtx;
    else
        node->parent->right_child = vtx;
    if (vtx) vtx->parent = node->parent;
}
rb_node* RedBlackTree::remove(int key) {
    m_requireUpdate = true;
    rb_node* z = lookup(key);
    if (!z) return NULL;
    rb_node* y = z;
    rb_node* x;
    rb_color ycolor = y->color;
    if (z->left_child == NULL) {
        x = z->right_child;
        rb_transplant(z, z->right_child);

    }
    else if (z->right_child == NULL) {
        x = z->left_child;
        rb_transplant(z, z->left_child);
    }
    else
    {
        y = min(z->right_child);
        ycolor = y->color;
        x = y->right_child;
        if (y->parent == z) {
            if (x) x->parent = y;
        }
        else {
            rb_transplant(y, y->right_child);
            y->right_child = z->right_child;
            y->right_child->parent = y;
        }
        rb_transplant(z, y);
        y->left_child = z->left_child;
        y->left_child->parent = y;
        y->color = z->color;
    }
    if (ycolor == black)
        if (x) rb_delfixup(x);
    return x;
};
rb_node* RedBlackTree::min() {
    rb_node* tree = m_root;
    rb_node* parent = nullptr;
    while (tree != NULL) {
        parent = tree;
        tree = tree->left_child;
    }
    return parent;
}
rb_node* RedBlackTree::min(rb_node* node) {
    rb_node* tree = node;
    rb_node* parent = nullptr;
    while (tree != NULL) {
        parent = tree;
        tree = tree->left_child;
    }
    return parent;
}
rb_node* RedBlackTree::max() {
    rb_node* tree = m_root;
    rb_node* parent = nullptr;
    while (tree != NULL) {
        parent = tree;
        tree = tree->right_child;
    }
    return parent;
}
rb_node* RedBlackTree::max(int* height) {
    rb_node* tree = m_root;
    rb_node* parent = nullptr;
    *height = 0;
    while (tree != NULL) {
        tree = tree->right_child;
        parent = tree;
        (*height)++;
    }
    return parent;
}
void RedBlackTree::rb_free() {

    if (m_root != NULL) {
        rb_free(m_root->right_child);
        rb_free(m_root->left_child);
        free(m_root);
    }
}
void RedBlackTree::printNode(rb_node* node, std::string* dotstr) {
    dotstr->append("");
    dotstr->append(std::to_string(node->key));
    dotstr->append(" [color=");
    (node->color == black) ? dotstr->append("white fillcolor=black fontcolor=white]") : dotstr->append("black fillcolor=red fontcolor=white]");
    dotstr->append(";\n");
    if (node->left_child || node->right_child) {
        dotstr->append("rank=same;");
        if (node->left_child)  dotstr->append(std::to_string(node->left_child->key).append("; "));
        if (node->right_child) dotstr->append(std::to_string(node->right_child->key).append(";"));
        dotstr->append("\n");
        if (node->left_child) dotstr->append(std::to_string(node->key).append(" -- ").append(std::to_string(node->left_child->key).append("\n")));
        if (node->right_child) dotstr->append(std::to_string(node->key).append(" -- ").append(std::to_string(node->right_child->key).append("\n")));
    }

    
}
void RedBlackTree::printPreorder(rb_node* node, std::string* dotstr) {
    if (node) {
        printNode(node, dotstr);

        /* then recur on left subtree */
        if (node->left_child)
            printPreorder(node->left_child, dotstr);

        /* now recur on right subtree */
        if (node->right_child)
            printPreorder(node->right_child, dotstr);
    }
}

void* RedBlackTree::render_dfs() {
    /* Generate dot language description of a BST */
    std::string dotstr;
    dotstr.append("graph G { node [shape=circle style=\"filled\"] \n");

    // DFS
    if (m_root != nullptr) {

        printNode(m_root, &dotstr);
        /* then recur on left subtree */
        if (m_root->left_child)
            printPreorder(m_root->left_child, &dotstr);

        /* now recur on right subtree */
        if (m_root->right_child)
            printPreorder(m_root->right_child, &dotstr);

    }
    dotstr.append("}");
    if (m_requireUpdate) {
        Agraph_t* g = agmemread(dotstr.c_str());
        gvLayout(m_gvc, g, "dot"); 
        gvRenderData(m_gvc, g, "bmp", &m_renderdata, &m_rd_len);
        m_requireUpdate = false;
        gvFreeLayout(m_gvc, g);
        agclose(g);
    }
    return(m_renderdata);
}
void RedBlackTree::free_renderdata() {
    if (m_renderdata) {
        gvFreeRenderData(m_renderdata);
        m_renderdata = nullptr;
    }
}
