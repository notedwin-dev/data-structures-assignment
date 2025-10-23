#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <cctype>

using namespace std;

// --- GLOBAL CONSTANTS (Using new limits) ---
#define LL_MAX_RECORDS_ASSIGNMENT 10000
#define LL_MATCHING_LIMIT 200 // New: Limit the number of records compared
#define LL_MAX_WORDS 500
#define LL_MAX_MATCHES (LL_MATCHING_LIMIT * LL_MATCHING_LIMIT)

// --- SUPPORTING STRUCTS ---
struct Node {
    string data;
    Node* next;
    Node(string val) : data(val), next(nullptr) {}
};

struct LLMatch
{
    int resumeIndex;
    int jobIndex;
    int score;
};

// --- CLASS DEFINITION AND IMPLEMENTATION ---

class LinkedList {
private:
    Node* head;
    int size;

    // Performance metrics
    double loadTime;
    double matchTime;
    double sortTime;
    int matchCount;

    // Data structures for the task
    LinkedList *jobDescriptions;
    LinkedList *resumes;
    LLMatch *matches;

    string toLowercase(const string &str)
    {
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

        string jobWords[LL_MAX_WORDS];
        string resumeWords[LL_MAX_WORDS];
        int jobCount = 0;
        int resumeCount = 0;

        while (ssJob >> jobToken && jobCount < LL_MAX_WORDS)
        {
            if (jobToken.length() > 2)
                jobWords[jobCount++] = jobToken;
        }
        while (ssResume >> resumeToken && resumeCount < LL_MAX_WORDS)
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
        while (getline(file, line) && recordCount < LL_MAX_RECORDS_ASSIGNMENT)
        {
            if (line.length() > 0 && line.front() == '"' && line.back() == '"')
            {
                line = line.substr(1, line.length() - 2);
            }
            insert(line);
            recordCount++;
        }

        if (recordCount >= LL_MAX_RECORDS_ASSIGNMENT)
        {
            cout << "WARNING: File '" << filename << "' was capped at " << LL_MAX_RECORDS_ASSIGNMENT << " records." << endl;
        }

        file.close();
    }

    void bubbleSort()
    {
        for (int i = 0; i < matchCount - 1; i++)
        {
            for (int j = 0; j < matchCount - i - 1; j++)
            {
                if (matches[j].score < matches[j + 1].score)
                {
                    LLMatch temp = matches[j];
                    matches[j] = matches[j + 1];
                    matches[j + 1] = temp;
                }
            }
        }
    }

public:
    LinkedList() : head(nullptr), size(0)
    {
        jobDescriptions = nullptr;
        resumes = nullptr;
        matches = nullptr;
        loadTime = 0.0;
        matchTime = 0.0;
        sortTime = 0.0;
        matchCount = 0;
    }

    ~LinkedList() {
        Node* curr = head;
        while (curr) {
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }

    void insert(const string& value) {
        Node *newNode = new (nothrow) Node(value);
        if (!newNode)
        {
            cerr << "ERROR: Failed to allocate new Node for LinkedList (OOM)." << endl;
            return;
        }

        if (!head) head = newNode;
        else {
            Node* curr = head;
            while (curr->next) curr = curr->next;
            curr->next = newNode;
        }
        size++;
    }

    string get(int index) const {
        if (index < 0 || index >= size)
        {
            return "ERROR: Index out of bounds.";
        }

        Node* curr = head;
        int count = 0;
        // O(n) traversal for access
        while (curr)
        {
            if (count == index) return curr->data;
            curr = curr->next;
            count++;
        }
        return "";
    }

    int getSize() const { return size; }
    double getLoadTime() const { return loadTime; }
    double getMatchTime() const { return matchTime; }
    double getSortTime() const { return sortTime; }
    int getMatchCount() const { return matchCount; }

    void runMatching()
    {
        jobDescriptions = new LinkedList();
        resumes = new LinkedList();
        // Allocation is now based on LL_MATCHING_LIMIT
        matches = new (nothrow) LLMatch[LL_MAX_MATCHES];
        if (!matches)
        {
            cerr << "FATAL ERROR: Failed to allocate memory for match results." << endl;
            return;
        }

        cout << "\n[LinkedList] Starting data load and matching process...\n";

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

        // 3. Matching Time Measurement (Search - using LL_MATCHING_LIMIT)
        auto startMatch = chrono::high_resolution_clock::now();

        // Match only the first LL_MATCHING_LIMIT records
        int resumeCount = min(resumes->getSize(), (int)LL_MATCHING_LIMIT);
        int jobCount = min(jobDescriptions->getSize(), (int)LL_MATCHING_LIMIT);

        // The complexity here is still O(N_match^2 * N_list * W^2), where N_list=10k, but N_match=200
        for (int i = 0; i < resumeCount; i++)
        {
            for (int j = 0; j < jobCount; j++)
            {
                // The O(N) access for LinkedList's get() is what we are measuring!
                int score = getMatchScore(jobDescriptions->get(j), resumes->get(i));

                if (score > 0 && matchCount < LL_MAX_MATCHES)
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
        cout << "\nTop 20 Linked-List Based Matches (Sorted by Score):\n";
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