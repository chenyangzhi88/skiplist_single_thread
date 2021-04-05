#include <iostream>
#include <time.h>
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <chrono>
#include <ctime>
#include "skiplist.h"
void random_array(int* array, int size)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, size); // define the range

    for(int n=0; n<size; ++n) {
        array[n] = distr(gen); // generate numbers
    }
}
//==================================================
int main() {

    // 1.Initialize an empty Skip_list object
    Skip_list s;
    const int size = 1000*1000;
    int* array = new int[size];;
    random_array(array, size);
    auto start0 = std::chrono::system_clock::now();
    // 2. insert()
    for (int i = 0; i < size; ++i) {
        s.Put(i, "v");
    }
    auto end0 = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end0 - start0;
    time_t end_time = std::chrono::system_clock::to_time_t(end0);
    std::cout << "first finished computation at " << std::ctime(&end_time) << "elapsed time: " << elapsed_seconds.count() << "s\n";
    // 2a. print()
    //s.Print();
    std::cout << std::endl;

    // 3. find()
    auto f = s.Get(10);
    if (f) std::cout << "Node found!\nvalue: " << f << '\n';
    else std::cout << "Node NOT found!\n";

    // 4. insert() - reassign
    s.Put(40, "TEST");

    // 4a. print()
    //s.Print();
    std::cout << std::endl;

    // 5. erase()
    s.Erase(40);

    // 5a. print();
    //s.Print();
    std::cout << std::endl;

    std::cout << "\nDone!\n";
    getchar();
}