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


  public:
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
Linkedlist llr;
int j=0;
  while(getline(input2,line2)){
     // llj.insertAtHead(line);
      llj.insertAtHead(line2);
      //insertLL(line);
      j++;




  }
int k=0;
  while(getline(input,line)){
     // llj.insertAtHead(line);
      llr.insertAtHead(line);
      //insertLL(line);
      k++;




  }
cout<<"how many records"<< k<<endl;
  // Print out our table

 //llr.print();
 find(llr,llj);
 auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    printf("Result: %.20f\n", sum);

    printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);
 return 0;
  }
void find(Linkedlist llr,Linkedlist llj){
  int jobs = 0;
  Node *ptr=llr.head;
  while(ptr->next!=NULL){
    string resumeLine=ptr->data;


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



 ptr=ptr->next;

}
}
