#include "skiplist.h"

class RangeSkiplist {
public:
    RangeSkiplist();
    ~RangeSkiplist();

    // non-modifying member functions

    /*
    It prints the key, value, level
    of each node of the skip list.

    Prints two nodes per line.
    */
    void Print() const;

    /*
    It searches the skip list and
    returns the element corresponding
    to the searchKey; otherwise it returns
    failure, in the form of null pointer.
    */
    std::string* Get(int key) const;

    // modifying member functions

    /*
    It searches the skip list for elements
    with seachKey, if there is an element
    with that key its value is reassigned to the
    newValue, otherwise it creates and splices
    a new node, of random level.
    */
    void Put(int key, const std::string& value);

    /*
    It deletes the element containing
    searchKey, if it exists.
    */
    void Erase(int searchKey);

    //void Merge();

private:

    struct Node {
        int startKey;
        int endKey;
        Skip_list* rangeSkipList;
        int size;
        // pointers to successor nodes
        std::vector<Node*> forward;

        Node(int startKey, int endKey, int level):
            startKey(startKey), endKey(endKey), forward(level, nullptr)
        {}
    };

    // Generates node levels in the range [1, maxLevel).
    int randomLevel() const;
    void Split(Node* node);
    void EraseNode(Node* node);
    void InsertNode(Node* node);
    //Returns number of incoming and outgoing pointers
    static int nodeLevel(const Node* v);

    //creates a node on the heap and returns a pointer to it.
    static Node* makeNode(int startKey, int endKey, int level);

    // Returns the node for which node->startKey < searchKey and node->endKey > searchKey
    Node* between_bound(int key) const;

    /*
    * Returns a collection of Pointers to Nodes
    * result[i] hold the last node of level i+1 for which result[i]->key < searchKey is true
    */
    std::vector<Node*> predecessors(int key) const ;

    // data members
    const float probability;
    const int maxLevel;
    Node* head; // pointer to first node
    Node* NIL;  // last node
};