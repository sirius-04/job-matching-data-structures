#ifndef RESUME_LINKED_LIST_HPP
#define RESUME_LINKED_LIST_HPP

#include <iostream>
#include "../../models/Resume/Resume.hpp"
using namespace std;

class ResumeNode
{
public:
    Resume data;
    ResumeNode *next;

    ResumeNode(Resume data);
};

class ResumeLinkedList
{
private:
    ResumeNode *head;
    ResumeNode *tail;
    int length;

public:
    // constructor & destructor
    ResumeLinkedList();
    ~ResumeLinkedList();

    // general functions & utilities
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
    ResumeLinkedList linearSearchResumeBySkills(const string *skillSet, int skillCount, bool matchAll);

    // binary search
    ResumeLinkedList *binarySearchResumeBySkills(const string *skillSet, int skillCount);
    // ResumeLinkedList *binarySearchResumeBySkills(const string *skillSet, int skillCount, bool matchAll);

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

    void quickSortById();
    void quickSortBySkill();
    void quickSortBySkillCount();

    // Swap Utility
    void swap(ResumeNode *a, ResumeNode *b);
};

#endif