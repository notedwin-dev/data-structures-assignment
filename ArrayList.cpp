#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <cctype>
#include <stdexcept>

using namespace std;

// --- GLOBAL CONSTANTS ---
#define MAX_RECORDS_ASSIGNMENT 10000
#define MATCHING_LIMIT 200 // New: Limit the number of records compared (200x200=40,000 matches)
#define MAX_WORDS 500
#define MAX_MATCHES (MATCHING_LIMIT * MATCHING_LIMIT)

// --- SUPPORTING STRUCT ---
struct Match
{
    int resumeIndex;
    int jobIndex;
    int score;
};

// --- CLASS DEFINITION AND IMPLEMENTATION ---

class ArrayList {
private:
    string* data;
    int capacity;
    int size;

    // Performance metrics
    double loadTime;
    double matchTime;
    double sortTime;
    int matchCount;

    // Data structures for the task (using custom ArrayLists)
    ArrayList *jobDescriptions;
    ArrayList *resumes;
    Match *matches;

    void resize()
    {
        int newCap = capacity * 2;
        cout << "DEBUG: Resizing Array from " << capacity << " to " << newCap << endl;

        string *newData = new (nothrow) string[newCap];
        if (!newData)
        {
            cerr << "ERROR: Failed to allocate new memory for ArrayList (OOM). Aborting resize." << endl;
            return;
        }

        for (int i = 0; i < size; i++)
            newData[i] = data[i];
        delete[] data;
        data = newData;
        capacity = newCap;
    }

    string toLowercase(const string& str) {
        string result = str;
        for (char &c : result)
        {
            if (!isalnum(c) && !isspace(c))
            {
                c = ' ';
            }
            c = tolower(c);
        }
        return result;
    }

    int getMatchScore(const string &jobDesc, const string &resumeText)
    {
        string cleanJob = toLowercase(jobDesc);
        string cleanResume = toLowercase(resumeText);

        stringstream ssJob(cleanJob);
        stringstream ssResume(cleanResume);
        string jobToken, resumeToken;

        string jobWords[MAX_WORDS];
        string resumeWords[MAX_WORDS];
        int jobCount = 0;
        int resumeCount = 0;

        while (ssJob >> jobToken && jobCount < MAX_WORDS)
        {
            if (jobToken.length() > 2)
                jobWords[jobCount++] = jobToken;
        }

        while (ssResume >> resumeToken && resumeCount < MAX_WORDS)
        {
            if (resumeToken.length() > 2)
                resumeWords[resumeCount++] = resumeToken;
        }

        int score = 0;
        for (int i = 0; i < jobCount; i++)
        {
            for (int j = 0; j < resumeCount; j++)
            {
                if (jobWords[i] == resumeWords[j])
                {
                    score++;
                }
            }
        }
        return score;
    }

    void loadFromCSV(const string &filename)
    {
        ifstream file(filename);
        string line;

        if (!file.is_open())
        {
            cerr << "ERROR: Failed to open file: " << filename << endl;
            return;
        }

        getline(file, line);

        int recordCount = 0;
        while (getline(file, line) && recordCount < MAX_RECORDS_ASSIGNMENT)
        {
            if (line.length() > 0 && line.front() == '"' && line.back() == '"')
            {
                line = line.substr(1, line.length() - 2);
            }
            insert(line);
            recordCount++;
        }

        if (recordCount >= MAX_RECORDS_ASSIGNMENT)
        {
            cout << "WARNING: File '" << filename << "' was capped at " << MAX_RECORDS_ASSIGNMENT << " records." << endl;
        }

        file.close();
    }

