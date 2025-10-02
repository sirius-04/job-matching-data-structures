#include "JobLinkedList.h"


JobNode::JobNode(string id, string position, string* skills, int skillCount) {
    this->id = id;
    this->position = position;
    this->skillCount = skillCount;
    this->skills = new string[skillCount];
    
    for (int i = 0; i < skillCount; i++) {
        this->skills[i] = skills[i];
    }
    this->next = nullptr;
}


JobLinkedList::JobLinkedList(string id, string position, string* skills, int skillCount) {
    JobNode* newNode = new JobNode(id, position, skills, skillCount);

    head = newNode;
    tail = newNode;
    length = 1;
}

JobLinkedList::JobLinkedList() {
    head = nullptr;
    tail = nullptr;
    length = 0;
}

JobLinkedList::~JobLinkedList() {
    JobNode* temp = head;
    while (temp != nullptr) {
        JobNode* next = temp->next;
        delete[] temp->skills;
        delete temp;
        temp = next;
    }
}


void JobLinkedList::printList() {
    JobNode* temp = head;

    while (temp != nullptr) {
        cout << "|" << temp->id << "| " << temp->position << " | Skills: ";
        for (int i = 0; i < temp->skillCount; i++) {
            cout << temp->skills[i];
            if (i < temp->skillCount - 1) cout << ", ";
        }
        cout << " | Total Skills: " << temp->skillCount << endl;
        temp = temp->next;
    }

    cout << endl;
}

void JobLinkedList::append(string id, string position, string* skills, int skillCount) {
    JobNode* newNode = new JobNode(id, position, skills, skillCount);

    if (length == 0) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    
    length++;
}

void JobLinkedList::prepend(string id, string position, string* skills, int skillCount) {
    JobNode* newNode = new JobNode(id, position, skills, skillCount);

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

    delete[] temp->skills;
    delete temp;
    length--;
}

void JobLinkedList::deleteLast() {
    if (length == 0) return;

    JobNode* temp = head;

    if (length == 1) {
        delete[] head->skills;
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

    delete[] temp->skills;
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

bool JobLinkedList::set(int index, string position, string* skills, int skillCount) {
    JobNode* temp = get(index);

    if (temp) {
        temp->position = position;

        delete[] temp->skills;
        temp->skills = new string[skillCount];
        for (int i = 0; i < skillCount; i++) {
            temp->skills[i] = skills[i];
        }
        
        temp-> skillCount = skillCount;
        return true;
    }

    return false;
}

bool JobLinkedList::insert(int index, string id, string position, string* skills, int skillCount) {
    if (index < 0 || index > length) return false;

    if (index == 0) {
        prepend(id, position, skills, skillCount);
    } else if (index == length) {
        append(id, position, skills, skillCount);
    } else {
        JobNode* newNode = new JobNode(id, position, skills, skillCount);
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

        delete[] temp->skills;
        delete temp;
        length--;
    }
}

void JobLinkedList::reverse() {
    if (length <= 1) return;

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