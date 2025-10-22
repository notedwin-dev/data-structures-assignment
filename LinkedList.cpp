#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
using namespace std;

struct Node {
    string description;
    Node* next;
    Node(string desc) : description(desc), next(nullptr) {}
};

struct LMatch {
    int resumeIndex;
    int jobIndex;
    int score;
};

class LinkedList {
private:
    Node* head;
    int size;

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

    void sortMatches(LMatch arr[], int n) {
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j].score < arr[j + 1].score) {
                    LMatch temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }

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

    // ========== Matching Logic ==========
    void runMatching(const string& jobFilePath, const string& resumeFilePath, int maxRecords = 200) {
        cout << "\n==============================\n";
        cout << "LINKED LIST–BASED MATCHING STARTED\n";
        cout << "==============================\n";

        LinkedList jobList;
        LinkedList resumeList;
        string line;

        ifstream jobFile(jobFilePath);
        while (getline(jobFile, line) && jobList.getSize() < maxRecords) {
            if (!line.empty()) jobList.insert(line);
        }
        jobFile.close();

        ifstream resumeFile(resumeFilePath);
        while (getline(resumeFile, line) && resumeList.getSize() < maxRecords) {
            if (!line.empty()) resumeList.insert(line);
        }
        resumeFile.close();

        cout << "Jobs loaded: " << jobList.getSize() << endl;
        cout << "Resumes loaded: " << resumeList.getSize() << endl;

        auto start = chrono::high_resolution_clock::now();

        Node* resPtr = resumeList.getHead();
        int resumeIndex = 0;
        int totalMatches = jobList.getSize() * resumeList.getSize();
        LMatch* matches = new LMatch[totalMatches];
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
};
