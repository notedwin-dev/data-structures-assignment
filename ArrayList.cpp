#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
using namespace std;

struct Match {
    int resumeIndex;
    int jobIndex;
    int score;
};

class ArrayList {
private:
    string* data;
    int capacity;
    int size;

    void expandCapacity() {
        int newCapacity = capacity * 2;
        string* newData = new string[newCapacity];
        for (int i = 0; i < size; i++)
            newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCapacity;
        cout << "[Info] ArrayList expanded to capacity: " << capacity << endl;
    }

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
        if (size >= capacity)
            expandCapacity();
        data[size++] = value;
    }

    string get(int index) const {
        if (index >= 0 && index < size)
            return data[index];
        else
            return "";
    }

    int getSize() const {
        return size;
    }

    void display(int limit = 10) const {
        for (int i = 0; i < size && i < limit; i++)
            cout << "[" << i + 1 << "] " << data[i] << endl;
        cout << "Total Records: " << size << endl;
    }

    // ========== Matching Logic ==========
    void runMatching(const string& jobFilePath, const string& resumeFilePath, int maxRecords = 200) {
        cout << "\n==============================\n";
        cout << "ARRAY-BASED MATCHING STARTED\n";
        cout << "==============================\n";

        ArrayList jobs(500);
        ArrayList resumes(500);
        string line;

        ifstream jobFile(jobFilePath);
        while (getline(jobFile, line) && jobs.getSize() < maxRecords) {
            if (!line.empty()) jobs.insert(line);
        }
        jobFile.close();

        ifstream resumeFile(resumeFilePath);
        while (getline(resumeFile, line) && resumes.getSize() < maxRecords) {
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
};
