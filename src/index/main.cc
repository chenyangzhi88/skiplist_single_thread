#include <iostream>
#include <time.h>
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <chrono>
#include <ctime>
#include <stdlib.h>
#include <string.h>
//#include "skiplist.h"
#include "rangeskiplist.h"
void random_array(int* array, int size)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, size); // define the range

    for(int n=0; n<size; ++n) {
        array[n] = distr(gen); // generate numbers
    }
}

int randomLevel() {
    int v = 1;
    while (((double)std::rand() / RAND_MAX) < 0.5 &&
        v < 16) {
        v++;
    }
    return v;
}

//==================================================
int main() {

    // 1.Initialize an empty Skip_list object
    Skip_list s;
    RangeSkiplist rangeList;
    const int size = 1000 * 1000 * 100;
    int* array = new int[size];
    random_array(array, size);
    std::map<int, int> testmap;
    auto start0 = std::chrono::system_clock::now();
    // 2. insert()
    for (int i = 0; i < size; ++i) {
        //s.Put(array[i], array[i]);
        rangeList.Put(array[i], array[i]);
        //testmap.insert(std::pair<int,int>(array[i],array[i]));
    }
    auto end0 = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end0 - start0;
    time_t end_time = std::chrono::system_clock::to_time_t(end0);
    std::cout << "first finished computation at " << std::ctime(&end_time) << "elapsed time: " << elapsed_seconds.count() << "s\n";
    std::cout << "put elapse time" << rangeList.ns_count << "s\n";
    std::cout << "split elapse time" << rangeList.split_count << "s\n";
    std::cout << "range elapse time" << rangeList.range_count << "s\n";
    {
        auto start0 = std::chrono::system_clock::now();
        // 2. insert()
        for (int i = 0; i < size; ++i) {
            //s.Put(array[i], array[i]);
            rangeList.Get(array[i]);
        }
        auto end0 = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end0 - start0;
        time_t end_time = std::chrono::system_clock::to_time_t(end0);
        std::cout << "first finished computation at " << std::ctime(&end_time) << "elapsed time: " << elapsed_seconds.count() << "s\n";
    }
    //rangeList.Print();
    // 2a. print()
    //s.Print();
    // 5a. print();
    //s.Print();
    std::cout << std::endl;
    std::cout << "\nDone---------------!\n";
}
