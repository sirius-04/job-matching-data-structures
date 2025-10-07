#ifndef JOB_LINKED_LIST_H
#define JOB_LINKED_LIST_H

#include <iostream>
using namespace std;

class JobNode {
    public:
        int id;
        string position;
        string* skills;
        int skillCount;
        JobNode* next;

        JobNode(int id, string position, string* skills, int skillCount);
};

class JobLinkedList {
    private:
        JobNode* head;
        JobNode* tail;
        int length;

    public:
        // constructor & destructor
        JobLinkedList(int id, string position, string* skills, int skillCount);
        JobLinkedList();
        ~JobLinkedList();

        // general functions or utilities
        void printList();
        void append(int id, string position, string* skills, int skillCount);
        void prepend(int id, string position, string* skills, int skillCount);
        void deleteFirst();
        void deleteLast();
        JobNode* get(int index);
        bool set(int index, string position, string* skills, int skillCount);
        bool insert(int index, int id, string position, string* skills, int skillCount);
        void deleteNode(int index);
        void reverse();

        // linear search
        JobLinkedList* linearSearchJobByPosition(const string& position);
        JobLinkedList* linearSearchJobBySkills(const string* skillSet, int skillCount, bool matchAll);
        
        // binary search
        JobLinkedList* binarySearchJobByPosition(const string& position);
        JobLinkedList* binarySearchJobBySkills(const string* skillSet, int skillCount, bool matchAll);

        // merge sort
        typedef bool (*CompareFn)(JobNode*, JobNode*);
        static bool compareById(JobNode* a, JobNode* b);
        static bool compareByPosition(JobNode* a, JobNode* b);
        static bool compareBySkillCount(JobNode* a, JobNode* b);
        static bool compareBySkill(JobNode* a, JobNode* b);

        JobNode* split(JobNode* head);
        JobNode* merge(JobNode* first, JobNode* second, CompareFn compare);
        JobNode* mergeSort(JobNode* head, CompareFn compare);
        void mergeSortBy(string criterion);

        // quick sort
        void quickSortById();
        void quickSortByPosition();
        void quickSortBySkill();
        void quickSortBySkillCount();
};

#endif