#ifndef RESUME_ARRAY_HPP
#define RESUME_ARRAY_HPP

#include <iostream>
#include <string>
#include "../../models/Resume/Resume.hpp"
#include "../../models/ConfigEnum/ConfigEnum.hpp"
using namespace std;

class ResumeArray
{
private:
    Resume *resumes;
    int size;
    int capacity;

    void resize();

    static void mergeSortSkills(string *skills, int left, int right);
    static void mergeSkills(string *skills, int left, int mid, int right);
    static void quickSortSkills(string *skills, int low, int high);
    static int partitionSkills(string *skills, int low, int high);

public:
    ResumeArray();
    ~ResumeArray();
    int getSize() const;
    Resume getResume(int index) const;

    void addResume(int id, string *skills, int skillCount);
    void printResumes();

    ResumeArray *linearSearchBySkills(const string *skillSet, int skillCount, bool matchAll);
    ResumeArray *binarySearchBySkills(const string *skillSet, int skillCount, bool matchAll, SortAlgorithm sortAlgo);

    int partition(int low, int high, bool (*cmp)(const Resume &, const Resume &));
    void quickSortHelper(int low, int high, bool (*cmp)(const Resume &, const Resume &));
    void quickSort(const string &criteria);
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
