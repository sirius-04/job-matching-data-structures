#ifndef RESUME_CIRCULAR_LINKED_LIST_HPP
#define RESUME_CIRCULAR_LINKED_LIST_HPP

#include <iostream>
#include "../../models/Resume/Resume.hpp"
using namespace std;

class ResumeCircularLinkedList
{
private:
    ResumeNode *head;
    ResumeNode *tail;
    int length;

public:
    // constructor & destructor
    ResumeCircularLinkedList();
    ~ResumeCircularLinkedList();

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
    ResumeCircularLinkedList *linearSearchResumeBySkills(const string *skillSet, int skillCount, bool matchAll);

    // binary search
    ResumeCircularLinkedList *binarySearchResumeBySkills(const string *skillSet, int skillCount, bool matchAll);

    // merge sort
    typedef bool (*CompareFn)(ResumeNode *, ResumeNode *);
    static bool compareById(ResumeNode *a, ResumeNode *b);
    static bool compareBySkillCount(ResumeNode *a, ResumeNode *b);
    static bool compareBySkill(ResumeNode *a, ResumeNode *b);

    ResumeNode *split(ResumeNode *head, int count);
    ResumeNode *merge(ResumeNode *first, ResumeNode *second, CompareFn compare);
    ResumeNode *mergeSort(ResumeNode *head, int count, CompareFn compare);
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