#ifndef JOB_CIRCULAR_LINKED_LIST_HPP
#define JOB_CIRCULAR_LINKED_LIST_HPP

#include <iostream>
#include "../../models/Job/Job.hpp"
#include "../../models/ConfigEnum/ConfigEnum.hpp"
using namespace std;

class JobCircularLinkedList
{
private:
    JobNode *head;
    JobNode *tail;
    int length;

    static void mergeSortSkills(string *skills, int left, int right);
    static void mergeSkills(string *skills, int left, int mid, int right);
    static void quickSortSkills(string *skills, int low, int high);
    static int partitionSkills(string *skills, int low, int high);

public:
    // constructor & destructor
    JobCircularLinkedList();
    ~JobCircularLinkedList();

    // general functions or utilities
    JobNode* getHead() const;
    void append(Job data);
    void prepend(Job data);
    void deleteFirst();
    void deleteLast();
    JobNode *get(int index);
    Job *findById(int id);
    bool set(int index, const string &position, const string *skills, int skillCount);
    bool insert(int index, Job data);
    void deleteNode(int index);
    void reverse();
    int getLength();

    static string cleanString(string str);
    void printSlice();

    // linear search
    JobCircularLinkedList *linearSearchJobByPosition(const string &position);
    JobCircularLinkedList *linearSearchJobBySkills(const string *skillSet, int skillCount, bool matchAll);

    // binary search
    JobNode *getMiddle(JobNode *start, JobNode *end);
    JobCircularLinkedList *binarySearchJobByPosition(const string &position, SortAlgorithm sortAlgo);
    JobCircularLinkedList *binarySearchJobBySkills(const string *skillSet, int skillCount, bool matchAll, SortAlgorithm sortAlgo);

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