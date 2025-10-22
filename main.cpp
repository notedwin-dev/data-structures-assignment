#include <iostream>
#include "ArrayList.cpp"
#include "LinkedList.cpp"
using namespace std;

int main() {
    cout << "===== JOB MATCHING COMPONENT =====\n";

    ArrayList arrayImpl;
    LinkedList linkedImpl;

    string jobFile = "job_description.csv";
    string resumeFile = "resume.csv";

    arrayImpl.runMatching(jobFile, resumeFile, 200);   // Array version
    linkedImpl.runMatching(jobFile, resumeFile, 200);  // Linked list version

    cout << "\n===== PROGRAM COMPLETED SUCCESSFULLY =====\n";
    return 0;
}
