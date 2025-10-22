#include "JobArray.hpp"
#include <fstream>
#include <sstream>

// normalize string
inline std::string normalizeString(std::string s)
{
    // Trim whitespace
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    // Convert to lowercase
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// ======================= JobArray Implementation =======================

JobArray::JobArray()
{
    capacity = 1000;
    size = 0;
    jobs = new Job[capacity];
}

JobArray::~JobArray()
{
    delete[] jobs;
}

void JobArray::resize()
{
    capacity *= 2;
    Job *newJobs = new Job[capacity];
    for (int i = 0; i < size; i++)
        newJobs[i] = jobs[i];
    delete[] jobs;
    jobs = newJobs;
}

int JobArray::getSize() const
{
    return size;
}

Job JobArray::getJob(int index) const
{
    if (index < 0 || index >= size)
    {
        cerr << "Error: Job index out of bounds: " << index << endl;
        return Job();
    }
    return jobs[index];
}

Job *JobArray::findById(int id)
{
    for (int i = 0; i < size; i++)
    {
        if (jobs[i].id == id)
            return &jobs[i]; // return pointer to matching job
    }
    return nullptr; // not found
}

void JobArray::addJob(int id, string position, string *skills, int skillCount)
{
    if (size >= capacity)
        resize();

    jobs[size].id = id;
    jobs[size].position = position;
    jobs[size].skillCount = skillCount;
    jobs[size].skills = new string[skillCount];
    for (int i = 0; i < skillCount; i++)
        jobs[size].skills[i] = skills[i];

    size++;
}

void JobArray::printJobs()
{
    cout << "\n--- Job Descriptions for Array ---\n";

    if (size <= 20)
    {
        for (int i = 0; i < size; i++)
        {
            cout << jobs[i].id << " | " << jobs[i].position << " | ";
            for (int j = 0; j < jobs[i].skillCount; j++)
            {
                cout << jobs[i].skills[j];
                if (j < jobs[i].skillCount - 1)
                    cout << ",";
            }
            cout << " | Total Skills: " << jobs[i].skillCount << endl;
        }
    }
    else
    {
        // Print first 10
        for (int i = 0; i < 10; i++)
        {
            cout << jobs[i].id << " | " << jobs[i].position << " | ";
            for (int j = 0; j < jobs[i].skillCount; j++)
            {
                cout << jobs[i].skills[j];
                if (j < jobs[i].skillCount - 1)
                    cout << ",";
            }
            cout << " | Total Skills: " << jobs[i].skillCount << endl;
        }

        cout << "...\n... (skipping " << (size - 20) << " jobs) ...\n...\n";

        // Print last 10
        for (int i = size - 10; i < size; i++)
        {
            cout << jobs[i].id << " | " << jobs[i].position << " | ";
            for (int j = 0; j < jobs[i].skillCount; j++)
            {
                cout << jobs[i].skills[j];
                if (j < jobs[i].skillCount - 1)
                    cout << ",";
            }
            cout << " | Total Skills: " << jobs[i].skillCount << endl;
        }
    }

    cout << "\nTotal jobs printed: " << (size <= 20 ? size : 20) << " out of " << size << endl;
}

// ======================= Linear Search =======================
JobArray *JobArray::linearSearchBySkills(const string *skillSet, int skillCount, bool matchAll)
{
    JobArray *result = new JobArray();

    for (int i = 0; i < size; i++)
    {
        int matchedSkills = 0;

        for (int j = 0; j < skillCount; j++)
        {
            string target = normalizeString(skillSet[j]);

            for (int k = 0; k < jobs[i].skillCount; k++)
            {
                string jobSkill = normalizeString(jobs[i].skills[k]);

                if (jobSkill == target)
                {
                    matchedSkills++;
                    break;
                }
            }
        }

        bool isMatch = matchAll ? (matchedSkills == skillCount) : (matchedSkills > 0);

        if (isMatch)
        {
            result->addJob(jobs[i].id, jobs[i].position, jobs[i].skills, jobs[i].skillCount);
        }
    }

    return result;
}

JobArray *JobArray::linearSearchByPosition(const string &position)
{
    JobArray *result = new JobArray();
    string target = normalizeString(position);

    for (int i = 0; i < size; i++)
    {
        string jobPos = normalizeString(jobs[i].position);
        if (jobPos == target)
        {
            result->addJob(jobs[i].id, jobs[i].position, jobs[i].skills, jobs[i].skillCount);
        }
    }

    return result;
}

// ======================= Merge Sort =======================
bool JobArray::compareById(const Job &a, const Job &b)
{
    return a.id < b.id;
}
bool JobArray::compareByFirstSkill(const Job &a, const Job &b)
{
    if (a.skillCount == 0)
        return true;
    if (b.skillCount == 0)
        return false;
    return a.skills[0] < b.skills[0];
}

bool JobArray::compareBySkillCount(const Job &a, const Job &b)
{
    return a.skillCount < b.skillCount;
}

bool JobArray::compareByPosition(const Job &a, const Job &b)
{
    return a.position < b.position;
}

void JobArray::merge(Job *arr, int left, int mid, int right, CompareFn compare)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Job *L = new Job[n1];
    Job *R = new Job[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (compare(L[i], R[j]))
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void JobArray::mergeSortHelper(Job *arr, int left, int right, CompareFn compare)
{
    if (left < right)
    {
        int mid = (left + right) / 2;
        mergeSortHelper(arr, left, mid, compare);
        mergeSortHelper(arr, mid + 1, right, compare);
        merge(arr, left, mid, right, compare);
    }
}

void JobArray::mergeSort(CompareFn compare)
{
    if (size > 1)
        mergeSortHelper(jobs, 0, size - 1, compare);
}

// quick sort
int JobArray::partition(int low, int high, bool (*cmp)(const Job &, const Job &))
{
    Job pivot = jobs[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (cmp(jobs[j], pivot))
        {
            i++;
            std::swap(jobs[i], jobs[j]);
        }
    }

    std::swap(jobs[i + 1], jobs[high]);
    return i + 1;
}

// quickSort helper
void JobArray::quickSortHelper(int low, int high, bool (*cmp)(const Job &, const Job &))
{
    if (low < high)
    {
        int pi = partition(low, high, cmp);
        quickSortHelper(low, pi - 1, cmp);
        quickSortHelper(pi + 1, high, cmp);
    }
}

void JobArray::quickSortById()
{
    auto cmp = [](const Job &a, const Job &b)
    { return a.id < b.id; };
    if (size > 1)
        quickSortHelper(0, size - 1, cmp);
}

void JobArray::quickSortByPosition()
{
    auto cmp = [](const Job &a, const Job &b)
    { return a.position < b.position; };
    if (size > 1)
        quickSortHelper(0, size - 1, cmp);
}

void JobArray::quickSortBySkill()
{
    auto cmp = [](const Job &a, const Job &b)
    { return a.skills[0] < b.skills[0]; };
    quickSortHelper(0, size - 1, cmp);
}

void JobArray::quickSortBySkillCount()
{
    auto cmp = [](const Job &a, const Job &b)
    { return a.skillCount < b.skillCount; };
    if (size > 1)
        quickSortHelper(0, size - 1, cmp);
}

void JobArray::quickSort(const std::string &criteria)
{
    if (size <= 1)
        return;

    if (criteria == "position")
    {
        quickSortByPosition();
    }
    else if (criteria == "skill")
    {
        quickSortBySkill();
    }
    else if (criteria == "skillCount")
    {
        quickSortBySkillCount();
    }
    else
    {
        cout << "Unknown sort criteria: " << criteria << endl;
    }
}

// binary search
JobArray *JobArray::binarySearchByPosition(const string &position)
{
    quickSortByPosition();
    JobArray *result = new JobArray();

    string target = normalizeString(position); // normalize input
    int left = 0, right = size - 1;
    int foundIndex = -1;

    while (left <= right)
    {
        int mid = (left + right) / 2;
        string midPos = normalizeString(jobs[mid].position);

        if (midPos == target)
        {
            foundIndex = mid;
            break;
        }
        else if (midPos < target)
            left = mid + 1;
        else
            right = mid - 1;
    }

    if (foundIndex == -1)
        return result;

    int i = foundIndex - 1;
    while (i >= 0 && normalizeString(jobs[i].position) == target)
    {
        result->addJob(jobs[i].id, jobs[i].position, jobs[i].skills, jobs[i].skillCount);
        i--;
    }

    i = foundIndex;
    while (i < size && normalizeString(jobs[i].position) == target)
    {
        result->addJob(jobs[i].id, jobs[i].position, jobs[i].skills, jobs[i].skillCount);
        i++;
    }

    return result;
}

JobArray *JobArray::binarySearchBySkills(const string *skillSet, int skillCount, bool matchAll)
{
    JobArray *result = new JobArray();

    for (int i = 0; i < size; i++)
    {
        sort(jobs[i].skills, jobs[i].skills + jobs[i].skillCount,
             [](const string &a, const string &b)
             { return normalizeString(a) < normalizeString(b); });

        int matches = 0;

        for (int k = 0; k < skillCount; k++)
        {
            string target = normalizeString(skillSet[k]);
            int left = 0, right = jobs[i].skillCount - 1;
            bool found = false;

            while (left <= right)
            {
                int mid = (left + right) / 2;
                string midSkill = normalizeString(jobs[i].skills[mid]);

                if (midSkill == target)
                {
                    found = true;
                    break;
                }
                else if (midSkill < target)
                    left = mid + 1;
                else
                    right = mid - 1;
            }

            if (found)
                matches++;
            else if (matchAll)
                break;
        }

        bool isMatch = matchAll ? (matches == skillCount) : (matches > 0);
        if (isMatch)
        {
            result->addJob(jobs[i].id, jobs[i].position, jobs[i].skills, jobs[i].skillCount);
        }
    }

    return result;
}
