#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <cctype>
#include <algorithm> // Needed for std::min, which is standard C++

using namespace std;

// Function to print results with consistent formatting
void printResults(const string &name, double loadTime, double matchTime, double sortTime, int matchCount)
{
    cout << "\n============================================\n";
    cout << name << " Performance Results\n";
    cout << "============================================\n";
    cout << left << setw(20) << "Records Loaded:" << "10000 (Expected)\n";
    cout << left << setw(20) << "Matches Found:" << matchCount << "\n";
    cout << "--------------------------------------------\n";
    cout << left << setw(20) << "Data Load Time:" << fixed << setprecision(3) << loadTime << " ms\n";
    cout << left << setw(20) << "Matching Time:" << fixed << setprecision(3) << matchTime << " ms\n";
    cout << left << setw(20) << "Sorting Time:" << fixed << setprecision(3) << sortTime << " ms\n";
    cout << "--------------------------------------------\n";
    cout << left << setw(20) << "TOTAL EXECUTION:" << fixed << setprecision(3) << loadTime + matchTime + sortTime << " ms\n";
}

// Including the entire implementation directly (as requested)
#include "ArrayList.cpp"
#include "LinkedList.cpp"

int main() {
    cout << "===== JOB MATCHING COMPONENT - LAB WORK #1 =====\n\n";

    // --- Array-Based Implementation ---
    ArrayList arrayMatch;
    arrayMatch.runMatching();
    printResults(
        "Array-Based List (ArrayList)",
        arrayMatch.getLoadTime(),
        arrayMatch.getMatchTime(),
        arrayMatch.getSortTime(),
        arrayMatch.getMatchCount());

    // --- Linked List Implementation ---
    LinkedList linkedMatch;
    linkedMatch.runMatching();
    printResults(
        "Linked List (LinkedList)",
        linkedMatch.getLoadTime(),
        linkedMatch.getMatchTime(),
        linkedMatch.getSortTime(),
        linkedMatch.getMatchCount());

    cout << "\n===== PROGRAM COMPLETED SUCCESSFULLY =====\n";
    return 0;
}