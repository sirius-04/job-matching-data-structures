#include "ResumeLinkedList.hpp"
#include <algorithm>

ResumeLinkedList::ResumeLinkedList()
{
    head = nullptr;
    tail = nullptr;
    length = 0;
}

ResumeLinkedList::~ResumeLinkedList()
{
    ResumeNode *current = head;
    while (current != nullptr)
    {
        ResumeNode *temp = current;
        current = current->next;
        delete temp;
    }

    head = nullptr;
    tail = nullptr;
    length = 0;
}

ResumeNode* ResumeLinkedList::getHead() const {
    return head;
}

void ResumeLinkedList::append(Resume data)
{
    ResumeNode *newNode = new ResumeNode(data);

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

void ResumeLinkedList::prepend(Resume data)
{
    ResumeNode *newNode = new ResumeNode(data);

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

    delete temp;
    length--;
}

Resume* ResumeLinkedList::findById(int id) {
    if (!head) {
        return nullptr;
    }
    
    ResumeNode* current = head;
    while (current) {
        if (current->data.id == id) {
            return &(current->data);
        }
        current = current->next;
    }
    return nullptr;  // Not found
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

bool ResumeLinkedList::set(int index, const string *skills, int skillCount)
{
    ResumeNode *temp = get(index);
    if (!temp)
        return false;

    Resume &resume = temp->data;

    // Free old skills before assigning new ones
    delete[] resume.skills;
    resume.skills = new string[skillCount];
    for (int i = 0; i < skillCount; ++i)
    {
        resume.skills[i] = skills[i];
    }

    resume.skillCount = skillCount;

    return true;
}

bool ResumeLinkedList::insert(int index, Resume data)
{
    if (index < 0 || index > length)
        return false;

    if (index == 0)
    {
        prepend(data);
    }
    else if (index == length)
    {
        append(data);
    }
    else
    {
        ResumeNode *newNode = new ResumeNode(data);
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

int ResumeLinkedList::getLength()
{
    return length;
}

ResumeLinkedList *ResumeLinkedList::linearSearchResumeBySkills(const string *skillSet, int skillCount, bool matchAll)
{
    ResumeLinkedList *resumeListBySkills = new ResumeLinkedList();

    if (skillSet == nullptr || skillCount <= 0)
        return resumeListBySkills;

    ResumeNode *current = head;

    while (current != nullptr)
    {
        const Resume &resume = current->data;
        int matchCount = 0;

        for (int i = 0; i < skillCount; ++i)
        {
            for (int j = 0; j < resume.skillCount; ++j)
            {
                if (skillSet[i] == resume.skills[j])
                {
                    matchCount++;
                    break;
                }
            }
        }

        bool isMatch = matchAll ? (matchCount == skillCount) : (matchCount > 0);

        if (isMatch)
            resumeListBySkills->append(resume);

        current = current->next;
    }

    return resumeListBySkills;
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
    return a->data.id < b->data.id;
}

bool ResumeLinkedList::compareBySkillCount(ResumeNode *a, ResumeNode *b)
{
    return a->data.skillCount < b->data.skillCount;
}

bool ResumeLinkedList::compareBySkill(ResumeNode *a, ResumeNode *b)
{
    if (a->data.skillCount == 0 || b->data.skillCount == 0)
    {
        return a->data.skillCount < b->data.skillCount;
    }

    int minCount = min(a->data.skillCount, b->data.skillCount);

    for (int i = 0; i < minCount; ++i)
    {
        if (a->data.skills[i] != b->data.skills[i])
            return a->data.skills[i] < b->data.skills[i];
    }

    return a->data.skillCount < b->data.skillCount;
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

// ======= Display Slice =======
void ResumeLinkedList::printSlice()
{
    int total = 0;
    ResumeNode *p = head;

    // Count total nodes
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
        if (index < 5 || index >= total - 5)
        {
            cout << "|" << p->data.id << " | Skills: ";
            for (int i = 0; i < p->data.skillCount; i++)
            {
                cout << p->data.skills[i];
                if (i < p->data.skillCount - 1)
                    cout << ", ";
            }
            cout << " | Total Skills: " << p->data.skillCount << endl;
        }
        else if (index == 10)
        {
            cout << "...\n...\n...(skipping " << total - 10 << " resumes)...\n...\n...\n";
        }

        p = p->next;
        index++;
    }

    cout << endl;
}

// ======= Swap =======
void ResumeLinkedList::swap(ResumeNode *a, ResumeNode *b)
{
    // Swap the entire Resume data structure
    Resume temp = a->data;
    a->data = b->data;
    b->data = temp;
}

// ======= Partition =======
// ======= Partition =======
ResumeNode *ResumeLinkedList::partitionBySkillCount(ResumeNode *low, ResumeNode *high)
{
    if (!low || !high)
        return nullptr;

    int pivot = high->data.skillCount;
    ResumeNode *i = nullptr; // i points to the last element < pivot

    for (ResumeNode *j = low; j != high; j = j->next)
    {
        if (!j) // Safety check
            break;

        if (j->data.skillCount < pivot)
        {
            // First element less than pivot
            if (i == nullptr)
                i = low;
            else
                i = i->next;

            swap(i, j);
        }
    }

    // Place pivot in correct position
    if (i == nullptr)
        i = low;
    else
        i = i->next;

    swap(i, high);
    return i;
}

ResumeNode *ResumeLinkedList::partitionBySkill(ResumeNode *low, ResumeNode *high)
{
    if (!low || !high)
        return nullptr;

    string pivot = (high->data.skillCount > 0) ? high->data.skills[0] : "";
    ResumeNode *i = nullptr;

    for (ResumeNode *j = low; j != high; j = j->next)
    {
        if (!j)
            break;

        string firstSkillJ = (j->data.skillCount > 0) ? j->data.skills[0] : "";

        if (firstSkillJ < pivot)
        {
            // First element less than pivot
            if (i == nullptr)
                i = low;
            else
                i = i->next;

            swap(i, j);
        }
    }

    // Place pivot in correct position
    if (i == nullptr)
        i = low;
    else
        i = i->next;

    swap(i, high);
    return i;
}

// ======= QuickSort Core =======
void ResumeLinkedList::quickSort(ResumeNode *low, ResumeNode *high, const string &type)
{
    // Base cases
    if (!low || !high || low == high)
        return;

    ResumeNode *temp = low;
    bool found = false;
    while (temp)
    {
        if (temp == high)
        {
            found = true;
            break;
        }
        temp = temp->next;
    }

    if (!found)
        return;

    // Partition
    ResumeNode *pivot = nullptr;
    if (type == "skillCount")
        pivot = partitionBySkillCount(low, high);
    else if (type == "skill")
        pivot = partitionBySkill(low, high);

    if (!pivot)
        return;

    // Find node before pivot for left partition
    ResumeNode *beforePivot = nullptr;
    if (low != pivot)
    {
        beforePivot = low;
        while (beforePivot && beforePivot->next != pivot)
            beforePivot = beforePivot->next;
    }

    // Sort left partition (from low to beforePivot)
    if (beforePivot && low != pivot)
        quickSort(low, beforePivot, type);

    // Sort right partition (from pivot->next to high)
    if (pivot && pivot != high && pivot->next)
        quickSort(pivot->next, high, type);
}

ResumeNode *ResumeLinkedList::sortTail()
{
    if (!head)
        return nullptr;

    ResumeNode *temp = head;
    while (temp && temp->next)
        temp = temp->next;
    return temp;
}

// ======= Sort Wrappers =======
void ResumeLinkedList::quickSortBySkillCount()
{
    if (!head || !head->next) // Empty or single element
        return;

    ResumeNode *lastNode = sortTail();
    quickSort(head, lastNode, "skillCount");
}

void ResumeLinkedList::quickSortBySkill()
{
    if (!head || !head->next) // Empty or single element
        return;

    ResumeNode *lastNode = sortTail();
    quickSort(head, lastNode, "skill");
}

ResumeLinkedList *ResumeLinkedList::binarySearchResumeBySkills(const string *skills, int skillCount, bool matchAll, SortAlgorithm sortAlgo)
{
    ResumeLinkedList *matches = new ResumeLinkedList();

    if (head == nullptr || skills == nullptr || skillCount <= 0)
        return matches;

    for (ResumeNode *p = head; p != nullptr; p = p->next)
    {
        // Sort this resume's skills array using the specified algorithm
        if (sortAlgo == QUICK) {
            quickSortSkills(p->data.skills, 0, p->data.skillCount - 1);
            
        } else {
            mergeSortSkills(p->data.skills, 0, p->data.skillCount - 1);
        }

        int matched = 0;

        // For each skill in search criteria
        for (int s = 0; s < skillCount; ++s)
        {
            bool found = false;
            int left = 0;
            int right = p->data.skillCount - 1;
            
            string target = skills[s];

            while (left <= right)
            {
                int mid = left + (right - left) / 2;
                string key = p->data.skills[mid];

                if (key == target)
                {
                    found = true;
                    break;
                }
                else if (key < target)
                    left = mid + 1;
                else
                    right = mid - 1;
            }

            if (found)
                matched++;
            else if (matchAll)
                break; // Early exit if matchAll and one skill not found
        }

        // Determine if the resume matches based on matchAll flag
        bool isMatch = matchAll ? (matched == skillCount) : (matched > 0);

        if (isMatch)
            matches->append(p->data);
    }

    return matches;
}

// Merge Sort for skills array
void ResumeLinkedList::mergeSortSkills(string *skills, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortSkills(skills, left, mid);
        mergeSortSkills(skills, mid + 1, right);
        mergeSkills(skills, left, mid, right);
    }
}

void ResumeLinkedList::mergeSkills(string *skills, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    string *L = new string[n1];
    string *R = new string[n2];
    
    for (int i = 0; i < n1; i++)
        L[i] = skills[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = skills[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            skills[k] = L[i];
            i++;
        } else {
            skills[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        skills[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        skills[k] = R[j];
        j++;
        k++;
    }
    
    delete[] L;
    delete[] R;
}

// Quick Sort for skills array
void ResumeLinkedList::quickSortSkills(string *skills, int low, int high) {
    if (low < high) {
        int pi = partitionSkills(skills, low, high);
        quickSortSkills(skills, low, pi - 1);
        quickSortSkills(skills, pi + 1, high);
    }
}

int ResumeLinkedList::partitionSkills(string *skills, int low, int high) {
    string pivot = skills[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (skills[j] < pivot) {
            i++;
            string temp = skills[i];
            skills[i] = skills[j];
            skills[j] = temp;
        }
    }
    
    string temp = skills[i + 1];
    skills[i + 1] = skills[high];
    skills[high] = temp;
    
    return i + 1;
}