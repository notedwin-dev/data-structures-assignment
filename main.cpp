#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include "ArrayList.cpp"
#include "LinkedList.cpp"
using namespace std;

struct Match {
    int resumeIndex;
    int jobIndex;
    int score;
};

// --- Utility functions ---
int getMatchScore(const string& job, const string& resume) {
    int score = 0;
    stringstream ss(resume);
    string word;
    while (ss >> word) {
        string jobLower = job, wordLower = word;
        for (auto &c : jobLower) c = tolower(c);
        for (auto &c : wordLower) c = tolower(c);
        if (jobLower.find(wordLower) != string::npos)
            score++;
    }
    return score;
}

void sortMatches(Match arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].score < arr[j + 1].score) {
                Match temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// --- Array-based Matching ---
void arrayBasedMatching() {
    cout << "\n==============================\n";
    cout << "ARRAY-BASED MATCHING STARTED\n";
    cout << "==============================\n";

    ArrayList jobs(500);
    ArrayList resumes(500);
    string line;

    ifstream jobFile("job_description.csv");
    while (getline(jobFile, line)) {
        if (!line.empty()) jobs.insert(line);
    }
    jobFile.close();

    ifstream resumeFile("resume.csv");
    while (getline(resumeFile, line)) {
        if (!line.empty()) resumes.insert(line);
    }
    resumeFile.close();

    cout << "Jobs loaded: " << jobs.getSize() << endl;
    cout << "Resumes loaded: " << resumes.getSize() << endl;

    auto start = chrono::high_resolution_clock::now();

    int totalMatches = jobs.getSize() * resumes.getSize();
    Match* matches = new Match[totalMatches];
    int count = 0;

    for (int i = 0; i < resumes.getSize(); i++) {
        for (int j = 0; j < jobs.getSize(); j++) {
            int score = getMatchScore(jobs.get(j), resumes.get(i));
            matches[count++] = {i, j, score};
        }
    }

    sortMatches(matches, count);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "\nTop 20 Array-Based Matches:\n";
    cout << "------------------------------\n";
    for (int i = 0; i < 20 && i < count; i++) {
        cout << i + 1 << ". Resume #" << matches[i].resumeIndex + 1
             << " <-> Job #" << matches[i].jobIndex + 1
             << " | Score: " << matches[i].score << endl;
    }
    cout << "------------------------------\n";
    cout << "Execution Time (Array): " << duration.count() << " ms\n";
    cout << "------------------------------\n";

    delete[] matches;
}

// --- Linked List Matching ---
void linkedListBasedMatching() {
    cout << "\n==============================\n";
    cout << "LINKED LIST–BASED MATCHING STARTED\n";
    cout << "==============================\n";

    LinkedList jobList;
    LinkedList resumeList;
    string line;

    ifstream jobFile("job_description.csv");
    while (getline(jobFile, line)) {
        if (!line.empty()) jobList.insert(line);
    }
    jobFile.close();

    ifstream resumeFile("resume.csv");
    while (getline(resumeFile, line)) {
        if (!line.empty()) resumeList.insert(line);
    }
    resumeFile.close();

    cout << "Jobs loaded: " << jobList.getSize() << endl;
    cout << "Resumes loaded: " << resumeList.getSize() << endl;

    auto start = chrono::high_resolution_clock::now();

    Node* resPtr = resumeList.getHead();
    int resumeIndex = 0;
    int totalMatches = jobList.getSize() * resumeList.getSize();
    Match* matches = new Match[totalMatches];
    int count = 0;

    while (resPtr) {
        Node* jobPtr = jobList.getHead();
        int jobIndex = 0;
        while (jobPtr) {
            int score = getMatchScore(jobPtr->description, resPtr->description);
            matches[count++] = {resumeIndex, jobIndex, score};
            jobPtr = jobPtr->next;
            jobIndex++;
        }
        resPtr = resPtr->next;
        resumeIndex++;
    }

    sortMatches(matches, count);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "\nTop 20 Linked List–Based Matches:\n";
    cout << "------------------------------\n";
    for (int i = 0; i < 20 && i < count; i++) {
        cout << i + 1 << ". Resume #" << matches[i].resumeIndex + 1
             << " <-> Job #" << matches[i].jobIndex + 1
             << " | Score: " << matches[i].score << endl;
    }
    cout << "------------------------------\n";
    cout << "Execution Time (Linked List): " << duration.count() << " ms\n";
    cout << "------------------------------\n";

    delete[] matches;
}

// --- Main Program ---
int main() {
    cout << "===== JOB MATCHING COMPONENT =====\n";

    auto start = chrono::high_resolution_clock::now();

    arrayBasedMatching();
    linkedListBasedMatching();

    auto end = chrono::high_resolution_clock::now();
    auto total = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "\n==================================\n";
    cout << "PROGRAM COMPLETED IN " << total.count() << " ms TOTAL\n";
    cout << "==================================\n";

    return 0;
}
