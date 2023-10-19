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
    trie* insert (const std::string& key, int value);
    trie* remove (const std::string& key);
    trie* lookup (const std::string& key);
    void print  ();
    
    private:
    trie*  root;
    trie*  create_node();
    trie*  get_child(trie* node, char c);
    trie*  set_child(trie* node, char c, trie* child);
};