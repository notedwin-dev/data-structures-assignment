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
            int newCap = capacity * 2;
            string* newData = new string[newCap];
            for (int i = 0; i < size; i++) newData[i] = data[i];
            delete[] data;
            data = newData;
            capacity = newCap;
        }
        data[size++] = value;
    }

    string get(int index) const {
        if (index >= 0 && index < size) return data[index];
        return "";
    }

    int getSize() const { return size; }

    // Convert string to lowercase (manual)
    string toLowercase(const string& str) {
        string result = str;
        for (int i = 0; result[i]; i++)
            result[i] = tolower(result[i]);
        return result;
    }

    // Tokenize string into words array
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

    // Optimized keyword-based score
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

    // Matching process for array-based version
    void runMatching() {
        cout << "==============================\n";
        cout << "ARRAY-BASED MATCHING STARTED\n";
        cout << "==============================\n";

        ArrayList jobs;
        ArrayList resumes;

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

        // Simple bubble sort (descending)
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

        cout << "Top 20 Array-Based Matches:\n";
        cout << "------------------------------\n";
        for (int i = 0; i < 20 && i < matchCount; i++) {
            cout << i + 1 << ". Resume #" << matches[i].resumeIndex + 1
                 << " <-> Job #" << matches[i].jobIndex + 1
                 << " | Score: " << matches[i].score << endl;
        }

        cout << "------------------------------\n";
        cout << "Execution Time (Array): " << execTime.count() << " ms\n";
        cout << "------------------------------\n\n";
    }
};
