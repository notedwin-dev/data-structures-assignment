#include <iostream>
#include <string>
using namespace std;

class ArrayList {
private:
    string* data;
    int capacity;
    int size;

public:
    ArrayList(int cap = 500) {
        capacity = cap;
        size = 0;
        data = new string[capacity];
    }

    ~ArrayList() {
        delete[] data;
    }

    void insert(const string& value) {
        if (size >= capacity) {
            cout << "Error: ArrayList full, cannot insert more items.\n";
            return;
        }
        data[size++] = value;
    }

    string get(int index) const {
        if (index >= 0 && index < size)
            return data[index];
        else
            return "";
    }

    int getSize() const {
        return size;
    }

    void display(int limit = 10) const {
        for (int i = 0; i < size && i < limit; i++)
            cout << "[" << i + 1 << "] " << data[i] << endl;
        cout << "Total Records: " << size << endl;
    }
};
