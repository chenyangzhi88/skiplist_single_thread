#include "rangeskiplist.h"
#include <chrono>
#include <ctime>
// Struct RangeSkipnode member implementations
// constructor
RangeSkiplist::RangeSkiplist() :
    probability(0.5),
    maxLevel(24)
{
    int headKey = std::numeric_limits<int>::min();
    int nilKey = std::numeric_limits<int>::max();
    head = new Node(headKey, headKey, maxLevel);
    NIL = new Node(nilKey, nilKey, maxLevel);
    int level = randomLevel();
    Node* firstNode = makeNode(headKey+1, nilKey-1, level);

    std::fill(head->forward.begin(), head->forward.begin() + level, firstNode);
    std::fill( head->forward.begin() + level, head->forward.end(), NIL);
    std::fill(firstNode->forward.begin(), firstNode->forward.begin() + level, NIL);
}

RangeSkiplist::~RangeSkiplist() {
    auto node = head;
    while (node->forward[0]) {
        auto tmp = node;
        node = node->forward[0];
        //delete tmp->rangeSkipList;
        //delete tmp;
    }
    delete node;
}

int RangeSkiplist::Get(int key) const {
    int res;
    if (auto x = between_bound(key)) {
        res = x->childNode.find(key)->second;
    }
    return res;
}

void RangeSkiplist::GetRanges(std::vector<int> keys, std::vector<Range>* rangs) {
    for (auto it = keys.begin(); it != keys.end(); it++) {
        if(auto x = between_bound(key)) {
            Range r = Range(x.startKey, x.endKey);
            r.node = x->rangeSkipList;
            ranges->push_back(r);
        } else {
            rangs->push_back(Range(min, max, nullptr));
        }
    }
    return;
}

void RangeSkiplist::Print() const {
    Node* list = head->forward[0];
    int lineLenght = 0;

    std::cout << "{";

    while (list != NIL) {
        std::cout << "startKey: " << list->startKey
            << ", endKey: " << list->endKey << ", size: " << list->size
            << ", level: " << nodeLevel(list) << ", node:"<<std::endl;
        //list->rangeSkipList->Print();
	    //for(int i=0; i < list->size; i++) {
	        //std::cout << list->dataArray[i].first << "->";
	    //}
        //for(auto it = list->childNode.begin(); it != list->childNode.end(); it++) {
        //    std::cout << it->first << "->";
        //}
        std::cout << std::endl;
        list = list->forward[0];

        if (list != NIL) std::cout << " : ";

        if (++lineLenght % 2 == 0) std::cout << "\n";
    }
    std::cout << "}\n";
}
double RangeSkiplist::ns_count = 0.0;
double RangeSkiplist::split_count = 0.0;
double RangeSkiplist::range_count = 0.0;
void RangeSkiplist::Put(int key, int value) {
    auto start0 = std::chrono::system_clock::now();
    auto x = between_bound(key);
    auto end0 = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end0 - start0;
	range_count += elapsed_seconds.count();
    if(x) {
        if (x != NIL) {
            auto start0 = std::chrono::system_clock::now();
            //x->rangeSkipList->Put(key, value);
	        //x->Insert(key, value);
            if (x->MapInsert(key, value)) {
                x->size++;
            }
            auto end0 = std::chrono::system_clock::now();
	        std::chrono::duration<double> elapsed_seconds = end0 - start0;
	        ns_count += elapsed_seconds.count();
            if(x->size == ARRAYSIZE) {
                auto start0 = std::chrono::system_clock::now();
                //Split(x);
                SplitMap(x);
                auto end0 = std::chrono::system_clock::now();
	            std::chrono::duration<double> elapsed_seconds = end0 - start0;
	            split_count += elapsed_seconds.count();
            }
        }
    }
}
/*
void RangeSkiplist::Erase(int key) {
    if (auto x = between_bound(key)) {
        if (x != NIL) {
            x->rangeSkipList->Erase(key);
        }
    }
}
*/
void RangeSkiplist::EraseNode(Node* node) {
    Node* x = head;
    for (unsigned int i = nodeLevel(head); i-- > 0;) {
        while ( x->forward[i] != node) {
            x = x->forward[i];
        }
        if (x->forward[i] == node) {
            x->forward[i] = node->forward[i];
        }
    }
}

void RangeSkiplist::InsertNode(Node* node) {
    auto preds = predecessors(node->startKey);
    // connect pointers of predecessors and new node to respective successors
    auto level = nodeLevel(node);
    for (int i = 0; i < level; ++i) {
        node->forward[i] = preds[i]->forward[i];
        preds[i]->forward[i] = node;
    }
}

//###### private member functions ######
int RangeSkiplist::nodeLevel(const Node* v) {
    return v->forward.size();
}

RangeSkiplist::Node* RangeSkiplist::makeNode(int startKey, int endKey, int level) {
    return new Node(startKey, endKey, level);
}

int RangeSkiplist::randomLevel() const {
    int v = 1;
    while (((double)std::rand() / RAND_MAX) < probability &&
        v < maxLevel) {
        v++;
    }
    return v;
}

std::vector<RangeSkiplist::Node*> RangeSkiplist::predecessors(int startKey) const {
    std::vector<Node*> result(nodeLevel(head), nullptr);
    Node* x = head;

    for (unsigned int i = nodeLevel(head); i-- > 0;) {
        while (x->forward[i]->startKey < startKey) {
            x = x->forward[i];
        }
        result[i] = x;
    }
    return result;
}

RangeSkiplist::Node* RangeSkiplist::between_bound(int key) const{
    Node* x = head;

    for (unsigned int i = nodeLevel(head); i-- > 0;) {
        while (x->forward[i]->startKey <= key) {
            x = x->forward[i];
            if (x->endKey > key) {
                return x;
            }
        }
    }
    return nullptr;
}

void RangeSkiplist::Split(Node* node) {
    int count = node->size / 2;
    Skip_list* skipList = node->rangeSkipList;
    Skip_list* rightList = new Skip_list();
    int midKey = skipList->numberthKey(count+1);
    skipList->Split(midKey, rightList);
    int rightLevel = randomLevel();
    Node* rightNode = makeNode(midKey, node->endKey, rightLevel);
    node->endKey = midKey;
    rightNode->rangeSkipList = rightList;
    rightNode->size = node->size - count;
    node->size = count;
    InsertNode(rightNode);
}

void RangeSkiplist::SplitNode(Node* node) {
    int midKey = node->MidKey();
    int rightLevel = randomLevel();
    Node* rightNode = makeNode(midKey, node->endKey, rightLevel);
    Node* leftNode = node;
    node->Split(rightNode);
    InsertNode(rightNode);
    leftNode->endKey = midKey;
}

void RangeSkiplist::SplitMap(Node* node) {
    int rightLevel = randomLevel();
    int midKey = 0;
    Node* rightNode = makeNode(midKey, node->endKey, rightLevel);
    Node* leftNode = node;
    node->SplitMap(rightNode);
    InsertNode(rightNode);
}
/*
RangeSkiplist::Merge() {
    //delete a node and iterate the deleted node element to put another node
    //
}
*/
