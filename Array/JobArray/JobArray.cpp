#include "JobArray.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Job::Job(int id, const string& title, const vector<string>& skills)
    : id(id), title(title), skills(skills) {}

// ======================= ArrayList-like methods =======================
void JobArray::add(const Job& job) {
    jobs.push_back(job);
}

Job& JobArray::get(int index) {
    if (index < 0 || index >= size()) throw out_of_range("Index out of range");
    return jobs[index];
}

const Job& JobArray::get(int index) const {
    if (index < 0 || index >= size()) throw out_of_range("Index out of range");
    return jobs[index];
}

void JobArray::remove(int index) {
    if (index < 0 || index >= size()) throw out_of_range("Index out of range");
    jobs.erase(jobs.begin() + index);
}

int JobArray::size() const {
    return jobs.size();
}

bool JobArray::containsJob(int id) const {
    for (const auto& job : jobs) {
        if (job.id == id) return true;
    }
    return false;
}

Job& JobArray::operator[](int index) { return get(index); }
const Job& JobArray::operator[](int index) const { return get(index); }

// ======================= CSV =======================
bool JobArray::loadFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file!\n";
        return false;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string idStr, title, skillsStr;
        getline(ss, idStr, ',');
        getline(ss, title, ',');
        getline(ss, skillsStr);

        int id = stoi(idStr);

        if (!skillsStr.empty() && skillsStr.front() == '"') skillsStr.erase(0, 1);
        if (!skillsStr.empty() && skillsStr.back() == '"') skillsStr.pop_back();

        vector<string> skillVec;
        string skill;
        stringstream skillSS(skillsStr);
        while (getline(skillSS, skill, ',')) {
            if (!skill.empty()) skillVec.push_back(skill);
        }

        add(Job(id, title, skillVec));
    }

    return true;
}

void JobArray::printJobs() const {
    for (const auto& job : jobs) {
        cout << job.id << " | " << job.title << " | ";
        for (size_t i = 0; i < job.skills.size(); i++) {
            cout << job.skills[i];
            if (i < job.skills.size() - 1) cout << ",";
        }
        cout << endl;
    }
}

// ======================= Search =======================
JobArray JobArray::linearSearchBySkill(const string& skill) const {
    JobArray result;
    for (const auto& job : jobs) {
        if (find(job.skills.begin(), job.skills.end(), skill) != job.skills.end()) {
            result.add(job);
        }
    }
    return result;
}

// ======================= Merge Sort =======================
void JobArray::merge(vector<Job>& arr, int left, int mid, int right, CompareFn compare) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Job> L(arr.begin() + left, arr.begin() + mid + 1);
    vector<Job> R(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (compare(L[i], R[j])) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void JobArray::mergeSortHelper(vector<Job>& arr, int left, int right, CompareFn compare) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid, compare);
        mergeSortHelper(arr, mid + 1, right, compare);
        merge(arr, left, mid, right, compare);
    }
}

void JobArray::mergeSort(CompareFn compare) {
    if (!jobs.empty()) {
        mergeSortHelper(jobs, 0, jobs.size() - 1, compare);
    }
}

// ======================= Comparison functions =======================
bool JobArray::compareById(const Job& a, const Job& b) { return a.id < b.id; }
bool JobArray::compareByTitle(const Job& a, const Job& b) { return a.title < b.title; }
bool JobArray::compareBySkillCount(const Job& a, const Job& b) { return a.skills.size() < b.skills.size(); }
bool JobArray::compareByFirstSkill(const Job& a, const Job& b) {
    if (a.skills.empty()) return true;
    if (b.skills.empty()) return false;
    return a.skills[0] < b.skills[0];
}