    void bubbleSort()
    {
        for (int i = 0; i < matchCount - 1; i++) {
            for (int j = 0; j < matchCount - i - 1; j++) {
                if (matches[j].score < matches[j + 1].score) {
                    Match temp = matches[j];
                    matches[j] = matches[j + 1];
                    matches[j + 1] = temp;
                }
            }
        }
    }

public:
    ArrayList(int cap = 500)
    {
        capacity = cap;
        size = 0;
        data = new (nothrow) string[capacity];
        if (!data)
        {
            cerr << "ERROR: Failed to allocate initial memory for ArrayList (OOM)." << endl;
            capacity = 0;
        }
        jobDescriptions = nullptr;
        resumes = nullptr;
        matches = nullptr;
        loadTime = 0.0;
        matchTime = 0.0;
        sortTime = 0.0;
        matchCount = 0;
    }

    ~ArrayList()
    {
        delete[] data;
    }

    void insert(const string &value)
    {
        if (size >= capacity)
            resize();
        if (size < capacity)
            data[size++] = value;
    }

    string get(int index) const
    {
        if (index >= 0 && index < size)
            return data[index];
        return "ERROR: Index out of bounds.";
    }

    int getSize() const { return size; }
    double getLoadTime() const { return loadTime; }
    double getMatchTime() const { return matchTime; }
    double getSortTime() const { return sortTime; }
    int getMatchCount() const { return matchCount; }

    void runMatching()
    {
        jobDescriptions = new ArrayList();
        resumes = new ArrayList();
        // Allocation is now based on MATCHING_LIMIT
        matches = new (nothrow) Match[MAX_MATCHES];
        if (!matches)
        {
            cerr << "FATAL ERROR: Failed to allocate memory for match results." << endl;
            return;
        }

        cout << "\n[ArrayList] Starting data load and matching process...\n";

        // 2. Data Loading and Load Time Measurement (still loading up to 10K)
        auto startLoad = chrono::high_resolution_clock::now();
        jobDescriptions->loadFromCSV("job_description.csv");
        resumes->loadFromCSV("resume.csv");
        auto endLoad = chrono::high_resolution_clock::now();
        loadTime = chrono::duration<double, milli>(endLoad - startLoad).count();

        if (jobDescriptions->getSize() == 0 || resumes->getSize() == 0)
        {
            cerr << "FATAL ERROR: Could not load data. Aborting matching." << endl;
            delete jobDescriptions;
            delete resumes;
            delete[] matches;
            return;
        }

        // 3. Matching Time Measurement (Search - using MATCHING_LIMIT)
        auto startMatch = chrono::high_resolution_clock::now();

        // Match only the first MATCHING_LIMIT records
        int resumeCount = min(resumes->getSize(), (int)MATCHING_LIMIT);
        int jobCount = min(jobDescriptions->getSize(), (int)MATCHING_LIMIT);

        for (int i = 0; i < resumeCount; i++)
        {
            for (int j = 0; j < jobCount; j++)
            {
                int score = getMatchScore(jobDescriptions->get(j), resumes->get(i));

                if (score > 0 && matchCount < MAX_MATCHES)
                {
                    matches[matchCount++] = {i, j, score};
                }
            }
        }

        auto endMatch = chrono::high_resolution_clock::now();
        matchTime = chrono::duration<double, milli>(endMatch - startMatch).count();

        // 4. Sorting Time Measurement
        auto startSort = chrono::high_resolution_clock::now();
        bubbleSort();
        auto endSort = chrono::high_resolution_clock::now();
        sortTime = chrono::duration<double, milli>(endSort - startSort).count();

        // 5. Output: Capped to the first 20 results
        cout << "\nTop 20 Array-Based Matches (Sorted by Score):\n";
        cout << "----------------------------------------------\n";
        for (int i = 0; i < 20 && i < matchCount; i++) {
            cout << i + 1 << ". Score: " << matches[i].score
                 << " | Job #" << matches[i].jobIndex + 1
                 << " vs. Resume #" << matches[i].resumeIndex + 1 << endl;
        }

        // 6. Cleanup
        delete jobDescriptions;
        delete resumes;
        delete[] matches;

        jobDescriptions = nullptr;
        resumes = nullptr;
        matches = nullptr;
    }
};