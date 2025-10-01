#ifndef JOB_LINKED_LIST_H
#define JOB_LINKED_LIST_H

#include <iostream>
using namespace std;

class JobNode {
    public:
        string id;
        string value;
        JobNode* next;

        JobNode(string id, string value);
};

class JobLinkedList {
    private:
        JobNode* head;
        JobNode* tail;
        int length;

    public:
        JobLinkedList(string id, string value);
        void printList();
        void append(string id, string value);
        void prepend(string id, string value);
        void deleteFirst();
        void deleteLast();
        JobNode* get(int index);
        bool set(int index, string value);
        bool insert(int index, string id, string value);
        void deleteNode(int index);
        void reverse();
};

#endif
