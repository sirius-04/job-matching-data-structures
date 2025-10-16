#ifndef RESUME_ARRAY_HPP
#define RESUME_ARRAY_HPP

#include <iostream>
#include <string>
#include "../../models/Resume/Resume.hpp"
using namespace std;

class ResumeArray
{
private:
    Resume *resumes;
    int size;
    int capacity;

    void resize();

public:
    ResumeArray();
    ~ResumeArray();
    int getSize() const;
    Resume getResume(int index) const;

    void addResume(int id, string *skills, int skillCount);
    void printResumes();

    ResumeArray linearSearchBySkills(const string *skillSet, int skillCount, bool matchAll);
    ResumeArray binarySearchBySkills(const string *skillSet, int skillCount, bool matchAll);

    int partition(int low, int high, bool (*cmp)(const Resume &, const Resume &));
    void quickSortHelper(int low, int high, bool (*cmp)(const Resume &, const Resume &));
    void quickSortById();
    void quickSortBySkill();
    void quickSortBySkillCount();

    typedef bool (*CompareFn)(const Resume &, const Resume &);
    static bool compareById(const Resume &a, const Resume &b);
    static bool compareBySkillCount(const Resume &a, const Resume &b);
    static bool compareByFirstSkill(const Resume &a, const Resume &b);

    void mergeSort(CompareFn compare);
    void mergeSortHelper(Resume *arr, int left, int right, CompareFn compare);
    void merge(Resume *arr, int left, int mid, int right, CompareFn compare);
};

#endif
