#include<algorithm>
#include<string.h>
#include "skiplist.h"
#include <map>
#include <ctime>
#include <chrono>
typedef std::pair<int, int> Item;
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
    int Get(int key) const;

    // modifying member functions

    /*
    It searches the skip list for elements
    with seachKey, if there is an element
    with that key its value is reassigned to the
    newValue, otherwise it creates and splices
    a new node, of random level.
    */
    void Put(int key, int value);

    /*
    It deletes the element containing
    searchKey, if it exists.
    */
    //void Erase(int searchKey);

    //void Merge();
    static double ns_count;
private:

    struct Node {
        int startKey;
        int endKey;
        Skip_list* rangeSkipList;
        Item* dataArray;
        int size;
        // pointers to successor nodes
        std::vector<Node*> forward;

        Node(int startKey, int endKey, int level):
            startKey(startKey), endKey(endKey), forward(level, nullptr)
        {
            //dataArray = new Item[1024];
            rangeSkipList = new Skip_list();
        }
        void Insert(int key, int value) {
            Item v = Item(key, value);
            //auto it = upper_bound(dataArray, dataArray + size, v);
            //memmove(it + 1, it, (size - (it - dataArray)) * sizeof(Item));
            //*it = v;
            size ++;
        }
        void Split(Node* rightNode) {
            int mid = size >> 1;
            int count = size;
            size = mid;
            memcpy(rightNode->dataArray, dataArray + mid, (count - mid) * sizeof(Item));
            rightNode->size = count - mid;
        }
        int MidKey() {
            return dataArray[size >> 1].first;
        }
    };

    // Generates node levels in the range [1, maxLevel).
    int randomLevel() const;
    void Split(Node* node);
    void SplitNode(Node* node);
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
