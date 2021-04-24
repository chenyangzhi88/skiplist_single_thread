#include<iostream>
#include<random>
#include <chrono>
#include <ctime>
//#include<map>
#include "map.h"
using namespace std;
void random_array(int* array, int size)
{
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, size); // define the range

	for(int n=0; n<size; ++n) {
		array[n] = distr(gen); // generate numbers
	}
}

int main() {
	const int size = 1024 * 64;
	int* array = new int[size];
	random_array(array, size);
	btree::map<int,int> testmap;
	auto start0 = std::chrono::system_clock::now();
	for(int i = 0; i < size; i++) {
		testmap.insert(std::pair<int, int>(array[i],array[i]));
	}
	auto end0 = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end0 - start0;
	time_t end_time = std::chrono::system_clock::to_time_t(end0);
	std::cout << "first finished computation at " << std::ctime(&end_time) << "elapsed time: " << elapsed_seconds.count() << "s\n";
	cout<<"traversal of constructed B tree\n";
	//traverse(r);
}
