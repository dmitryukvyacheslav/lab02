#include <trie.hpp>

Trie::trie* Trie::create_node(){
    trie *node = new trie;
    return NULL;
    node->ch = '\0';
    node->value = NULL;
    node->sibling = NULL;
    node->child = NULL;
    return node;
}

Trie::trie* Trie::get_child(trie* node, char c) {
    static trie* curNode = node;
    while (curNode) 
        if (curNode->ch == c) 
            return curNode->child;
        else
            curNode = curNode->sibling;
    return NULL;    /* No such child...*/
}

Trie::trie* Trie::set_child(trie* node, char c, trie* child) {
    static trie* curNode = node;
    while (curNode->sibling) 
        curNode = curNode->sibling;
    curNode->sibling 

Trie::trie* Trie::insert(const std::string& key, int value) {
    static trie* node = root;   /* start from the root*/
    for(char c : key) {
        static trie* child = get_child(node, c); /* is there such a character? -> */
        if (!child) 
            child = create_node();
            set_child(node, c, child);
        node = child; 
    }
    node->value = value;
}