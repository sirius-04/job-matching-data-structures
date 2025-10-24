#include "JobCircularLinkedList.hpp"
#include <algorithm>

JobCircularLinkedList::JobCircularLinkedList()
{
    head = nullptr;
    tail = nullptr;
    length = 0;
}

JobCircularLinkedList::~JobCircularLinkedList()
{
    if (head == nullptr)
        return;

    JobNode *current = head;
    JobNode *temp;

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

JobNode *JobCircularLinkedList::getHead() const
{
    return head;
}

void JobCircularLinkedList::append(Job data)
{
    JobNode *newNode = new JobNode(data);

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

void JobCircularLinkedList::prepend(Job data)
{
    JobNode *newNode = new JobNode(data);

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

void JobCircularLinkedList::deleteFirst()
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
        tail->next = head; // Maintain circular link
    }

    delete temp;
    length--;
}

void JobCircularLinkedList::deleteLast()
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

    JobNode *current = head;
    while (current->next != tail)
    {
        current = current->next;
    }

    delete tail;
    tail = current;
    tail->next = head; // Maintain circular link
    length--;
}

JobNode *JobCircularLinkedList::get(int index)
{
    if (index < 0 || index >= length || length == 0)
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

Job *JobCircularLinkedList::findById(int id)
{
    if (!head)
        return nullptr;

    JobNode *current = head;
    do
    {
        if (current->data.id == id)
            return &current->data;
        current = current->next;
    } while (current != head);

    return nullptr;
}

bool JobCircularLinkedList::set(int index, const string &position, const string *skills, int skillCount)
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

bool JobCircularLinkedList::insert(int index, Job data)
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

void JobCircularLinkedList::deleteNode(int index)
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

void JobCircularLinkedList::reverse()
{
    if (length <= 1)
        return;

    JobNode *prev = tail;
    JobNode *current = head;
    JobNode *next;

    JobNode *oldHead = head;
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

int JobCircularLinkedList::getLength()
{
    return length;
}

JobCircularLinkedList *JobCircularLinkedList::linearSearchJobByPosition(const string &position)
{
    if (position.empty() || length == 0)
        return nullptr;

    JobCircularLinkedList *jobListByPosition = new JobCircularLinkedList();

    JobNode *current = head;
    for (int i = 0; i < length; i++)
    {
        if (current->data.position == position)
        {
            jobListByPosition->append(current->data);
        }
        current = current->next;
    }

    return jobListByPosition;
}

JobCircularLinkedList *JobCircularLinkedList::linearSearchJobBySkills(const string *skillSet, int skillCount, bool matchAll)
{
    if (skillSet == nullptr || skillCount <= 0)
        return new JobCircularLinkedList();

    JobCircularLinkedList *jobListBySkills = new JobCircularLinkedList();

    if (length == 0)
        return jobListBySkills;

    JobNode *current = head;

    for (int i = 0; i < length; i++)
    {
        const Job &job = current->data;
        int matchCount = 0;

        for (int j = 0; j < skillCount; ++j)
        {
            for (int k = 0; k < job.skillCount; ++k)
            {
                if (skillSet[j] == job.skills[k])
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

JobNode *JobCircularLinkedList::split(JobNode *start, int len)
{
    if (len <= 1)
        return nullptr;

    JobNode *slow = start;
    int mid = len / 2;

    for (int i = 0; i < mid - 1; i++)
    {
        slow = slow->next;
    }

    JobNode *second = slow->next;
    slow->next = nullptr;
    return second;
}

JobNode *JobCircularLinkedList::merge(JobNode *first, JobNode *second, CompareFn compare)
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

JobNode *JobCircularLinkedList::mergeSort(JobNode *start, int len, CompareFn compare)
{
    if (!start || len <= 1)
        return start;

    JobNode *second = split(start, len);

    int firstLen = len / 2;
    int secondLen = len - firstLen;

    start = mergeSort(start, firstLen, compare);
    second = mergeSort(second, secondLen, compare);

    return merge(start, second, compare);
}

bool JobCircularLinkedList::compareByPosition(JobNode *a, JobNode *b)
{
    return a->data.position < b->data.position;
}

bool JobCircularLinkedList::compareBySkillCount(JobNode *a, JobNode *b)
{
    return a->data.skillCount < b->data.skillCount;
}

bool JobCircularLinkedList::compareBySkill(JobNode *a, JobNode *b)
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

void JobCircularLinkedList::mergeSortBy(string criterion)
{
    if (length <= 1)
        return;

    // Break circular link for sorting
    tail->next = nullptr;

    if (criterion == "position")
    {
        head = mergeSort(head, length, compareByPosition);
    }
    else if (criterion == "skillCount")
    {
        head = mergeSort(head, length, compareBySkillCount);
    }
    else if (criterion == "skill")
    {
        head = mergeSort(head, length, compareBySkill);
    }

    // Find new tail efficiently during restoration
    if (head)
    {
        tail = head;
        while (tail->next != nullptr)
            tail = tail->next;
        tail->next = head;
    }
}

string JobCircularLinkedList::cleanString(string s)
{
    s.erase(remove(s.begin(), s.end(), '"'), s.end());
    while (!s.empty() && isspace(s.front()))
        s.erase(s.begin());
    while (!s.empty() && isspace(s.back()))
        s.pop_back();
    return s;
}

void JobCircularLinkedList::printSlice()
{
    if (length == 0)
    {
        cout << "\n===== Job Circular Linked List (Empty) =====\n\n";
        return;
    }

    JobNode *p = head;
    int index = 0;
    cout << "\n===== Job Circular Linked List (Showing first 10 and last 10) =====\n";

    for (int i = 0; i < length; i++)
    {
        if (index < 5 || index >= length - 5)
        {
            cout << "|" << p->data.id << "| " << p->data.position << " | Skills: ";
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
            cout << "...\n...\n...(skipping " << length - 10 << " jobs)...\n...\n...\n";
        }

        p = p->next;
        index++;
    }

    cout << endl;
}

void JobCircularLinkedList::swap(JobNode *a, JobNode *b)
{
    std::swap(a->data.id, b->data.id);
    std::swap(a->data.position, b->data.position);
    std::swap(a->data.skills, b->data.skills);
    std::swap(a->data.skillCount, b->data.skillCount);
}

JobNode *JobCircularLinkedList::partitionBySkillCount(JobNode *low, JobNode *high)
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

JobNode *JobCircularLinkedList::partitionBySkill(JobNode *low, JobNode *high)
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

JobNode *JobCircularLinkedList::partitionByPosition(JobNode *low, JobNode *high)
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

void JobCircularLinkedList::quickSort(JobNode *low, JobNode *high, const string &type)
{
    if (!low || !high || low == high)
        return;

    JobNode *temp = low;
    bool validRange = false;
    while (temp != nullptr)
    {
        if (temp == high)
        {
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

void JobCircularLinkedList::quickSortBySkillCount()
{
    if (length <= 1)
        return;

    tail->next = nullptr;                // Break circular link
    quickSort(head, tail, "skillCount"); // Use tail directly!

    // Restore circular link - tail is still valid
    tail->next = head;
}

void JobCircularLinkedList::quickSortBySkill()
{
    if (length <= 1)
        return;

    tail->next = nullptr;           // Break circular link
    quickSort(head, tail, "skill"); // Use tail directly!

    // Restore circular link
    tail->next = head;
}

void JobCircularLinkedList::quickSortByPosition()
{
    if (length <= 1)
        return;

    tail->next = nullptr;              // Break circular link
    quickSort(head, tail, "position"); // Use tail directly!

    // Restore circular link
    tail->next = head;
}

JobNode *JobCircularLinkedList::getMiddle(JobNode *start, JobNode *end)
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

JobCircularLinkedList *JobCircularLinkedList::binarySearchJobByPosition(
    const string &positionKeyword, SortAlgorithm sortAlgo)
{
    if (head == nullptr)
        return new JobCircularLinkedList();

    JobCircularLinkedList *matches = new JobCircularLinkedList();

    // Break circular link ONCE
    tail->next = nullptr;

    // Sort
    if (sortAlgo == QUICK)
    {
        quickSort(head, tail, "position");
    }
    else
    {
        head = mergeSort(head, length, compareByPosition);
        // Update tail after merge sort
        tail = head;
        while (tail->next != nullptr)
            tail = tail->next;
    }

    // Binary search (already broken, no need to break again)
    JobNode *start = head;
    JobNode *end = nullptr;

    while (start != end)
    {
        JobNode *mid = getMiddle(start, end);
        if (mid == nullptr)
            break;

        string currentPos = mid->data.position;

        if (currentPos == positionKeyword)
        {
            matches->append(mid->data);

            JobNode *left = head;
            while (left != mid)
            {
                if (left->data.position == positionKeyword)
                    matches->append(left->data);
                left = left->next;
            }

            JobNode *right = mid->next;
            while (right != end)
            {
                if (right->data.position == positionKeyword)
                    matches->append(right->data);
                right = right->next;
            }

            break;
        }
        else if (currentPos < positionKeyword)
        {
            start = mid->next;
        }
        else
        {
            end = mid;
        }
    }

    // Restore circular link ONCE at the end
    tail->next = head;

    return matches;
}

JobCircularLinkedList *JobCircularLinkedList::binarySearchJobBySkills(const string *skills, int skillCount, bool matchAll, SortAlgorithm sortAlgo)
{
    JobCircularLinkedList *matches = new JobCircularLinkedList();

    if (head == nullptr || skills == nullptr || skillCount <= 0)
        return matches;

    // For each target skill, find all nodes containing it
    JobCircularLinkedList **skillMatches = new JobCircularLinkedList *[skillCount];

    for (int s = 0; s < skillCount; ++s)
    {
        string targetSkill = skills[s];
        skillMatches[s] = new JobCircularLinkedList();

        // Sort the linked list NODES by skill (node-based sorting)
        // Note: These methods already handle breaking/restoring circular link
        if (sortAlgo == QUICK)
        {
            quickSortBySkill();
        }
        else
        {
            mergeSortBy("skill");
        }

        // Traverse all nodes to find those containing targetSkill
        // Use counter-based traversal to handle circular list properly
        JobNode *p = head;
        for (int idx = 0; idx < length; idx++)
        {
            bool hasTargetSkill = false;
            for (int i = 0; i < p->data.skillCount; ++i)
            {
                if (p->data.skills[i] == targetSkill)
                {
                    hasTargetSkill = true;
                    break;
                }
            }

            if (hasTargetSkill)
            {
                skillMatches[s]->append(p->data);
            }

            p = p->next; // Safe because circular link is maintained
        }
    }

    // Combine results based on matchAll
    if (matchAll)
    {
        // Intersection: job must be in all skillMatches lists
        if (skillCount > 0 && skillMatches[0]->length > 0)
        {
            JobNode *p = skillMatches[0]->head;
            for (int idx = 0; idx < skillMatches[0]->length; idx++)
            {
                bool inAll = true;
                for (int s = 1; s < skillCount; ++s)
                {
                    bool foundInList = false;
                    JobNode *q = skillMatches[s]->head;
                    for (int jdx = 0; jdx < skillMatches[s]->length; jdx++)
                    {
                        if (p->data.id == q->data.id)
                        {
                            foundInList = true;
                            break;
                        }
                        q = q->next;
                    }
                    if (!foundInList)
                    {
                        inAll = false;
                        break;
                    }
                }

                if (inAll)
                    matches->append(p->data);

                p = p->next;
            }
        }
    }
    else
    {
        // Union: job must be in at least one skillMatches list
        for (int s = 0; s < skillCount; ++s)
        {
            if (skillMatches[s]->length == 0)
                continue;

            JobNode *p = skillMatches[s]->head;
            for (int idx = 0; idx < skillMatches[s]->length; idx++)
            {
                // Check if already added
                bool alreadyAdded = false;
                if (matches->length > 0)
                {
                    JobNode *q = matches->head;
                    for (int jdx = 0; jdx < matches->length; jdx++)
                    {
                        if (p->data.id == q->data.id)
                        {
                            alreadyAdded = true;
                            break;
                        }
                        q = q->next;
                    }
                }

                if (!alreadyAdded)
                    matches->append(p->data);

                p = p->next;
            }
        }
    }

    // Clean up
    for (int s = 0; s < skillCount; ++s)
    {
        delete skillMatches[s];
    }
    delete[] skillMatches;

    return matches;
}

// Merge Sort for skills array
void JobCircularLinkedList::mergeSortSkills(string *skills, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSortSkills(skills, left, mid);
        mergeSortSkills(skills, mid + 1, right);
        mergeSkills(skills, left, mid, right);
    }
}

void JobCircularLinkedList::mergeSkills(string *skills, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    string *L = new string[n1];
    string *R = new string[n2];

    for (int i = 0; i < n1; i++)
        L[i] = skills[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = skills[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            skills[k] = L[i];
            i++;
        }
        else
        {
            skills[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        skills[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        skills[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

// Quick Sort for skills array
void JobCircularLinkedList::quickSortSkills(string *skills, int low, int high)
{
    if (low < high)
    {
        int pi = partitionSkills(skills, low, high);
        quickSortSkills(skills, low, pi - 1);
        quickSortSkills(skills, pi + 1, high);
    }
}

int JobCircularLinkedList::partitionSkills(string *skills, int low, int high)
{
    string pivot = skills[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (skills[j] < pivot)
        {
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