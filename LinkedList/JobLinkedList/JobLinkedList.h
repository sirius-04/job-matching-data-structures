#ifndef JOB_LINKED_LIST_H
#define JOB_LINKED_LIST_H

#include <iostream>
using namespace std;

class JobNode {
    public:
        string id;
        string position;
        string* skills;
        int skillCount;
        JobNode* next;

        JobNode(string id, string position, string* skills, int skillCount);
};

class JobLinkedList {
    private:
        JobNode* head;
        JobNode* tail;
        int length;

    public:
        // constructor & destructor
        JobLinkedList(string id, string position, string* skills, int skillCount);
        JobLinkedList();
        ~JobLinkedList();

        void printList();
        void append(string id, string position, string* skills, int skillCount);
        void prepend(string id, string position, string* skills, int skillCount);
        void deleteFirst();
        void deleteLast();
        JobNode* get(int index);
        bool set(int index, string position, string* skills, int skillCount);
        bool insert(int index, string id, string position, string* skills, int skillCount);
        void deleteNode(int index);
        void reverse();

        // linear search
        JobLinkedList* linearSearchJobByPosition(const string& position);
        JobNode* linearSearchJobBySkill(string skill);
        JobNode* linearSearchBySkillSet(string* skills, int skillCount);
        
        // binary search
        JobNode* binarySearchJobByPosition(string position);
        JobNode* binarySearchJobBySkill(string skill);
        JobNode* binarySearchJobBySkillSet(string* skills, int skillCount);

        // merge sort
        void mergeSortJobsByPosition();
        void mergeSortJobsBySkillCount();
        void mergeSortJobsById();
        void mergeSortJobsBySkill();    

        // quick sort
        void quickSortJobsByPosition();
        void quickSortJobsBySkillCount();
        void quickSortJobsById();
        void quickSortJobsBySkill();
};

#endif
