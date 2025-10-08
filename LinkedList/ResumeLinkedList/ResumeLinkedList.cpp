#include "ResumeLinkedList.hpp"

ResumeNode::ResumeNode(int id, string *skills, int skillCount)
{
    this->id = id;
    this->skillCount = skillCount;
    this->skills = new string[skillCount];

    for (int i = 0; i < skillCount; i++)
    {
        this->skills[i] = skills[i];
    }
    this->next = nullptr;
}

ResumeLinkedList::ResumeLinkedList(int id, string *skills, int skillCount)
{
    ResumeNode *newNode = new ResumeNode(id, skills, skillCount);

    head = newNode;
    tail = newNode;
    length = 1;
}

ResumeLinkedList::ResumeLinkedList()
{
    head = nullptr;
    tail = nullptr;
    length = 0;
}

ResumeLinkedList::~ResumeLinkedList()
{
    ResumeNode *temp = head;
    while (temp != nullptr)
    {
        ResumeNode *next = temp->next;
        delete[] temp->skills;
        delete temp;
        temp = next;
    }
}

void ResumeLinkedList::printList()
{
    ResumeNode *temp = head;

    while (temp != nullptr)
    {
        cout << "|" << temp->id << " | Skills: ";
        for (int i = 0; i < temp->skillCount; i++)
        {
            cout << temp->skills[i];
            if (i < temp->skillCount - 1)
                cout << ", ";
        }
        cout << " | Total Skills: " << temp->skillCount << endl;
        temp = temp->next;
    }

    cout << endl;
}

void ResumeLinkedList::append(int id, string *skills, int skillCount)
{
    ResumeNode *newNode = new ResumeNode(id, skills, skillCount);

    if (length == 0)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }

    length++;
}

void ResumeLinkedList::prepend(int id, string *skills, int skillCount)
{
    ResumeNode *newNode = new ResumeNode(id, skills, skillCount);

    if (length == 0)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        newNode->next = head;
        head = newNode;
    }

    length++;
}

void ResumeLinkedList::deleteFirst()
{
    if (length == 0)
        return;

    ResumeNode *temp = head;

    if (length == 1)
    {
        head = nullptr;
        tail = nullptr;
    }
    else
    {
        head = head->next;
    }

    delete[] temp->skills;
    delete temp;
    length--;
}

