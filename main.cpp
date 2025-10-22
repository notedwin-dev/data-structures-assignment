#include <iostream>
#include "ArrayList.cpp"
#include "LinkedList.cpp"
using namespace std;

int main() {
    cout << "===== JOB MATCHING COMPONENT =====\n\n";

    ArrayList arrayMatch;
    arrayMatch.runMatching();

    LinkedList linkedMatch;
    linkedMatch.runMatching();

    cout << "===== PROGRAM COMPLETED SUCCESSFULLY =====\n";
    return 0;
}
