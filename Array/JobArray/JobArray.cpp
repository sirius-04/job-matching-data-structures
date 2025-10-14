#include "JobArray.hpp"
#include <fstream>
#include <sstream>
#include <vector>

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
        newJobs[i] = jobs[i]; // safe with copy constructor
    delete[] jobs;
    jobs = newJobs;
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

bool JobArray::loadFromCSV(const string &filename)
{
    ifstream file(filename);
    cout << "Attempting to open: " << filename << endl;

    if (!file.is_open())
    {
        cerr << "❌ Failed to open file!" << endl;
        return false;
    }

    file.rdbuf()->pubsetbuf(nullptr, 0);

    // Handle UTF-8 BOM
    char c1 = file.peek();
    if (c1 == '\xEF')
    {
        file.get();
        file.get();
        file.get();
    }

    cout << "✅ File opened successfully!" << endl;

    string line;
    getline(file, line); // skip header

    int lineCount = 0;
    while (getline(file, line))
    {
        if (line.empty())
            continue;
        lineCount++;

        stringstream ss(line);
        string idStr, position, skillsStr;

        getline(ss, idStr, ',');
        getline(ss, position, ',');
        getline(ss, skillsStr);

        int id = 0;
        try
        {
            id = stoi(idStr);
        }
        catch (...)
        {
            cerr << "Invalid ID on line " << lineCount << ": " << idStr << endl;
            continue;
        }

        if (!skillsStr.empty() && skillsStr.front() == '"')
            skillsStr.erase(0, 1);
        if (!skillsStr.empty() && skillsStr.back() == '"')
            skillsStr.pop_back();

        vector<string> skillVec;
        string skill;
        stringstream skillSS(skillsStr);
        while (getline(skillSS, skill, ','))
        {
            if (!skill.empty())
                skillVec.push_back(skill);
        }

        int skillCount = skillVec.size();
        string *skillsArr = new string[skillCount];
        for (int i = 0; i < skillCount; i++)
            skillsArr[i] = skillVec[i];

        addJob(id, position, skillsArr, skillCount);
        delete[] skillsArr;
    }

    cout << "✅ Loaded " << size << " jobs\n";
    file.close();
    return true;
}

void JobArray::printJobs()
{
    cout << "\n--- Job Descriptions ---\n";
    for (int i = 0; i < size; i++)
    {
        cout << jobs[i].id << " | " << jobs[i].position << " | ";
        for (int j = 0; j < jobs[i].skillCount; j++)
        {
            cout << jobs[i].skills[j];
            if (j < jobs[i].skillCount - 1)
                cout << ",";
        }
        cout << endl;
    }
}

// ======================= Linear Search =======================
JobArray JobArray::linearSearchJobBySkills(const string *skillSet, int skillCount, bool matchAll)
{
    JobArray result;

    for (int i = 0; i < size; i++)
    {
        int matchedSkills = 0;

        for (int j = 0; j < skillCount; j++)
        {
            for (int k = 0; k < jobs[i].skillCount; k++)
            {
                if (jobs[i].skills[k] == skillSet[j])
                {
                    matchedSkills++;
                    break;
                }
            }
        }

        // ✅ if matchAll = true, all skills must match
        // ✅ if matchAll = false, at least one must match
        bool addThis = matchAll ? (matchedSkills == skillCount) : (matchedSkills > 0);

        if (addThis)
        {
            result.addJob(jobs[i].id, jobs[i].position, jobs[i].skills, jobs[i].skillCount);
        }
    }

    return result;
}

JobArray JobArray::linearSearchByPosition(const string &position)
{
    JobArray result;
    for (int i = 0; i < size; i++)
    {
        if (jobs[i].position == position)
        {
            result.addJob(jobs[i].id, jobs[i].position, jobs[i].skills, jobs[i].skillCount);
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
    {
        string skillA = (a.skillCount > 0) ? a.skills[0] : "";
        string skillB = (b.skillCount > 0) ? b.skills[0] : "";
        return skillA < skillB;
    };
    if (size > 1)
        quickSortHelper(0, size - 1, cmp);
}

void JobArray::quickSortBySkillCount()
{
    auto cmp = [](const Job &a, const Job &b)
    { return a.skillCount < b.skillCount; };
    if (size > 1)
        quickSortHelper(0, size - 1, cmp);
}

// binary search
JobArray JobArray::binarySearchByPosition(const string &position)
{
    JobArray result;

    int left = 0, right = size - 1;
    int foundIndex = -1;

    while (left <= right)
    {
        int mid = (left + right) / 2;

        if (jobs[mid].position == position)
        {
            foundIndex = mid;
            break;
        }
        else if (jobs[mid].position < position)
            left = mid + 1;
        else
            right = mid - 1;
    }

    if (foundIndex == -1)
        return result;

    int i = foundIndex;
    while (i >= 0 && jobs[i].position == position)
    {
        result.addJob(jobs[i].id, jobs[i].position, jobs[i].skills, jobs[i].skillCount);
        i--;
    }

    i = foundIndex + 1;
    while (i < size && jobs[i].position == position)
    {
        result.addJob(jobs[i].id, jobs[i].position, jobs[i].skills, jobs[i].skillCount);
        i++;
    }

    return result;
}

JobArray JobArray::binarySearchJobBySkills(const string *skillSet, int skillCount)
{
    JobArray result;

    for (int k = 0; k < skillCount; k++)
    {
        string skill = skillSet[k];
        int left = 0, right = size - 1;
        int foundIndex = -1;

        while (left <= right)
        {
            int mid = (left + right) / 2;

            if (jobs[mid].skillCount == 0)
            {
                left = mid + 1;
                continue;
            }

            string firstSkill = jobs[mid].skills[0];

            if (firstSkill == skill)
            {
                foundIndex = mid;
                break;
            }
            else if (firstSkill < skill)
                left = mid + 1;
            else
                right = mid - 1;
        }

        if (foundIndex == -1)
            continue;

        int i = foundIndex;
        while (i >= 0 && jobs[i].skillCount > 0 && jobs[i].skills[0] == skill)
        {
            result.addJob(jobs[i].id, jobs[i].position, jobs[i].skills, jobs[i].skillCount);
            i--;
        }

        i = foundIndex + 1;
        while (i < size && jobs[i].skillCount > 0 && jobs[i].skills[0] == skill)
        {
            result.addJob(jobs[i].id, jobs[i].position, jobs[i].skills, jobs[i].skillCount);
            i++;
        }
    }

    return result;
}
