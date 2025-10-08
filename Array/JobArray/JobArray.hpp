#ifndef JOB_ARRAY_HPP
#define JOB_ARRAY_HPP

#include <iostream>
#include <string>
using namespace std;

class Job {
public:
    int id;
    string title;
    string* skills;
    int skillCount;

    Job();
    Job(int id, string title, string* skills, int skillCount);
    Job(const Job& other);             
    Job& operator=(const Job& other);  
    ~Job();
};

class JobArray {
private:
    Job* jobs;
    int size;
    int capacity;

    void resize();

public:
    JobArray();
    ~JobArray();

    // Core
    void addJob(int id, string title, string* skills, int skillCount);
    bool loadFromCSV(const string& filename);
    void printJobs();

    // Linear search
    JobArray linearSearchBySkill(const string& skill);

    // Merge sort
    typedef bool (*CompareFn)(const Job&, const Job&);
    static bool compareById(const Job& a, const Job& b);
    static bool compareBySkillCount(const Job& a, const Job& b);
    static bool compareByTitle(const Job& a, const Job& b);
    static bool compareByFirstSkill(const Job& a, const Job& b);

    void mergeSort(CompareFn compare);
    void mergeSortHelper(Job* arr, int left, int right, CompareFn compare);
    void merge(Job* arr, int left, int mid, int right, CompareFn compare);
};

#endif
