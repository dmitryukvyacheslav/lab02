#include <stddef.h>
#include <string>


class Trie {
    

    public:
    typedef struct trie {
        int value;
        char ch;
        trie* sibling;
        trie* child;
    } trie;
    Trie();
    trie* insert (const char* key, int value);
    trie* remove (const char* key);
    trie* lookup (const char* key);
    void print();
    
    private:
    trie*  root;
    trie*  create_node();
    trie*  get_child (trie* node, char c);
    trie*  set_child (trie* node, char c, trie* child);
    trie*  remove_dfs(trie* rt, trie* parent, const char* key, int* found);
    void traverse(std::string& prefix, trie* node);
};