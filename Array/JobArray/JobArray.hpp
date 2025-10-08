#ifndef JOBARRAY_HPP
#define JOBARRAY_HPP

#include <string>
#include <vector>
#include <functional>
using namespace std;

struct Job {
    int id;
    string title;
    vector<string> skills;

    Job() = default;
    Job(int id, const string& title, const vector<string>& skills);
};

class JobArray {
private:
    vector<Job> jobs;

    // Merge sort helper functions
    void merge(vector<Job>& arr, int left, int mid, int right, function<bool(const Job&, const Job&)> compare);
    void mergeSortHelper(vector<Job>& arr, int left, int right, function<bool(const Job&, const Job&)> compare);

public:
    using CompareFn = function<bool(const Job&, const Job&)>;

    // ArrayList
    void add(const Job& job);
    Job& get(int index);
    const Job& get(int index) const;
    void remove(int index);
    int size() const;
    bool containsJob(int id) const;

    Job& operator[](int index);
    const Job& operator[](int index) const;

    // CSV operations
    bool loadFromCSV(const string& filename);
    void printJobs() const;

    // Search
    JobArray linearSearchBySkill(const string& skill) const;

    // Sorting
    void mergeSort(CompareFn compare);

    // Comparison functions
    static bool compareById(const Job& a, const Job& b);
    static bool compareByTitle(const Job& a, const Job& b);
    static bool compareBySkillCount(const Job& a, const Job& b);
    static bool compareByFirstSkill(const Job& a, const Job& b);

    // Iterators
    auto begin() { return jobs.begin(); }
    auto end() { return jobs.end(); }
    auto begin() const { return jobs.begin(); }
    auto end() const { return jobs.end(); }
};

#endif
