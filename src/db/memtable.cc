#include "index/btree/map.h"
class memtable {
private:
    /* data */
    char* data_ptr_;
    size_t size;
    uint_64 curOffset;
    
public:
    memtable(/* arg  */);
    ~memtable(); };


memtable::memtable(/* args */) {
}


memtable::~memtable
{
}

