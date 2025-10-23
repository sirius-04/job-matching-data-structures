#include "JobLinkedList.hpp"
#include <algorithm>

JobLinkedList::JobLinkedList()
{
    head = nullptr;
    tail = nullptr;
    length = 0;
}

JobLinkedList::~JobLinkedList()
{
    JobNode *current = head;
    while (current != nullptr)
    {
        JobNode *temp = current;
        current = current->next;
        delete temp;
    }

    head = nullptr;
    tail = nullptr;
    length = 0;
}

JobNode* JobLinkedList::getHead() const {
    return head;
}

void JobLinkedList::append(Job data)
{
    JobNode *newNode = new JobNode(data);

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

void JobLinkedList::prepend(Job data)
{
    JobNode *newNode = new JobNode(data);

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

void JobLinkedList::deleteFirst()
{
    if (length == 0)
        return;

    JobNode *temp = head;

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

void JobLinkedList::deleteLast()
{
    if (length == 0)
        return;

    JobNode *temp = head;

    if (length == 1)
    {
        delete head;

        head = nullptr;
        tail = nullptr;
    }
    else
    {
        JobNode *pre = head;

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

JobNode *JobLinkedList::get(int index)
{
    if (index < 0 || index >= length)
    {
        return nullptr;
    }

    JobNode *temp = head;

    for (int i = 0; i < index; i++)
    {
        temp = temp->next;
    }

    return temp;
}

Job* JobLinkedList::findById(int id)
{
    if (!head)
        return nullptr;

    JobNode* current = head;
    while (current != nullptr) {
        if (current->data.id == id)
            return &current->data;
        current = current->next;
    }

    return nullptr;
}

bool JobLinkedList::set(int index, const string &position, const string *skills, int skillCount)
{
    JobNode *temp = get(index);
    if (!temp)
        return false;

    Job &job = temp->data;

    job.position = position;

    delete[] job.skills;
    job.skills = new string[skillCount];
    for (int i = 0; i < skillCount; ++i)
    {
        job.skills[i] = skills[i];
    }

    job.skillCount = skillCount;

    return true;
}

bool JobLinkedList::insert(int index, Job data)
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
        JobNode *newNode = new JobNode(data);
        JobNode *temp = get(index - 1);

        newNode->next = temp->next;
        temp->next = newNode;
        length++;
    }
    return true;
}

void JobLinkedList::deleteNode(int index)
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
        JobNode *prev = get(index - 1);
        JobNode *temp = prev->next;

        prev->next = temp->next;

        delete temp;
        length--;
    }
}

void JobLinkedList::reverse()
{
    if (length <= 1)
        return;

    JobNode *temp = head;
    head = tail;
    tail = temp;
    JobNode *after = temp->next;
    JobNode *before = nullptr;

    for (int i = 0; i < length; i++)
    {
        after = temp->next;
        temp->next = before;
        before = temp;
        temp = after;
    }
}

int JobLinkedList::getLength()
{
    return length;
}

JobLinkedList *JobLinkedList::linearSearchJobByPosition(const string &position)
{
    if (position.empty())
        return nullptr;

    JobLinkedList *jobListByPosition = new JobLinkedList();

    JobNode *current = head;
    while (current != nullptr)
    {
        if (current->data.position == position)
        {
            jobListByPosition->append(current->data);
        }

        current = current->next;
    }

    return jobListByPosition;
}

JobLinkedList *JobLinkedList::linearSearchJobBySkills(const string *skillSet, int skillCount, bool matchAll)
{
    if (skillSet == nullptr || skillCount <= 0)
        return new JobLinkedList(); // safer

    JobLinkedList *jobListBySkills = new JobLinkedList();
    JobNode *current = head;

    while (current != nullptr)
    {
        const Job &job = current->data;
        int matchCount = 0;

        for (int i = 0; i < skillCount; ++i)
        {
            for (int j = 0; j < job.skillCount; ++j)
            {
                if (skillSet[i] == job.skills[j])
                {
                    matchCount++;
                    break;
                }
            }
        }

        bool isMatch = matchAll ? (matchCount == skillCount) : (matchCount > 0);

        if (isMatch)
            jobListBySkills->append(job);

        current = current->next;
    }

    return jobListBySkills;
}

JobNode *JobLinkedList::split(JobNode *head)
{
    JobNode *fast = head;
    JobNode *slow = head;

    while (fast->next != nullptr && fast->next->next != nullptr)
    {
        fast = fast->next->next;
        slow = slow->next;
    }

    JobNode *second = slow->next;
    slow->next = nullptr;
    return second;
}

JobNode *JobLinkedList::merge(JobNode *first, JobNode *second, CompareFn compare)
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

JobNode *JobLinkedList::mergeSort(JobNode *head, CompareFn compare)
{
    if (!head || !head->next)
        return head;

    JobNode *second = split(head);

    head = mergeSort(head, compare);
    second = mergeSort(second, compare);

    return merge(head, second, compare);
}

bool JobLinkedList::compareByPosition(JobNode *a, JobNode *b)
{
    return a->data.position < b->data.position;
}

bool JobLinkedList::compareBySkillCount(JobNode *a, JobNode *b)
{
    return a->data.skillCount < b->data.skillCount;
}

bool JobLinkedList::compareBySkill(JobNode *a, JobNode *b)
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

void JobLinkedList::mergeSortBy(string criterion)
{
    if (criterion == "position")
    {
        head = mergeSort(head, compareByPosition);
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

// ======= Clean String (remove quotes and trim spaces) =======
string JobLinkedList::cleanString(string s)
{
    s.erase(remove(s.begin(), s.end(), '"'), s.end());
    while (!s.empty() && isspace(s.front()))
        s.erase(s.begin());
    while (!s.empty() && isspace(s.back()))
        s.pop_back();
    return s;
}

// ======= Display First 10 + Last 10 =======
void JobLinkedList::printSlice()
{
    int total = 0;
    JobNode *p = head;
    while (p != NULL)
    {
        total++;
        p = p->next;
    }

    p = head;
    int index = 0;
    cout << "\n===== Job Linked List (Showing first 10 and last 10) =====\n";

    while (p != nullptr)
    {
        if (index < 5 || index >= total - 5)
        {
            cout << "|" << p->data.id << "| " << p->data.position << " | Skills: ";
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
            cout << "...\n...\n...(skipping " << total - 10 << " jobs)...\n...\n...\n";
        }

        p = p->next;
        index++;
    }

    cout << endl;
}

// ======= Swap Utility =======
void JobLinkedList::swap(JobNode *a, JobNode *b)
{
    std::swap(a->data.id, b->data.id);
    std::swap(a->data.position, b->data.position);
    std::swap(a->data.skills, b->data.skills);
    std::swap(a->data.skillCount, b->data.skillCount);
}

// ======= Partition =======
JobNode *JobLinkedList::partitionBySkillCount(JobNode *low, JobNode *high)
{
    int pivot = high->data.skillCount;
    JobNode *i = low;
    for (JobNode *j = low; j != high; j = j->next)
    {
        if (j->data.skillCount < pivot)
        {
            swap(i, j);
            i = i->next;
        }
    }
    swap(i, high);
    return i;
}

JobNode *JobLinkedList::partitionBySkill(JobNode *low, JobNode *high)
{
    string pivot = (high->data.skillCount > 0) ? high->data.skills[0] : "";
    JobNode *i = low;
    for (JobNode *j = low; j != high; j = j->next)
    {
        string firstSkillJ = (j->data.skillCount > 0) ? j->data.skills[0] : "";
        if (firstSkillJ < pivot)
        {
            swap(i, j);
            i = i->next;
        }
    }
    swap(i, high);
    return i;
}

JobNode *JobLinkedList::partitionByPosition(JobNode *low, JobNode *high)
{
    string pivot = high->data.position;
    JobNode *i = low;
    for (JobNode *j = low; j != high; j = j->next)
    {
        if (j->data.position < pivot)
        {
            swap(i, j);
            i = i->next;
        }
    }
    swap(i, high);
    return i;
}

// ======= QuickSort Core =======
void JobLinkedList::quickSort(JobNode *low, JobNode *high, const string &type)
{
    if (!low || !high || low == high)
        return;

    JobNode *temp = low;
    bool validRange = false;
    while (temp != nullptr) {
        if (temp == high) {
            validRange = true;
            break;
        }
        temp = temp->next;
    }
    if (!validRange)
        return;

    JobNode *pivot = nullptr;
    if (type == "skillCount")
        pivot = partitionBySkillCount(low, high);
    else if (type == "skill")
        pivot = partitionBySkill(low, high);
    else if (type == "position")
        pivot = partitionByPosition(low, high);

    JobNode *beforePivot = low;
    while (beforePivot && beforePivot->next != pivot)
        beforePivot = beforePivot->next;

    if (beforePivot != nullptr && low != pivot)
        quickSort(low, beforePivot, type);

    if (pivot != nullptr && pivot->next != nullptr)
        quickSort(pivot->next, high, type);
}

JobNode *JobLinkedList::sortTail()
{
    JobNode *temp = head;
    while (temp && temp->next)
        temp = temp->next;
    return temp;
}

// ======= Sort Wrappers =======
void JobLinkedList::quickSortBySkillCount()
{
    JobNode *lastNode = sortTail();
    quickSort(head, lastNode, "skillCount");
}

void JobLinkedList::quickSortBySkill()
{
    JobNode *lastNode = sortTail();
    quickSort(head, lastNode, "skill");
}

void JobLinkedList::quickSortByPosition()
{
    JobNode *lastNode = sortTail();
    quickSort(head, lastNode, "position");
}

JobNode *JobLinkedList::getMiddle(JobNode *start, JobNode *end)
{
    if (start == nullptr)
        return nullptr;

    JobNode *slow = start;
    JobNode *fast = start->next;

    while (fast != end)
    {
        fast = fast->next;
        if (fast != end)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    return slow;
}

JobLinkedList *JobLinkedList::binarySearchJobByPosition(const string &positionKeyword, SortAlgorithm sortAlgo)
{
    // If the list is empty, return an empty result pointer
    if (head == nullptr)
        return new JobLinkedList();

    // Create a new list to hold matches
    JobLinkedList *matches = new JobLinkedList();

    // Sort the list by position before binary searching
    if (sortAlgo == QUICK) {
        quickSortByPosition();
    } else {
        mergeSortBy("position");
    }

    JobNode *start = head;
    JobNode *end = nullptr;

    // Perform binary search on the linked list
    while (start != end)
    {
        JobNode *mid = getMiddle(start, end);
        if (mid == nullptr)
            break;

        string currentPos = mid->data.position;

        if (currentPos == positionKeyword)
        {
            // Add the mid node
            matches->append(mid->data);

            // Search left side for duplicates
            JobNode *left = head;
            while (left != mid)
            {
                if (left->data.position == positionKeyword)
                    matches->append(left->data);
                left = left->next;
            }

            // Search right side for duplicates
            JobNode *right = mid->next;
            while (right != end)
            {
                if (right->data.position == positionKeyword)
                    matches->append(right->data);
                right = right->next;
            }

            break; // all matches collected
        }
        else if (currentPos < positionKeyword)
        {
            start = mid->next; // search right half
        }
        else
        {
            end = mid; // search left half
        }
    }

    return matches;
}

// ======= True Binary Search on Linked List =======
JobLinkedList *JobLinkedList::binarySearchJobBySkills(const string *skills, int skillCount, bool matchAll, SortAlgorithm sortAlgo)
{
    JobLinkedList *matches = new JobLinkedList();

    if (head == nullptr || skills == nullptr || skillCount <= 0)
        return matches;

    for (JobNode *p = head; p != nullptr; p = p->next)
    {
        // Sort this job's skills array using the specified algorithm
        if (sortAlgo == QUICK) {
            quickSortSkills(p->data.skills, 0, p->data.skillCount - 1);
            
        } else {
            mergeSortSkills(p->data.skills, 0, p->data.skillCount - 1);
        }

        int matched = 0;

        // For each skill to search
        for (int s = 0; s < skillCount; ++s)
        {
            bool found = false;
            int left = 0;
            int right = p->data.skillCount - 1;
            
            string target = skills[s];

            // Binary search through this job's sorted skill array
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

        // Determine match criteria
        bool isMatch = matchAll ? (matched == skillCount) : (matched > 0);

        if (isMatch)
            matches->append(p->data);
    }

    return matches;
}

// Merge Sort for skills array
void JobLinkedList::mergeSortSkills(string *skills, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortSkills(skills, left, mid);
        mergeSortSkills(skills, mid + 1, right);
        mergeSkills(skills, left, mid, right);
    }
}

void JobLinkedList::mergeSkills(string *skills, int left, int mid, int right) {
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
void JobLinkedList::quickSortSkills(string *skills, int low, int high) {
    if (low < high) {
        int pi = partitionSkills(skills, low, high);
        quickSortSkills(skills, low, pi - 1);
        quickSortSkills(skills, pi + 1, high);
    }
}

int JobLinkedList::partitionSkills(string *skills, int low, int high) {
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
