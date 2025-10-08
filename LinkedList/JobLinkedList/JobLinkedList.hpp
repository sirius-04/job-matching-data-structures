#ifndef JOB_LINKED_LIST_HPP
#define JOB_LINKED_LIST_HPP

#include <iostream>
using namespace std;

class JobNode
{
public:
    int id;
    string position;
    string *skills;
    int skillCount;
    JobNode *next;

    JobNode(int id, string position, string *skills, int skillCount);
};

class JobLinkedList
{
private:
    JobNode *head;
    JobNode *tail;
    int length;

public:
    // constructor & destructor
    JobLinkedList(int id, string position, string *skills, int skillCount);
    JobLinkedList();
    ~JobLinkedList();

    // general functions or utilities
    void printList();
    void append(int id, string position, string *skills, int skillCount);
    void prepend(int id, string position, string *skills, int skillCount);
    void deleteFirst();
    void deleteLast();
    JobNode *get(int index);
    bool set(int index, string position, string *skills, int skillCount);
    bool insert(int index, int id, string position, string *skills, int skillCount);
    void deleteNode(int index);
    void reverse();

    static string cleanString(string str);
    void displaySlice();
    void insertLast(int id, string position, string *skills, int skillCount);

    // linear search
    JobLinkedList *linearSearchJobByPosition(const string &position);
    JobLinkedList *linearSearchJobBySkills(const string *skillSet, int skillCount, bool matchAll);

    // binary search
    JobNode *findMiddle(JobNode *start, JobNode *end);
    JobLinkedList *binarySearchJobByPosition(const string &position);
    JobLinkedList *binarySearchJobBySkills(const string *skillSet, int skillCount);

    // merge sort
    typedef bool (*CompareFn)(JobNode *, JobNode *);
    static bool compareById(JobNode *a, JobNode *b);
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

    void quickSortById();
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