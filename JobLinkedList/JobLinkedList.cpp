#include "JobLinkedList.h"


JobNode::JobNode(string id, string value) {
    this->id = id;
    this->value = value;
    this->next = nullptr;
}

JobLinkedList::JobLinkedList(string id, string value) {
    JobNode* newNode = new JobNode(id, value);

    head = newNode;
    tail = newNode;
    length = 1;
}

void JobLinkedList::printList() {
    JobNode* temp = head;

    while (temp != nullptr) {
    cout << "|" << temp->id << "| " << temp->value << endl;

    temp = temp->next;
    }

    cout << endl;
}

void JobLinkedList::append(string id, string value) {
    JobNode* newNode = new JobNode(id, value);

    if (length == 0) {
    head = newNode;
    tail = newNode;
    } else {
    tail->next = newNode;
    tail = newNode;
    }
    
    length++;
}

void JobLinkedList::prepend(string id, string value) {
    JobNode* newNode = new JobNode(id, value);

    if (length == 0) {
    head = newNode;
    tail = newNode;
    } else {
    newNode->next = head;
    head = newNode;
    }

    length++;
}

void JobLinkedList::deleteFirst() {
    if (length == 0) return;

    JobNode* temp = head;

    if (length == 1) {
    head = nullptr;
    tail = nullptr;
    } else {
    head = head->next;
    }

    delete temp;
    length--;
}

void JobLinkedList::deleteLast() {
    if (length == 0) return;

    JobNode* temp = head;

    if (length == 1) {
    delete head;

    head = nullptr;
    tail = nullptr;
    } else {
    JobNode* pre = head;

    while (temp->next != nullptr) {
        pre = temp;
        temp = temp->next;
    }

    tail = pre;
    tail->next = nullptr;
    }

    delete temp;
    length--;
}

JobNode* JobLinkedList::get(int index) {
    if (index < 0 || index >= length) {
    return nullptr;
    }

    JobNode* temp = head;

    for (int i = 0; i < index; i++) {
    temp = temp->next;
    }

    return temp;
}

bool JobLinkedList::set(int index, string value) {
    JobNode* temp = get(index);

    if (temp) {
    temp->value = value;
    return true;
    }

    return false;
}

bool JobLinkedList::insert(int index, string id, string value) {
    if (index < 0 || index > length) return false;

    if (index == 0) {
    prepend(id, value);
    } else if (index == length) {
    append(id, value);
    } else {
    JobNode* newNode = new JobNode(id, value);
    JobNode* temp = get(index - 1);

    newNode->next = temp->next;
    temp->next = newNode;
    length++;
    }
    return true;
}

void JobLinkedList::deleteNode(int index) {
    if (index < 0 || index >= length) return;

    if (index == 0) {
    deleteFirst();
    } else if (index == length - 1) {
    deleteLast();
    } else {
    JobNode* prev = get(index - 1);
    JobNode* temp = prev->next;

    prev->next = temp->next;

    delete temp;
    length--;
    }
}

void JobLinkedList::reverse() {
    JobNode* temp = head;
    head = tail;
    tail = temp;
    JobNode* after = temp->next;
    JobNode* before = nullptr;

    for (int i = 0; i < length; i++) {
    after = temp->next;
    temp->next = before;
    before = temp;
    temp = after;
    }
}