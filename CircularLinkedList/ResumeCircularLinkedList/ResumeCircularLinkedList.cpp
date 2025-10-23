#include "ResumeCircularLinkedList.hpp"
#include <algorithm>

ResumeCircularLinkedList::ResumeCircularLinkedList()
{
    head = nullptr;
    tail = nullptr;
    length = 0;
}

ResumeCircularLinkedList::~ResumeCircularLinkedList()
{
    if (head == nullptr)
        return;

    ResumeNode *current = head;
    ResumeNode *temp;

    // Break the circular link first
    tail->next = nullptr;

    while (current != nullptr)
    {
        temp = current;
        current = current->next;
        delete temp;
    }

    head = nullptr;
    tail = nullptr;
    length = 0;
}

ResumeNode* ResumeCircularLinkedList::getHead() const {
    return head;
}

void ResumeCircularLinkedList::append(Resume data)
{
    ResumeNode *newNode = new ResumeNode(data);

    if (length == 0)
    {
        head = newNode;
        tail = newNode;
        newNode->next = head; // Circular link
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
        tail->next = head; // Maintain circular link
    }

    length++;
}

void ResumeCircularLinkedList::prepend(Resume data)
{
    ResumeNode *newNode = new ResumeNode(data);

    if (length == 0)
    {
        head = newNode;
        tail = newNode;
        newNode->next = head; // Circular link
    }
    else
    {
        newNode->next = head;
        head = newNode;
        tail->next = head; // Maintain circular link
    }

    length++;
}

void ResumeCircularLinkedList::deleteFirst()
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
        tail->next = head; // Maintain circular link
    }

    delete temp;
    length--;
}

void ResumeCircularLinkedList::deleteLast()
{
    if (length == 0)
        return;

    if (length == 1)
    {
        delete head;
        head = nullptr;
        tail = nullptr;
        length--;
        return;
    }

    ResumeNode *current = head;
    while (current->next != tail)
    {
        current = current->next;
    }

    delete tail;
    tail = current;
    tail->next = head; // Maintain circular link
    length--;
}

Resume* ResumeCircularLinkedList::findById(int id) {
    if (!head) {
        return nullptr;
    }
    
    ResumeNode* current = head;
    do {
        if (current->data.id == id) {
            return &(current->data);
        }
        current = current->next;
    } while (current != head);  // Stop when we circle back to head
    
    return nullptr;  // Not found
}

