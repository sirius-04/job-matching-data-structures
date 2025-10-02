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
};

#endif
