#include <containers\vector.hpp>
#include <iostream>
#include <thread>
#include <vector>
//
//static void addToVector(std::vector<int>& vector, size_t numElements) {
//    for (int i = 0; i < numElements; i++) {
//        vector.push_back(i);
//    }
//}
//
//static void addToCustomVector(Vector<int>& vector, size_t numElements) {
//    for (int i = 0; i < numElements; i++) {
//        vector.push_back(i);
//    }
//}

size_t testVector(bool custom) {
    std::vector<std::thread> threads;
    if (custom) {
        Vector<int> vector; 
        for (uint32_t i = 0; i < 20; ++i) {
            threads.push_back(std::thread([&] {
                for (int k = 0; k < 100; ++k) {
                    vector.push_back(k);
                }
            }));
        }

        for (auto& t : threads) {
            t.join();
        }

        return vector.size();
    }
    else {
        std::vector<int> vector;
        for (uint32_t i = 0; i < 20; ++i) {
            threads.push_back(std::thread([&] {
                for (int k = 0; k < 100; ++k) {
                   vector.push_back(k);
                }
            }));
        }

        for (auto& t : threads) {
            t.join();
        }

        return vector.size();
    }
}

int main() {
    std::cout << " Start" << std::endl;
    for (uint32_t i = 0; i < 100; ++i) {
        const size_t val = testVector(false);
        if (val != 2000) {
            std::cout << "Error in simulation " << i << ", Vector size is " << val << std::endl;
        }
        else {
            std::cout << "Correct simulation " << i << ", Vector size is " << val << std::endl;
        }
    }

    std::cout << " End" << std::endl;
    return 0;
}