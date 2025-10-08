#include "JobArray.hpp"
#include <fstream>
#include <sstream>
#include <vector>

// ======================= Job Implementation =======================

Job::Job() {
    skills = new string[50];
    skillCount = 0;
}

Job::Job(int id, string title, string* skills, int skillCount) {
    this->id = id;
    this->title = title;
    this->skillCount = skillCount;
    this->skills = new string[skillCount];
    for (int i = 0; i < skillCount; i++)
        this->skills[i] = skills[i];
}

// Copy constructor
Job::Job(const Job& other) {
    id = other.id;
    title = other.title;
    skillCount = other.skillCount;
    skills = new string[skillCount];
    for (int i = 0; i < skillCount; i++)
        skills[i] = other.skills[i];
}

// Copy assignment
Job& Job::operator=(const Job& other) {
    if (this != &other) {
        delete[] skills;
        id = other.id;
        title = other.title;
        skillCount = other.skillCount;
        skills = new string[skillCount];
        for (int i = 0; i < skillCount; i++)
            skills[i] = other.skills[i];
    }
    return *this;
}

Job::~Job() {
    delete[] skills;
}

// ======================= JobArray Implementation =======================

JobArray::JobArray() {
    capacity = 1000; 
    size = 0;
    jobs = new Job[capacity];
}

JobArray::~JobArray() {
    delete[] jobs;
}

void JobArray::resize() {
    capacity *= 2;
    Job* newJobs = new Job[capacity];
    for (int i = 0; i < size; i++)
        newJobs[i] = jobs[i]; // safe with copy constructor
    delete[] jobs;
    jobs = newJobs;
}

void JobArray::addJob(int id, string title, string* skills, int skillCount) {
    if (size >= capacity) resize();

    jobs[size].id = id;
    jobs[size].title = title;
    jobs[size].skillCount = skillCount;
    jobs[size].skills = new string[skillCount];
    for (int i = 0; i < skillCount; i++)
        jobs[size].skills[i] = skills[i];

    size++;
}

bool JobArray::loadFromCSV(const string& filename) {
    ifstream file(filename);
    cout << "Attempting to open: " << filename << endl;

    if (!file.is_open()) {
        cerr << "❌ Failed to open file!" << endl;
        return false;
    }

    file.rdbuf()->pubsetbuf(nullptr, 0); 

    // Handle UTF-8 BOM
    char c1 = file.peek();
    if (c1 == '\xEF') {
        file.get(); file.get(); file.get();
    }

    cout << "✅ File opened successfully!" << endl;

    string line;
    getline(file, line); // skip header

    int lineCount = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;
        lineCount++;

        stringstream ss(line);
        string idStr, title, skillsStr;

        getline(ss, idStr, ',');
        getline(ss, title, ',');
        getline(ss, skillsStr);

        int id = 0;
        try {
            id = stoi(idStr);
        } catch (...) {
            cerr << "Invalid ID on line " << lineCount << ": " << idStr << endl;
            continue;
        }

        if (!skillsStr.empty() && skillsStr.front() == '"') skillsStr.erase(0,1);
        if (!skillsStr.empty() && skillsStr.back() == '"') skillsStr.pop_back();

        vector<string> skillVec;
        string skill;
        stringstream skillSS(skillsStr);
        while (getline(skillSS, skill, ',')) {
            if (!skill.empty()) skillVec.push_back(skill);
        }

        int skillCount = skillVec.size();
        string* skillsArr = new string[skillCount];
        for (int i = 0; i < skillCount; i++)
            skillsArr[i] = skillVec[i];

        addJob(id, title, skillsArr, skillCount);
        delete[] skillsArr;
    }

    cout << "✅ Loaded " << size << " jobs\n";
    file.close();
    return true;
}

void JobArray::printJobs() {
    cout << "\n--- Job Descriptions ---\n";
    for (int i = 0; i < size; i++) {
        cout << jobs[i].id << " | " << jobs[i].title << " | ";
        for (int j = 0; j < jobs[i].skillCount; j++) {
            cout << jobs[i].skills[j];
            if (j < jobs[i].skillCount - 1) cout << ",";
        }
        cout << endl;
    }
}

// ======================= Linear Search =======================
JobArray JobArray::linearSearchBySkill(const string& skill) {
    JobArray result;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < jobs[i].skillCount; j++) {
            if (jobs[i].skills[j] == skill) {
                result.addJob(jobs[i].id, jobs[i].title, jobs[i].skills, jobs[i].skillCount);
                break;
            }
        }
    }
    return result;
}

JobArray JobArray::linearSearchByTitle(const string& title) {
    JobArray result;
    for (int i = 0; i < size; i++) {
        if (jobs[i].title == title) {
            result.addJob(jobs[i].id, jobs[i].title, jobs[i].skills, jobs[i].skillCount);
        }
    }
    return result;
}

// ======================= Merge Sort =======================
bool JobArray::compareById(const Job& a, const Job& b) {
    return a.id < b.id;
}
bool JobArray::compareByFirstSkill(const Job& a, const Job& b) {
    if (a.skillCount == 0) return true;
    if (b.skillCount == 0) return false;
    return a.skills[0] < b.skills[0];
}

bool JobArray::compareBySkillCount(const Job& a, const Job& b) {
    return a.skillCount < b.skillCount;
}

bool JobArray::compareByTitle(const Job& a, const Job& b) {
    return a.title < b.title;
}

void JobArray::merge(Job* arr, int left, int mid, int right, CompareFn compare) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Job* L = new Job[n1];
    Job* R = new Job[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (compare(L[i], R[j]))
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void JobArray::mergeSortHelper(Job* arr, int left, int right, CompareFn compare) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSortHelper(arr, left, mid, compare);
        mergeSortHelper(arr, mid + 1, right, compare);
        merge(arr, left, mid, right, compare);
    }
}

void JobArray::mergeSort(CompareFn compare) {
    if (size > 1)
        mergeSortHelper(jobs, 0, size - 1, compare);
}