ResumeNode *ResumeCircularLinkedList::get(int index)
{
    if (index < 0 || index >= length || length == 0)
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

bool ResumeCircularLinkedList::set(int index, const string *skills, int skillCount)
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

bool ResumeCircularLinkedList::insert(int index, Resume data)
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

void ResumeCircularLinkedList::deleteNode(int index)
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

void ResumeCircularLinkedList::reverse()
{
    if (length <= 1)
        return;

    ResumeNode *prev = tail;
    ResumeNode *current = head;
    ResumeNode *next;

    ResumeNode *oldHead = head;
    head = tail;
    tail = oldHead;

    for (int i = 0; i < length; i++)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
}

int ResumeCircularLinkedList::getLength()
{
    return length;
}

ResumeCircularLinkedList *ResumeCircularLinkedList::linearSearchResumeBySkills(const string *skillSet, int skillCount, bool matchAll)
{
    ResumeCircularLinkedList *resumeListBySkills = new ResumeCircularLinkedList();

    if (skillSet == nullptr || skillCount <= 0 || length == 0)
        return resumeListBySkills;

    ResumeNode *current = head;

    for (int i = 0; i < length; i++)
    {
        const Resume &resume = current->data;
        int matchCount = 0;

        for (int j = 0; j < skillCount; ++j)
        {
            for (int k = 0; k < resume.skillCount; ++k)
            {
                if (skillSet[j] == resume.skills[k])
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

ResumeNode *ResumeCircularLinkedList::split(ResumeNode *start, int len)
{
    if (len <= 1)
        return nullptr;

    ResumeNode *slow = start;
    int mid = len / 2;

    for (int i = 0; i < mid - 1; i++)
    {
        slow = slow->next;
    }

    ResumeNode *second = slow->next;
    slow->next = nullptr;
    return second;
}

ResumeNode *ResumeCircularLinkedList::merge(ResumeNode *first, ResumeNode *second, CompareFn compare)
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

ResumeNode *ResumeCircularLinkedList::mergeSort(ResumeNode *start, int len, CompareFn compare)
{
    if (!start || len <= 1)
        return start;

    ResumeNode *second = split(start, len);

    int firstLen = len / 2;
    int secondLen = len - firstLen;

    start = mergeSort(start, firstLen, compare);
    second = mergeSort(second, secondLen, compare);

    return merge(start, second, compare);
}

bool ResumeCircularLinkedList::compareById(ResumeNode *a, ResumeNode *b)
{
    return a->data.id < b->data.id;
}

bool ResumeCircularLinkedList::compareBySkillCount(ResumeNode *a, ResumeNode *b)
{
    return a->data.skillCount < b->data.skillCount;
}

bool ResumeCircularLinkedList::compareBySkill(ResumeNode *a, ResumeNode *b)
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

void ResumeCircularLinkedList::mergeSortBy(const string &criterion)
{
    if (length <= 1)
        return;

    // Break circular link for sorting
    tail->next = nullptr;

    if (criterion == "id")
    {
        head = mergeSort(head, length, compareById);
    }
    else if (criterion == "skillCount")
    {
        head = mergeSort(head, length, compareBySkillCount);
    }
    else if (criterion == "skill")
    {
        head = mergeSort(head, length, compareBySkill);
    }

    // Find new tail efficiently - only needed after merge sort
    if (head) {
        tail = head;
        while (tail->next != nullptr)
            tail = tail->next;
        tail->next = head;
    }
}


// ======= Clean String =======
string ResumeCircularLinkedList::cleanString(string s)
{
    s.erase(remove(s.begin(), s.end(), '"'), s.end());
    while (!s.empty() && isspace(s.front()))
        s.erase(s.begin());
    while (!s.empty() && isspace(s.back()))
        s.pop_back();
    return s;
}

// ======= Display Slice =======
void ResumeCircularLinkedList::printSlice()
{
    if (length == 0)
    {
        cout << "\n===== Resume Circular Linked List (Empty) =====\n\n";
        return;
    }

    ResumeNode *p = head;
    int index = 0;

    cout << "\n===== Resume Circular Linked List (Showing first 10 and last 10) =====\n";

    for (int i = 0; i < length; i++)
    {
        if (index < 5 || index >= length - 5)
        {
            cout << "|" << p->data.id << " | Skills: ";
            for (int j = 0; j < p->data.skillCount; j++)
            {
                cout << p->data.skills[j];
                if (j < p->data.skillCount - 1)
                    cout << ", ";
            }
            cout << " | Total Skills: " << p->data.skillCount << endl;
        }
        else if (index == 5)
        {
            cout << "...\n...\n...(skipping " << length - 10 << " resumes)...\n...\n...\n";
        }

        p = p->next;
        index++;
    }

    cout << endl;
}

// ======= Swap =======
void ResumeCircularLinkedList::swap(ResumeNode *a, ResumeNode *b)
{
    // Swap the entire Resume data structure
    Resume temp = a->data;
    a->data = b->data;
    b->data = temp;
}

// ======= Partition =======
ResumeNode *ResumeCircularLinkedList::partitionBySkillCount(ResumeNode *low, ResumeNode *high)
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

ResumeNode *ResumeCircularLinkedList::partitionBySkill(ResumeNode *low, ResumeNode *high)
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
void ResumeCircularLinkedList::quickSort(ResumeNode *low, ResumeNode *high, const string &type)
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

ResumeNode *ResumeCircularLinkedList::sortTail()
{
    if (!head || length == 0)
        return nullptr;

    ResumeNode *temp = head;
    for (int i = 0; i < length - 1; i++)
        temp = temp->next;
    return temp;
}

// ======= Sort Wrappers =======
void ResumeCircularLinkedList::quickSortBySkillCount()
{
    if (!head || length <= 1)
        return;

    tail->next = nullptr; // Break circular link
    quickSort(head, tail, "skillCount");  // Use tail directly!

    // Restore circular link - tail is still valid
    tail->next = head;
}

void ResumeCircularLinkedList::quickSortBySkill()
{
    if (!head || length <= 1)
        return;

    tail->next = nullptr; // Break circular link
    quickSort(head, tail, "skill");  // Use tail directly!

    // Restore circular link
    tail->next = head;
}

ResumeCircularLinkedList *ResumeCircularLinkedList::binarySearchResumeBySkills(const string *skills, int skillCount, bool matchAll, SortAlgorithm sortAlgo)
{
    ResumeCircularLinkedList *matches = new ResumeCircularLinkedList();

    if (head == nullptr || skills == nullptr || skillCount <= 0)
        return matches;

    ResumeNode *p = head;
    for (int idx = 0; idx < length; idx++)
    {
        // Sort this resume's skills array using the specified algorithm
        if (sortAlgo == QUICK) {
            quickSortSkills(p->data.skills, 0, p->data.skillCount - 1);
        } else {
            mergeSortSkills(p->data.skills, 0, p->data.skillCount - 1);
        }

        int matched = 0;

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

        bool isMatch = matchAll ? (matched == skillCount) : (matched > 0);

        if (isMatch)
            matches->append(p->data);

        p = p->next;
    }

    return matches;
}

// Merge Sort for skills array
void ResumeCircularLinkedList::mergeSortSkills(string *skills, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortSkills(skills, left, mid);
        mergeSortSkills(skills, mid + 1, right);
        mergeSkills(skills, left, mid, right);
    }
}

void ResumeCircularLinkedList::mergeSkills(string *skills, int left, int mid, int right) {
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
void ResumeCircularLinkedList::quickSortSkills(string *skills, int low, int high) {
    if (low < high) {
        int pi = partitionSkills(skills, low, high);
        quickSortSkills(skills, low, pi - 1);
        quickSortSkills(skills, pi + 1, high);
    }
}

int ResumeCircularLinkedList::partitionSkills(string *skills, int low, int high) {
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