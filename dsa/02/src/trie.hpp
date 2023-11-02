#include <stddef.h>
#include <string>


class Trie {
    typedef struct trie {
        int value;
        char ch;
        trie* sibling;
        trie* child;
    } trie;

    public:
    Trie ();
    ~Trie();
    trie* insert (const char* key, int value);
    trie* remove (char* key);
    trie* lookup (char* key);
    void print();
    
    private:
    trie*  root;
    trie*  create_node();
    trie*  get_child (trie* node, char c);
    trie*  set_child (trie* node, char c, trie* child);
    trie*  remove_dfs(trie* rt, trie* parent, char* key, int* found);
};