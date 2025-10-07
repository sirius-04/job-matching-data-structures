#include "ResumeLinkedList.hpp"

ResumeNode::ResumeNode(int id, string* skills, int skillCount) {
    this->id = id;
    this->skillCount = skillCount;
    this->skills = new string[skillCount];
    
    for (int i = 0; i < skillCount; i++) {
        this->skills[i] = skills[i];
    }
    this->next = nullptr;
}


ResumeLinkedList::ResumeLinkedList(int id, string* skills, int skillCount) {
    ResumeNode* newNode = new ResumeNode(id, skills, skillCount);

    head = newNode;
    tail = newNode;
    length = 1;
}

ResumeLinkedList::ResumeLinkedList() {
    head = nullptr;
    tail = nullptr;
    length = 0;
}

ResumeLinkedList::~ResumeLinkedList() {
    ResumeNode* temp = head;
    while (temp != nullptr) {
        ResumeNode* next = temp->next;
        delete[] temp->skills;
        delete temp;
        temp = next;
    }
}

void ResumeLinkedList::printList() {
    ResumeNode* temp = head;

    while (temp != nullptr) {
        cout << "|" << temp->id << " | Skills: ";
        for (int i = 0; i < temp->skillCount; i++) {
            cout << temp->skills[i];
            if (i < temp->skillCount - 1) cout << ", ";
        }
        cout << " | Total Skills: " << temp->skillCount << endl;
        temp = temp->next;
    }

    cout << endl;
}

void ResumeLinkedList::append(int id, string* skills, int skillCount) {
    ResumeNode* newNode = new ResumeNode(id, skills, skillCount);

    if (length == 0) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    
    length++;
}

void ResumeLinkedList::prepend(int id, string* skills, int skillCount) {
    ResumeNode* newNode = new ResumeNode(id, skills, skillCount);

    if (length == 0) {
        head = newNode;
        tail = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }

    length++;
}

void ResumeLinkedList::deleteFirst() {
    if (length == 0) return;

    ResumeNode* temp = head;

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

void ResumeLinkedList::deleteLast() {
    if (length == 0) return;

    ResumeNode* temp = head;

    if (length == 1) {
        delete[] head->skills;
        delete head;

        head = nullptr;
        tail = nullptr;
    } else {
        ResumeNode* pre = head;

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

ResumeNode* ResumeLinkedList::get(int index) {
    if (index < 0 || index >= length) {
        return nullptr;
    }

    ResumeNode* temp = head;

    for (int i = 0; i < index; i++) {
        temp = temp->next;
    }

    return temp;
}

bool ResumeLinkedList::set(int index, int id, string* skills, int skillCount) {
    ResumeNode* temp = get(index);

    if (temp) {
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

bool ResumeLinkedList::insert(int index, int id, string* skills, int skillCount) {
    if (index < 0 || index > length) return false;

    if (index == 0) {
        prepend(id, skills, skillCount);
    } else if (index == length) {
        append(id, skills, skillCount);
    } else {
        ResumeNode* newNode = new ResumeNode(id, skills, skillCount);
        ResumeNode* temp = get(index - 1);

        newNode->next = temp->next;
        temp->next = newNode;
        length++;
    }
    return true;
}

void ResumeLinkedList::deleteNode(int index) {
    if (index < 0 || index >= length) return;

    if (index == 0) {
        deleteFirst();
    } else if (index == length - 1) {
        deleteLast();
    } else {
        ResumeNode* prev = get(index - 1);
        ResumeNode* temp = prev->next;

        prev->next = temp->next;

        delete[] temp->skills;
        delete temp;
        length--;
    }
}

void ResumeLinkedList::reverse() {
    if (length <= 1) return;

    ResumeNode* temp = head;
    head = tail;
    tail = temp;
    ResumeNode* after = temp->next;
    ResumeNode* before = nullptr;

    for (int i = 0; i < length; i++) {
        after = temp->next;
        temp->next = before;
        before = temp;
        temp = after;
    }
}

ResumeLinkedList* ResumeLinkedList::linearSearchResumeBySkills(const string* skillSet, int skillCount, bool matchAll) {
    if (skillSet == nullptr || skillCount <= 0) return nullptr;
    
    ResumeLinkedList* jobListBySkills = new ResumeLinkedList();
    ResumeNode* current = head;

    while (current != nullptr) {
        int matchCount = 0;

        for (int i = 0; i < skillCount; i++) {
            for (int j = 0; j < current->skillCount; j++) {
                if (skillSet[i] == current->skills[j]) {
                    matchCount++;
                    break;
                }
            }
        }

        bool isMatch = false;

        if (matchAll) {
            if (matchCount == skillCount) isMatch = true;
        } else {
            if (matchCount > 0) isMatch = true;
        }

        if (isMatch) {
            jobListBySkills->append(
                current->id,
                current->skills,
                current->skillCount
            );
        }

        current = current->next;
    }

    return jobListBySkills;
}

ResumeNode *ResumeLinkedList::split(ResumeNode *head)
{
  ResumeNode *fast = head;
  ResumeNode *slow = head;

  while (fast->next != nullptr && fast->next->next != nullptr)
  {
    fast = fast->next->next;
    slow = slow->next;
  }

  ResumeNode *second = slow->next;
  slow->next = nullptr;
  return second;
}

ResumeNode* ResumeLinkedList::merge(ResumeNode* first, ResumeNode* second, CompareFn compare) {
    if (!first) return second;
    if (!second) return first;

    if (compare(first, second)) {
        first->next = merge(first->next, second, compare);
        return first;
    } else {
        second->next = merge(first, second->next, compare);
        return second;
    }
}

ResumeNode* ResumeLinkedList::mergeSort(ResumeNode* head, CompareFn compare) {
    if (!head || !head->next) return head;

    ResumeNode* second = split(head);

    head = mergeSort(head, compare);
    second = mergeSort(second, compare);

    return merge(head, second, compare);
}

bool ResumeLinkedList::compareById(ResumeNode* a, ResumeNode* b) {
    return a->id < b->id;
}

bool ResumeLinkedList::compareBySkillCount(ResumeNode* a, ResumeNode* b) {
    return a->skillCount < b->skillCount;
}

bool ResumeLinkedList::compareBySkill(ResumeNode* a, ResumeNode* b) {
    if (a->skillCount == 0 || b->skillCount == 0) {
        return a->skillCount < b->skillCount;
    }

    int minCount = min(a->skillCount, b->skillCount);

    for (int i = 0; i < minCount; ++i) {
        if (a->skills[i] != b->skills[i])
            return a->skills[i] < b->skills[i];
    }

    return a->skillCount < b->skillCount;
}

void ResumeLinkedList::mergeSortBy(const string& criterion) {
    if (criterion == "id") {
        head = mergeSort(head, compareById);
    }
    else if (criterion == "skillCount") {
        head = mergeSort(head, compareBySkillCount);
    }
    else if (criterion == "skill") {
        head = mergeSort(head, compareBySkill);
    }
}
