#ifndef SORT_SEARCH_CPP
#define SORT_SEARCH_CPP

#include <iostream>
using namespace std;

// Custom dynamic array class
class MyArray {
private:
    int* data;
    int size;
    int capacity;

public:
    // Constructor
    MyArray(int cap = 10) {
        size = 0;
        capacity = cap;
        data = new int[capacity];
    }

    // Destructor
    ~MyArray() {
        delete[] data;
    }

    // Add an element to the array
    void add(int value) {
        if (size == capacity) {
            // Resize when full
            capacity *= 2;
            int* newData = new int[capacity];
            for (int i = 0; i < size; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
        }
        data[size++] = value;
    }

    // Display all elements
    void display() const {
        for (int i = 0; i < size; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }

    // Bubble Sort
    void sortArray() {
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (data[j] > data[j + 1]) {
                    int temp = data[j];
                    data[j] = data[j + 1];
                    data[j + 1] = temp;
                }
            }
        }
    }

    // Binary Search (requires sorted array)
    int binarySearch(int target) const {
        int left = 0, right = size - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (data[mid] == target)
                return mid; // found
            else if (data[mid] < target)
                left = mid + 1;
            else
                right = mid - 1;
        }
        return -1; // not found
    }

    // Get current size
    int getSize() const {
        return size;
    }
};

#endif // SORT_SEARCH_CPP
