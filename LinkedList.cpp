#include <iostream>
#include <string>
using namespace std;

struct Node {
    string description;
    Node* next;
    Node(string desc) : description(desc), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}

    void insert(string value) {
        Node* newNode = new Node(value);
        if (!head) head = newNode;
        else {
            Node* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
        size++;
    }

    Node* getHead() { return head; }
    int getSize() { return size; }

    void display(int limit = 10) {
        Node* temp = head;
        int count = 0;
        while (temp && count < limit) {
            cout << "[" << count + 1 << "] " << temp->description << endl;
            temp = temp->next;
            count++;
        }
        cout << "Total Records: " << size << endl;
    }
};
