#include "skiplist.h"

Skip_list::Skip_list() :
    probability(0.5),
    maxLevel(16)
{
    int headKey = std::numeric_limits<int>::min();
    head = new Node(headKey, "head", maxLevel);

    int nilKey = std::numeric_limits<int>::max();
    NIL = new Node(nilKey, "NIL", maxLevel);

    std::fill(head->forward.begin(), head->forward.end(), NIL);
}

Skip_list::~Skip_list() {
    auto node = head;
    while (node->forward[0]) {
        auto tmp = node;
        node = node->forward[0];
        delete tmp;
    }
    delete node;
}

std::string* Skip_list::Get(int key) const {
    std::string* res{};
    if (auto x = lower_bound(key)) {
        if (x->key == key && x != NIL) {
            res = &(x->value);
        }
    }
    return res;
}

void Skip_list::Print() const {
    Node* list = head->forward[0];
    int lineLenght = 0;

    std::cout << "{";

    while (list != NIL) {
        std::cout << "value: " << list->value
            << ", key: " << list->key
            << ", level: " << nodeLevel(list);

        list = list->forward[0];

        if (list != NIL) std::cout << " : ";

        if (++lineLenght % 2 == 0) std::cout << "\n";
    }
    std::cout << "}\n";
}

void Skip_list::Put(int key, const std::string& value) {
    auto preds = predecessors(key);

    {//reassign value if node exists and return
        auto next = preds[0]->forward[0];
        if (next->key == key && next != NIL) {
            next->value = value;
            return;
        }
    }

    // create new node
    const int newNodeLevel = randomLevel();
    auto newNodePtr = makeNode(key, value, newNodeLevel);

    // connect pointers of predecessors and new node to respective successors
    for (int i = 0; i < newNodeLevel; ++i) {
        newNodePtr->forward[i] = preds[i]->forward[i];
        preds[i]->forward[i] = newNodePtr;
    }
}


void Skip_list::Erase(int key) {
    auto preds = predecessors(key);

    //check if the node exists
    auto node = preds[0]->forward[0];
    if (node->key != key || node == NIL) {
        return;
    }

    // update pointers and delete node
    for (size_t i = 0; i < nodeLevel(node); ++i) {
        preds[i]->forward[i] = node->forward[i];
    }
    delete node;
}

//###### private member functions ######
int Skip_list::nodeLevel(const Node* v) {
    return v->forward.size();
}

Skip_list::Node* Skip_list::makeNode(int key, std::string val, int level)  {
    return new Node(key, val, level);
}

int Skip_list::randomLevel() const {
    int v = 1;
    while (((double)std::rand() / RAND_MAX) < probability &&
        v < maxLevel) {
        v++;
    }
    return v;
}

Skip_list::Node* Skip_list::lower_bound(int key) const{
    Node* x = head;

    for (unsigned int i = nodeLevel(head); i-- > 0;) {
        while ( x->forward[i]->key < key) {
            x = x->forward[i];
        }
    }
    return x->forward[0];
}

std::vector<Skip_list::Node*> Skip_list::predecessors(int key) const {
    std::vector<Node*> result(nodeLevel(head),nullptr);
    Node* x = head;

    for (unsigned int i = nodeLevel(head); i-- > 0;) {
        while (x->forward[i]->key < key) {
            x = x->forward[i];
        }
        result[i] = x;
    }
    return result;
}

void Skip_list::Split(int key, Skip_list* rightList) {
    auto preds = predecessors(key);

    // create new node
    int nilKey = std::numeric_limits<int>::max();
    NIL = new Node(nilKey, "NIL", maxLevel);
    int headKey = std::numeric_limits<int>::min();
    head = new Node(headKey, "head", maxLevel);
    //for leftNode
    // connect pointers of predecessors and new node to respective successors
    for (int i = 0; i < maxLevel; ++i) {
        preds[i]->forward[i] = NIL;
    }
    rightList->NIL = this->NIL;
    this->NIL = NIL;
    //for rightNode
    for (int i = 0; i < maxLevel; ++i) {
        head->forward[i] = preds[i]->forward[i];
    }
    rightList->head = head;
}

int Skip_list::numberthKey(int nth) {
    Node* list = head->forward[0];
    int size = 0;
    while (list != NIL && size != nth) {
        list = list->forward[0];
    }
    return list->key;
}