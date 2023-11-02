#include <trie.hpp>

Trie::trie* Trie::create_node(){
    trie *node = new trie;
    node->ch = '\0';
    node->value = NULL;
    node->sibling = NULL;
    node->child = NULL;
    return node;
}


Trie::trie* Trie::get_child(trie* node, char c) {
    // node->child - связный список
    // у одного из его элементов должна быть буква c
    if (!node->child) return nullptr;
    Trie::trie* ll_item = node->child;
    while (ll_item)
        if (ll_item->ch == c) return ll_item;
        else ll_item = ll_item->sibling; 
    return NULL; // ничего не нашли   
}

Trie::trie* Trie::set_child(trie* node, char c, trie* child) {
    // node->child - связный список
    // в него надо добавить ноду child c буквой c
    Trie::trie* ll_item = node->child;
    
    // что если детей нет и список надо создать?
    if (!ll_item) {
        node->child = child;
        node->child->ch = c;
    }

    // может быть, такая буква уже есть?
    // пройдемся по списку, и добавим элемент, если нету
    else {
        while(1) {
            if (ll_item->ch == c) return ll_item; // есть
            else 
                if (ll_item->sibling) ll_item = ll_item->sibling;
                else break; // дошли до конца, пора вставлять элемент
        }
        ll_item->sibling = child;
        ll_item->sibling->ch = c;
    }
    return ll_item;
}

Trie::trie* Trie::insert(const char* key, int value) {
    trie* node = root;   /* start from the root*/
    int i = 0;
    while(key[i] != '\0') {
        trie* child = get_child(node, key[i]); /* is there such a character? -> */
        if (!child) 
            child = create_node();
            set_child(node, key[i], child);
        node = child; 
        child = nullptr;
        i++;
    }
    node->value = value;
    return node;
}

Trie::trie* Trie::remove_dfs(trie* rt, trie* parent, const char* key, int* found) {
    trie* node = nullptr;
    trie* prev = nullptr;

    // если дерево или ключ пустые, нет смысла что-то делать
    *found = (*key == '\0' && rt == NULL) ? 1 : 0;
    if (*key == '\0' || rt == NULL)
        return rt;

    // поиск буквы ключа
    for (node = rt; node != nullptr; node = node->sibling) {
        if (node->ch == *key)
            break;
        prev = node;            
    }
    if (node == nullptr)
        return rt; // не нашли букву

    remove_dfs(node->child, node, key+1, found);
    
    if (*found > 0 && node->child == NULL) {
        // дошли до конца, такой ключ есть - сносим его
        if (prev != nullptr)
            prev->sibling = node->sibling;
        else {
            if (parent != nullptr)
                parent->child = node->sibling;
            else
                rt = node->sibling;
        }
  
        free(node);
    }
    return rt;
}

Trie::trie* Trie::remove(const char* key){
    int found;
    remove_dfs(root->child, root, key, &found);
    return root;
}

Trie::trie* Trie::lookup(const char* key){
    trie* node = root;
    int i = 0;
    while(key[i] != '\0') {
        trie* child = get_child(node, key[i]); /* is there such a character? -> */
        if (!child) 
            return nullptr;
        node = child;
        i++;
    }
    if (node->value == NULL) 
        return nullptr;
    return node;
}

void Trie::traverse(std::string& prefix, trie* node) {
    if (node->value)
        printf("%s\t[%d]\n", prefix.c_str(), node->value);

    for (char index = 0; index < 26; ++index) {
        char next = 'a' + index;
        trie* pChild = get_child(node, next);
        if (pChild) {
            prefix.push_back(next);
            traverse(prefix, pChild);
            prefix.pop_back();
        }
    }
}

void Trie::print() {
    std::string pfx;
    traverse(pfx, root);
}

Trie::Trie() {
    root = create_node();
}