#ifndef JOB_CIRCULAR_LINKED_LIST_HPP
#define JOB_CIRCULAR_LINKED_LIST_HPP

#include <iostream>
#include "../../models/Job/Job.hpp"
using namespace std;

class JobCircularLinkedList
{
private:
    JobNode *head;
    JobNode *tail;
    int length;

    // Helper to break and restore circular link
    void breakCircular();
    void restoreCircular();

public:
    // constructor & destructor
    JobCircularLinkedList();
    ~JobCircularLinkedList();

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
    JobCircularLinkedList linearSearchJobByPosition(const string &position);
    JobCircularLinkedList linearSearchJobBySkills(const string *skillSet, int skillCount, bool matchAll);

    // binary search
    JobNode *getMiddle(JobNode *start, int count);
    JobCircularLinkedList binarySearchJobByPosition(const string &position);
    JobCircularLinkedList *binarySearchJobBySkills(const string *skillSet, int skillCount, bool matchAll);

    // merge sort
    typedef bool (*CompareFn)(JobNode *, JobNode *);
    static bool compareByPosition(JobNode *a, JobNode *b);
    static bool compareBySkillCount(JobNode *a, JobNode *b);
    static bool compareBySkill(JobNode *a, JobNode *b);

    JobNode *split(JobNode *head, int count);
    JobNode *merge(JobNode *first, JobNode *second, CompareFn compare);
    JobNode *mergeSort(JobNode *head, int count, CompareFn compare);
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