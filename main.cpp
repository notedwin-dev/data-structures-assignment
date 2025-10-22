#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>

using namespace std;

class Node {
  public:
    string data;
    Node *next;

    // Default constructor
    Node() {
        data = "";
        next = NULL;
    }

    // Parameterised Constructor
    Node(string data) {
        this->data = data;
        this->next = NULL;
    }
};

// Linked list class to implement a singly linked list
class Linkedlist {

private:
    // Merge two sorted linked lists
    Node* sortedMerge(Node *a, Node *b) {
        if (!a) return b;
        if (!b) return a;

        Node *result = NULL;

        // Compare strings lexicographically
        if (a->data <= b->data) {
            result = a;
            result->next = sortedMerge(a->next, b);
        } else {
            result = b;
            result->next = sortedMerge(a, b->next);
        }
        return result;
    }
     void frontBackSplit(Node *source, Node **frontRef, Node **backRef) {
        Node *slow = source;
        Node *fast = source->next;

        // Advance 'fast' two nodes, and 'slow' one node
        while (fast) {
            fast = fast->next;
            if (fast) {
                slow = slow->next;
                fast = fast->next;
            }
        }

        *frontRef = source;
        *backRef = slow->next;
        slow->next = NULL;
    }

    // Recursive merge sort function
    Node* mergeSortRecursive(Node *h) {
        if (!h || !h->next) return h; // Base case

        Node *a, *b;
        frontBackSplit(h, &a, &b);

        a = mergeSortRecursive(a);
        b = mergeSortRecursive(b);

        return sortedMerge(a, b);
    }


  public:
      void mergeSort() {
        head = mergeSortRecursive(head);
    }
    // Default constructor
    Node *head;
    Linkedlist() {
        head = NULL;
    }

    // Function to insert a node at the start of the
    // linked list
    void insertAtHead(string data) {

        // Create the new Node
        Node *newNode = new Node(data);

        // Assign to head of the list is empty
        if (head == NULL) {
            head = newNode;
            return;
        }

        // Insert the newly created linked list at the head
        newNode->next = this->head;
        this->head = newNode;
    }

    // Function to print the linked list.
    void print() {
        Node *temp = head;

        // Check for empty list
        if (head == NULL) {
            cout << "List empty" << endl;
            return;
        }
        cout<<"the linked list"<<endl;
        // Traverse the list
        while (temp != NULL) {

            cout << temp->data << " "<<endl;
            temp = temp->next;
        }
    }
};

// Dynamic Array class
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

void find(Linkedlist llr,Linkedlist llj);

int main(){
double sum = 0;
auto begin = std::chrono::high_resolution_clock::now();
 string filename2("job_description.csv");
string filename("resume.csv");
 ifstream input(filename);
  ifstream input2(filename2);
  string line,line2;
  if (!input.is_open()) {
    cerr << "Couldn't read file: " << filename << "\n";
    return 1;
  }
 if (!input2.is_open()) {
    cerr << "Couldn't read file: " << filename2 << "\n";
    return 1;
  }
//string cvrows[100];
  //std::vector<std::vector<std::string>> csvRows;
Linkedlist llj;
//llj.print();
Linkedlist llr;
int j=0;
  while(getline(input2,line2)){
     // llj.insertAtHead(line);
      llj.insertAtHead(line2);
      //insertLL(line);
      j++;




  }
  llj.mergeSort();
int k=0;
  while(getline(input,line)){
     // llj.insertAtHead(line);
      llr.insertAtHead(line);
      //insertLL(line);
      k++;

  }
  llr.mergeSort();
//cout<<"how many records"<< k<<endl;
  // Print out our table

 //llr.print();
 find(llr,llj);
 auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    printf("Result: %.20f\n", sum);

    printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);
 return 0;

// Test Dynamic Array
MyArray arr;
    // Add elements
    arr.add(50);
    arr.add(10);
    arr.add(30);
    arr.add(20);
    arr.add(40);

    cout << "Original Array: ";
    arr.display();

    // Sort array
    arr.sortArray();
    cout << "Sorted Array: ";
    arr.display();

    // Search for an element
    int target = 30;
    int index = arr.binarySearch(target);
    if (index != -1)
        cout << "Element " << target << " found at index " << index << endl;
    else
        cout << "Element " << target << " not found." << endl;

    return 0;
  }

void find(Linkedlist llr,Linkedlist llj){
  int jobs = 0;
  Node *ptr=llr.head;
  while(ptr->next!=NULL){
    string resumeLine=ptr->data;
cout << "matching records:" << endl;

  Node *ptr2=llj.head;
   while(ptr2->next!=NULL){
    string jobline=ptr2->data;
     int x=-1;
bool present=false;

 string word="";

string s=resumeLine.substr(35);
    for (char c : s) {
        if (c == '.') {
            x=jobline.find(word);
            if (x>=0){
                present=true;
                x=-1;
            }
            else{
                present=false;
                break;
            }
        break;}

        if (c == ',') {
           x=jobline.find(word);
            if (x>=0){
                present=true;
                x=-1;
            }
            else{
                present=false;
                break;
            }
            word = "";
        }
        else {
            word += c;
        }
    }
if (present==true){
    cout<<++jobs<<"job matching\n"<<resumeLine<<jobline << endl;
    break;
}
    ptr2=ptr2->next;
  }

cout << "next job list" << endl;
 ptr=ptr->next;
}
}