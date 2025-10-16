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

    // Break the circular link first
    tail->next = nullptr;

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

void JobCircularLinkedList::append(Job data)
{
    JobNode *newNode = new JobNode(data);

    if (length == 0)
    {
        head = newNode;
        tail = newNode;
        newNode->next = head; // Point to itself (circular)
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
        newNode->next = head; // Point to itself (circular)
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

    // Traverse to second-to-last node
    for (int i = 0; i < length - 2; i++)
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
    if (index < 0 || index >= length || head == nullptr)
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

    JobNode *current = head;
    JobNode *prev = tail;
    JobNode *next = nullptr;

    for (int i = 0; i < length; i++)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    // Swap head and tail
    JobNode *temp = head;
    head = tail;
    tail = temp;
}

int JobCircularLinkedList::getLength()
{
    return length;
}

JobCircularLinkedList JobCircularLinkedList::linearSearchJobByPosition(const string &position)
{
    JobCircularLinkedList jobListByPosition;

    if (position.empty() || head == nullptr)
        return jobListByPosition;

    JobNode *current = head;

    for (int i = 0; i < length; i++)
    {
        if (current->data.position == position)
        {
            jobListByPosition.append(current->data);
        }
        current = current->next;
    }

    return jobListByPosition;
}

JobCircularLinkedList JobCircularLinkedList::linearSearchJobBySkills(const string *skillSet, int skillCount, bool matchAll)
{
    JobCircularLinkedList jobListBySkills;

    if (skillSet == nullptr || skillCount <= 0 || head == nullptr)
        return jobListBySkills;

    JobNode *current = head;

    for (int i = 0; i < length; i++)
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
            jobListBySkills.append(job);

        current = current->next;
    }

    return jobListBySkills;
}

void JobCircularLinkedList::breakCircular()
{
    if (tail != nullptr)
    {
        tail->next = nullptr;
    }
}

void JobCircularLinkedList::restoreCircular()
{
    if (tail != nullptr && head != nullptr)
    {
        tail->next = head;
    }
}

JobNode *JobCircularLinkedList::split(JobNode *start, int count)
{
    if (count <= 1)
        return nullptr;

    JobNode *slow = start;

    for (int i = 0; i < (count / 2) - 1; i++)
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

JobNode *JobCircularLinkedList::mergeSort(JobNode *start, int count, CompareFn compare)
{
    if (!start || count <= 1)
        return start;

    JobNode *second = split(start, count);

    int firstCount = count / 2;
    int secondCount = count - firstCount;

    start = mergeSort(start, firstCount, compare);
    second = mergeSort(second, secondCount, compare);

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

    breakCircular();

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

    // Find new tail and restore circular link
    JobNode *temp = head;
    for (int i = 0; i < length - 1; i++)
    {
        temp = temp->next;
    }
    tail = temp;

    restoreCircular();
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
    if (head == nullptr)
    {
        cout << "\n===== Job Circular Linked List is empty =====\n\n";
        return;
    }

    JobNode *p = head;
    int index = 0;
    cout << "\n===== Job Circular Linked List (Showing first 5 and last 5) =====\n";

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

    if (pivot != nullptr && pivot->next != nullptr && pivot->next != head)
        quickSort(pivot->next, high, type);
}

JobNode *JobCircularLinkedList::sortTail()
{
    if (head == nullptr)
        return nullptr;

    JobNode *temp = head;
    for (int i = 0; i < length - 1; i++)
    {
        temp = temp->next;
    }
    return temp;
}

void JobCircularLinkedList::quickSortBySkillCount()
{
    if (length <= 1)
        return;

    breakCircular();
    JobNode *lastNode = sortTail();
    quickSort(head, lastNode, "skillCount");
    restoreCircular();
}

void JobCircularLinkedList::quickSortBySkill()
{
    if (length <= 1)
        return;

    breakCircular();
    JobNode *lastNode = sortTail();
    quickSort(head, lastNode, "skill");
    restoreCircular();
}

void JobCircularLinkedList::quickSortByPosition()
{
    if (length <= 1)
        return;

    breakCircular();
    JobNode *lastNode = sortTail();
    quickSort(head, lastNode, "position");
    restoreCircular();
}

JobNode *JobCircularLinkedList::getMiddle(JobNode *start, int count)
{
    if (start == nullptr || count <= 0)
        return nullptr;

    JobNode *mid = start;
    for (int i = 0; i < count / 2; i++)
    {
        mid = mid->next;
    }

    return mid;
}

JobCircularLinkedList JobCircularLinkedList::binarySearchJobByPosition(const string &positionKeyword)
{
    JobCircularLinkedList matches;

    if (head == nullptr || positionKeyword.empty())
        return matches;

    quickSortByPosition();

    int start = 0;
    int end = length;

    while (start < end)
    {
        int mid = start + (end - start) / 2;
        JobNode *midNode = get(mid);

        if (midNode == nullptr)
            break;

        string currentPos = midNode->data.position;

        if (currentPos == positionKeyword)
        {
            // Collect all matches
            for (int i = 0; i < length; i++)
            {
                JobNode *node = get(i);
                if (node->data.position == positionKeyword)
                    matches.append(node->data);
            }
            break;
        }
        else if (currentPos < positionKeyword)
        {
            start = mid + 1;
        }
        else
        {
            end = mid;
        }
    }

    return matches;
}

JobCircularLinkedList *JobCircularLinkedList::binarySearchJobBySkills(const string *skills, int skillCount, bool matchAll)
{
    JobCircularLinkedList *matches = new JobCircularLinkedList();

    if (head == nullptr || skills == nullptr || skillCount <= 0)
        return matches;

    quickSortBySkill();

    JobNode *p = head;
    for (int idx = 0; idx < length; idx++)
    {
        int matched = 0;

        for (int s = 0; s < skillCount; ++s)
        {
            bool found = false;
            int left = 0;
            int right = p->data.skillCount - 1;

            while (left <= right)
            {
                int mid = left + (right - left) / 2;
                string key = p->data.skills[mid];

                if (key == skills[s])
                {
                    found = true;
                    break;
                }
                else if (key < skills[s])
                    left = mid + 1;
                else
                    right = mid - 1;
            }

            if (found)
                matched++;
        }

        bool isMatch = matchAll ? (matched == skillCount) : (matched > 0);

        if (isMatch)
            matches->append(p->data);

        p = p->next;
    }

    return matches;
}