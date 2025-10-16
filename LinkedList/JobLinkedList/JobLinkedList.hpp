#ifndef JOB_LINKED_LIST_HPP
#define JOB_LINKED_LIST_HPP

#include <iostream>
#include "../../models/Job/Job.hpp"
using namespace std;

class JobLinkedList
{
private:
    JobNode *head;
    JobNode *tail;
    int length;

public:
    // constructor & destructor
    JobLinkedList();
    ~JobLinkedList();

    // general functions or utilities
    void append(Job data);
    void prepend(Job data);
    void deleteFirst();
    void deleteLast();
    JobNode *get(int index);
    bool set(int index, const string &position, const string *skills, int skillCount);
    bool insert(int index, Job data);
    void deleteNode(int index);
    void reverse();
    int getLength();

    static string cleanString(string str);
    void printSlice();

    // linear search
    JobLinkedList *linearSearchJobByPosition(const string &position);
    JobLinkedList *linearSearchJobBySkills(const string *skillSet, int skillCount, bool matchAll);

    // binary search
    JobNode *getMiddle(JobNode *start, JobNode *end);
    JobLinkedList binarySearchJobByPosition(const string &position);
    JobLinkedList *binarySearchJobBySkills(const string *skillSet, int skillCount, bool matchAll);

    // merge sort
    typedef bool (*CompareFn)(JobNode *, JobNode *);
    static bool compareByPosition(JobNode *a, JobNode *b);
    static bool compareBySkillCount(JobNode *a, JobNode *b);
    static bool compareBySkill(JobNode *a, JobNode *b);

    JobNode *split(JobNode *head);
    JobNode *merge(JobNode *first, JobNode *second, CompareFn compare);
    JobNode *mergeSort(JobNode *head, CompareFn compare);
    void mergeSortBy(string criterion);

    // quick sort
    JobNode *sortTail();
    void quickSort(JobNode *low, JobNode *high, const string &type);

    void quickSortByPosition();
    void quickSortBySkill();
    void quickSortBySkillCount();

    JobNode *partitionBySkillCount(JobNode *low, JobNode *high);
    JobNode *partitionBySkill(JobNode *low, JobNode *high);
    JobNode *partitionByPosition(JobNode *low, JobNode *high);

    // swap Utility
    void swap(JobNode *a, JobNode *b);
};

#endif