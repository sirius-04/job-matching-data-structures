#ifndef RESUME_LINKED_LIST_HPP
#define RESUME_LINKED_LIST_HPP

#include <iostream>
#include "../../models/Resume/Resume.hpp"
#include "../../models/ConfigEnum/ConfigEnum.hpp"
using namespace std;

class ResumeLinkedList
{
private:
    ResumeNode *head;
    ResumeNode *tail;
    int length;

    static void mergeSortSkills(string *skills, int left, int right);
    static void mergeSkills(string *skills, int left, int mid, int right);
    static void quickSortSkills(string *skills, int low, int high);
    static int partitionSkills(string *skills, int low, int high);

public:
    // constructor & destructor
    ResumeLinkedList();
    ~ResumeLinkedList();

    // general functions & utilities
    ResumeNode* getHead() const;
    void append(Resume data);
    void prepend(Resume data);
    void deleteFirst();
    void deleteLast();
    ResumeNode *get(int index);
    bool set(int index, const string *skills, int skillCount);
    bool insert(int index, Resume data);
    void deleteNode(int index);
    void reverse();
    int getLength();

    static string cleanString(string s);
    void printSlice();

    // linear search
    ResumeLinkedList *linearSearchResumeBySkills(const string *skillSet, int skillCount, bool matchAll);

    // binary search
    ResumeLinkedList *binarySearchResumeBySkills(const string *skillSet, int skillCount, bool matchAll, SortAlgorithm sortAlgo);

    // merge sort
    typedef bool (*CompareFn)(ResumeNode *, ResumeNode *);
    static bool compareById(ResumeNode *a, ResumeNode *b);
    static bool compareBySkillCount(ResumeNode *a, ResumeNode *b);
    static bool compareBySkill(ResumeNode *a, ResumeNode *b);

    ResumeNode *split(ResumeNode *head);
    ResumeNode *merge(ResumeNode *first, ResumeNode *second, CompareFn compare);
    ResumeNode *mergeSort(ResumeNode *head, CompareFn compare);
    void mergeSortBy(const string &criterion);

    // quick sort
    ResumeNode *sortTail();
    void quickSort(ResumeNode *low, ResumeNode *high, const string &type);

    ResumeNode *partitionBySkillCount(ResumeNode *low, ResumeNode *high);
    ResumeNode *partitionBySkill(ResumeNode *low, ResumeNode *high);

    void quickSortBySkill();
    void quickSortBySkillCount();

    // Swap Utility
    void swap(ResumeNode *a, ResumeNode *b);
};

#endif