void ResumeLinkedList::deleteLast()
{
    if (length == 0)
        return;

    ResumeNode *temp = head;

    if (length == 1)
    {
        delete[] head->skills;
        delete head;

        head = nullptr;
        tail = nullptr;
    }
    else
    {
        ResumeNode *pre = head;

        while (temp->next != nullptr)
        {
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

ResumeNode *ResumeLinkedList::get(int index)
{
    if (index < 0 || index >= length)
    {
        return nullptr;
    }

    ResumeNode *temp = head;

    for (int i = 0; i < index; i++)
    {
        temp = temp->next;
    }

    return temp;
}

bool ResumeLinkedList::set(int index, int id, string *skills, int skillCount)
{
    ResumeNode *temp = get(index);

    if (temp)
    {
        delete[] temp->skills;

        temp->skills = new string[skillCount];
        for (int i = 0; i < skillCount; i++)
        {
            temp->skills[i] = skills[i];
        }

        temp->skillCount = skillCount;
        return true;
    }

    return false;
}

bool ResumeLinkedList::insert(int index, int id, string *skills, int skillCount)
{
    if (index < 0 || index > length)
        return false;

    if (index == 0)
    {
        prepend(id, skills, skillCount);
    }
    else if (index == length)
    {
        append(id, skills, skillCount);
    }
    else
    {
        ResumeNode *newNode = new ResumeNode(id, skills, skillCount);
        ResumeNode *temp = get(index - 1);

        newNode->next = temp->next;
        temp->next = newNode;
        length++;
    }
    return true;
}

void ResumeLinkedList::deleteNode(int index)
{
    if (index < 0 || index >= length)
        return;

    if (index == 0)
    {
        deleteFirst();
    }
    else if (index == length - 1)
    {
        deleteLast();
    }
    else
    {
        ResumeNode *prev = get(index - 1);
        ResumeNode *temp = prev->next;

        prev->next = temp->next;

        delete[] temp->skills;
        delete temp;
        length--;
    }
}

void ResumeLinkedList::reverse()
{
    if (length <= 1)
        return;

    ResumeNode *temp = head;
    head = tail;
    tail = temp;
    ResumeNode *after = temp->next;
    ResumeNode *before = nullptr;

    for (int i = 0; i < length; i++)
    {
        after = temp->next;
        temp->next = before;
        before = temp;
        temp = after;
    }
}

ResumeLinkedList *ResumeLinkedList::linearSearchResumeBySkills(const string *skillSet, int skillCount, bool matchAll)
{
    if (skillSet == nullptr || skillCount <= 0)
        return nullptr;

    ResumeLinkedList *jobListBySkills = new ResumeLinkedList();
    ResumeNode *current = head;

    while (current != nullptr)
    {
        int matchCount = 0;

        for (int i = 0; i < skillCount; i++)
        {
            for (int j = 0; j < current->skillCount; j++)
            {
                if (skillSet[i] == current->skills[j])
                {
                    matchCount++;
                    break;
                }
            }
        }

        bool isMatch = false;

        if (matchAll)
        {
            if (matchCount == skillCount)
                isMatch = true;
        }
        else
        {
            if (matchCount > 0)
                isMatch = true;
        }

        if (isMatch)
        {
            jobListBySkills->append(
                current->id,
                current->skills,
                current->skillCount);
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

ResumeNode *ResumeLinkedList::merge(ResumeNode *first, ResumeNode *second, CompareFn compare)
{
    if (!first)
        return second;
    if (!second)
        return first;

    if (compare(first, second))
    {
        first->next = merge(first->next, second, compare);
        return first;
    }
    else
    {
        second->next = merge(first, second->next, compare);
        return second;
    }
}

ResumeNode *ResumeLinkedList::mergeSort(ResumeNode *head, CompareFn compare)
{
    if (!head || !head->next)
        return head;

    ResumeNode *second = split(head);

    head = mergeSort(head, compare);
    second = mergeSort(second, compare);

    return merge(head, second, compare);
}

bool ResumeLinkedList::compareById(ResumeNode *a, ResumeNode *b)
{
    return a->id < b->id;
}

bool ResumeLinkedList::compareBySkillCount(ResumeNode *a, ResumeNode *b)
{
    return a->skillCount < b->skillCount;
}

bool ResumeLinkedList::compareBySkill(ResumeNode *a, ResumeNode *b)
{
    if (a->skillCount == 0 || b->skillCount == 0)
    {
        return a->skillCount < b->skillCount;
    }

    int minCount = min(a->skillCount, b->skillCount);

    for (int i = 0; i < minCount; ++i)
    {
        if (a->skills[i] != b->skills[i])
            return a->skills[i] < b->skills[i];
    }

    return a->skillCount < b->skillCount;
}

void ResumeLinkedList::mergeSortBy(const string &criterion)
{
    if (criterion == "id")
    {
        head = mergeSort(head, compareById);
    }
    else if (criterion == "skillCount")
    {
        head = mergeSort(head, compareBySkillCount);
    }
    else if (criterion == "skill")
    {
        head = mergeSort(head, compareBySkill);
    }
}

// ======= Insert Last =======
void ResumeLinkedList::insertLast(int id, string *skills, int skillCount)
{
    ResumeNode *t = new ResumeNode(id, skills, skillCount);
    if (head == nullptr)
        head = tail = t;
    else
    {
        tail->next = t;
        tail = t;
    }
}

// ======= Clean String =======
string ResumeLinkedList::cleanString(string s)
{
    s.erase(remove(s.begin(), s.end(), '"'), s.end());
    while (!s.empty() && isspace(s.front()))
        s.erase(s.begin());
    while (!s.empty() && isspace(s.back()))
        s.pop_back();
    return s;
}

// ======= Display =======
void ResumeLinkedList::display()
{
    ResumeNode *p = head;
    cout << "\n===== Displaying All Resumes =====\n";
    while (p != nullptr)
    {
        cout << "ID: " << p->id << endl;
        cout << "Skill Count: " << p->skillCount << endl;
        cout << "Skills: ";
        for (int i = 0; i < p->skillCount; i++)
        {
            cout << p->skills[i];
            if (i < p->skillCount - 1)
                cout << ", ";
        }
        cout << "\n----------------------\n";
        p = p->next;
    }
}

// ======= Display Slice =======
void ResumeLinkedList::displaySlice()
{
    int total = 0;
    ResumeNode *p = head;
    while (p != nullptr)
    {
        total++;
        p = p->next;
    }

    p = head;
    int index = 0;
    cout << "\n===== Resume Linked List (Showing first 10 and last 10) =====\n";
    while (p != nullptr)
    {
        if (index < 10 || index >= total - 10)
        {
            cout << "ID: " << p->id << endl;
            cout << "Skill Count: " << p->skillCount << endl;
            cout << "Skills: ";
            for (int i = 0; i < p->skillCount; i++)
            {
                cout << p->skills[i];
                if (i < p->skillCount - 1)
                    cout << ", ";
            }
            cout << "\n----------------------\n";
        }
        else if (index == 10)
        {
            cout << "...\n...(skipping " << total - 20 << " resumes)...\n...\n";
        }
        p = p->next;
        index++;
    }
}

// ======= Swap =======
void ResumeLinkedList::swap(ResumeNode *a, ResumeNode *b)
{
    std::swap(a->id, b->id);
    std::swap(a->skills, b->skills);
    std::swap(a->skillCount, b->skillCount);
}

// ======= Partition =======
ResumeNode *ResumeLinkedList::partitionBySkillCount(ResumeNode *low, ResumeNode *high)
{
    int pivot = high->skillCount;
    ResumeNode *i = low;
    for (ResumeNode *j = low; j != high; j = j->next)
    {
        if (j->skillCount < pivot)
        {
            swap(i, j);
            i = i->next;
        }
    }
    swap(i, high);
    return i;
}

ResumeNode *ResumeLinkedList::partitionBySkill(ResumeNode *low, ResumeNode *high)
{
    string pivot = (high->skillCount > 0) ? high->skills[0] : "";
    ResumeNode *i = low;
    for (ResumeNode *j = low; j != high; j = j->next)
    {
        string firstSkillJ = (j->skillCount > 0) ? j->skills[0] : "";
        if (firstSkillJ < pivot)
        {
            swap(i, j);
            i = i->next;
        }
    }
    swap(i, high);
    return i;
}

// ======= QuickSort Core =======
void ResumeLinkedList::quickSort(ResumeNode *low, ResumeNode *high, const string &type)
{
    if (!low || !high || low == high)
        return;

    ResumeNode *pivot = nullptr;
    if (type == "skillCount")
        pivot = partitionBySkillCount(low, high);
    else if (type == "skill")
        pivot = partitionBySkill(low, high);

    ResumeNode *beforePivot = low;
    while (beforePivot && beforePivot->next != pivot)
        beforePivot = beforePivot->next;

    if (beforePivot != nullptr && low != pivot)
        quickSort(low, beforePivot, type);

    if (pivot != nullptr && pivot->next != nullptr)
        quickSort(pivot->next, high, type);
}

ResumeNode *ResumeLinkedList::getLast()
{
    ResumeNode *temp = head;
    while (temp && temp->next)
        temp = temp->next;
    return temp;
}

// ======= Sort Wrappers =======
void ResumeLinkedList::quickSortBySkillCount()
{
    ResumeNode *lastNode = getLast();
    quickSort(head, lastNode, "skillCount");
}

void ResumeLinkedList::quickSortBySkill()
{
    ResumeNode *lastNode = getLast();
    quickSort(head, lastNode, "skill");
}

// ======= Search by Skills =======
ResumeLinkedList *ResumeLinkedList::binarySearchResumeBySkills(const string *skillSet, int skillCount)
{
    if (head == nullptr || skillSet == nullptr || skillCount <= 0)
        return nullptr;

    quickSortBySkill(); // optional sorting by first skill

    ResumeLinkedList *matches = new ResumeLinkedList();
    ResumeNode *p = head;

    while (p != nullptr)
    {
        int matched = 0;

        for (int s = 0; s < skillCount; s++)
        {
            for (int j = 0; j < p->skillCount; j++)
            {
                if (p->skills[j] == skillSet[s])
                {
                    matched++;
                    break;
                }
            }
        }

        if (matched == skillCount)
            matches->insertLast(p->id, p->skills, p->skillCount);

        p = p->next;
    }

    return (matches->head == nullptr) ? nullptr : matches;
}
