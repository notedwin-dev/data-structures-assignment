#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <cctype>
using namespace std;

#define MAX_RECORDS 200
#define MAX_WORDS 200
#define MAX_WORD_LENGTH 50

struct Node {
    string data;
    Node* next;
    Node(string val) : data(val), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}

    ~LinkedList() {
        Node* curr = head;
        while (curr) {
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }

    void insert(const string& value) {
        Node* newNode = new Node(value);
        if (!head) head = newNode;
        else {
            Node* curr = head;
            while (curr->next) curr = curr->next;
            curr->next = newNode;
        }
        size++;
    }

    string get(int index) const {
        Node* curr = head;
        int count = 0;
        while (curr) {
            if (count == index) return curr->data;
            curr = curr->next;
            count++;
        }
        return "";
    }

    int getSize() const { return size; }

    // Lowercase converter
    string toLowercase(const string& str) {
        string result = str;
        for (int i = 0; result[i]; i++)
            result[i] = tolower(result[i]);
        return result;
    }

    // Tokenizer
    int tokenize(const string& text, string words[MAX_WORDS]) {
        string word = "";
        int count = 0;
        for (int i = 0; i <= text.length(); i++) {
            char c = text[i];
            if (isalnum(c)) word += tolower(c);
            else if (!word.empty()) {
                words[count++] = word;
                word = "";
                if (count >= MAX_WORDS) break;
            }
        }
        if (!word.empty() && count < MAX_WORDS) words[count++] = word;
        return count;
    }

    int getMatchScore(const string& job, const string& resume) {
        string jobWords[MAX_WORDS];
        string resumeWords[MAX_WORDS];
        int jobCount = tokenize(job, jobWords);
        int resCount = tokenize(resume, resumeWords);

        int score = 0;
        for (int i = 0; i < resCount; i++) {
            for (int j = 0; j < jobCount; j++) {
                if (resumeWords[i] == jobWords[j]) score++;
            }
        }
        return score;
    }

    void runMatching() {
        cout << "==============================\n";
        cout << "LINKED LIST–BASED MATCHING STARTED\n";
        cout << "==============================\n";

        LinkedList jobs;
        LinkedList resumes;

        ifstream jobFile("job_description.csv");
        string line;
        while (getline(jobFile, line) && jobs.getSize() < MAX_RECORDS) {
            if (!line.empty()) jobs.insert(toLowercase(line));
        }
        jobFile.close();

        ifstream resumeFile("resume.csv");
        while (getline(resumeFile, line) && resumes.getSize() < MAX_RECORDS) {
            if (!line.empty()) resumes.insert(toLowercase(line));
        }
        resumeFile.close();

        cout << "Jobs loaded: " << jobs.getSize() << endl;
        cout << "Resumes loaded: " << resumes.getSize() << endl << endl;

        auto start = chrono::high_resolution_clock::now();

        struct Match {
            int resumeIndex;
            int jobIndex;
            int score;
        } matches[40000];

        int matchCount = 0;

        for (int i = 0; i < resumes.getSize(); i++) {
            for (int j = 0; j < jobs.getSize(); j++) {
                int score = getMatchScore(jobs.get(j), resumes.get(i));
                if (score > 0 && matchCount < 40000) {
                    matches[matchCount++] = { i, j, score };
                }
            }
        }

        // Bubble sort
        for (int i = 0; i < matchCount - 1; i++) {
            for (int j = 0; j < matchCount - i - 1; j++) {
                if (matches[j].score < matches[j + 1].score) {
                    Match temp = matches[j];
                    matches[j] = matches[j + 1];
                    matches[j + 1] = temp;
                }
            }
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> execTime = end - start;

        cout << "Top 20 Linked List–Based Matches:\n";
        cout << "------------------------------\n";
        for (int i = 0; i < 20 && i < matchCount; i++) {
            cout << i + 1 << ". Resume #" << matches[i].resumeIndex + 1
                 << " <-> Job #" << matches[i].jobIndex + 1
                 << " | Score: " << matches[i].score << endl;
        }

        cout << "------------------------------\n";
        cout << "Execution Time (Linked List): " << execTime.count() << " ms\n";
        cout << "------------------------------\n\n";
    }
};
