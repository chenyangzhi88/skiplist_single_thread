#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

class Skip_list {
public:
    Skip_list();
    ~Skip_list();

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
    void Split(int midKey, Skip_list* rightList);
    int numberthKey(int nth);
private:

    struct Node {
        int key;
        std::string value;

        // pointers to successor nodes
        std::vector<Node*> forward;

        Node(int k, const std::string& v, int level):
            key(k), value(v),forward(level, nullptr)
        {}
    };

    // Generates node levels in the range [1, maxLevel).
    int randomLevel() const;

    //Returns number of incoming and outgoing pointers
    static int nodeLevel(const Node* v);

    //creates a node on the heap and returns a pointer to it.
    static Node* makeNode(int key, std::string val, int level);

    // Returns the first node for which node->key < searchKey is false
    Node* lower_bound(int key) const ;

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