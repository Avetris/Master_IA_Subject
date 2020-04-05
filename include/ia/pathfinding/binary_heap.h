#ifndef __BINARY_HEAP_H__
#define __BINARY_HEAP_H__ 1
// A C++ program to demonstrate common Binary Heap Operations 
#include<iostream> 
#include<climits> 
#include <engine/math.h>
#include <vector>

using namespace std;

// A class for Min Heap 
template<class T, class Comp>
class BinaryHeap {
public:
    BinaryHeap() {}
    void push(T element) {
        heap.push_back(element);
        heapifyup<Comp>(heap.size() - 1);
    }
    void deleteMin() {
        if (heap.size() == 0) {
            cout << "Heap is Empty" << endl;
            return;
        }
        heap[0] = heap.at(heap.size() - 1);
        heap.pop_back();
        heapifydown<Comp>(0);
    }
    T extractMin() {
        if (heap.size() == 0) {
            return nullptr;
        }
        else
            return heap.front();
    }
    void showHeap() {
        vector <T>::iterator pos = heap.begin();
        cout << "Heap --> ";
        while (pos != heap.end()) {
            cout << *pos << " ";
            pos++;
        }
        cout << endl;
    }
    int size() {
        return heap.size();
    }
    template <typename Comp>
    void heapifydown(int index) {
        int child = left(index);
        int child1 = right(index);
        if (child >= 0 && child1 >= 0 && Comp()(heap[child], heap[child1])) {
            child = child1;
        }
        if (child > 0 && Comp()(heap[index], heap[child])) {
            swap<T>(heap[index], heap[child]);
            heapifydown<Comp>(child);
        }
    }
private:
    vector <T> heap;
    int left(int parent){
        int left = 2 * parent + 1;
        if (left < heap.size())
            return left;
        else
            return -1;
    }
    int right(int parent){
        int right = 2 * parent + 2;
        if (right < heap.size())
            return right;
        else
            return -1;
    }
    int par(int child){
        int p = (child - 1) / 2;
        if (child == 0)
            return -1;
        else
            return p;
    }
    template <typename Comp>
    void heapifyup(int index){
        if (index >= 0 && par(index) >= 0 && Comp()(heap[par(index)], heap[index])) {
            swap<T>(heap[index], heap[par(index)]);
            heapifyup<Comp>(par(index));
        }
    }
};
#endif