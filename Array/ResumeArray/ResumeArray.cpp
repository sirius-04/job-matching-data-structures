#include "ResumeArray.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
using namespace std;

// ======================= Resume Implementation =======================

Resume::Resume()
{
    skills = new string[50];
    skillCount = 0;
}

Resume::Resume(int id, string *skills, int skillCount)
{
    this->id = id;
    this->skillCount = skillCount;
    this->skills = new string[skillCount];
    for (int i = 0; i < skillCount; i++)
    {
        this->skills[i] = skills[i];
    }
}

Resume::Resume(const Resume &other)
{
    id = other.id;
    skillCount = other.skillCount;
    skills = new string[skillCount];
    for (int i = 0; i < skillCount; i++)
        skills[i] = other.skills[i];
}

Resume &Resume::operator=(const Resume &other)
{
    if (this != &other)
    {
        delete[] skills;
        id = other.id;
        skillCount = other.skillCount;
        skills = new string[skillCount];
        for (int i = 0; i < skillCount; i++)
            skills[i] = other.skills[i];
    }
    return *this;
}

Resume::~Resume()
{
    delete[] skills;
}

// ======================= ResumeArray Implementation =======================

ResumeArray::ResumeArray()
{
    capacity = 50;
    size = 0;
    resumes = new Resume[capacity];
}

ResumeArray::~ResumeArray()
{
    delete[] resumes;
}

void ResumeArray::resize()
{
    capacity *= 2;
    Resume *newResumes = new Resume[capacity];

    for (int i = 0; i < size; i++)
    {
        newResumes[i].id = resumes[i].id;
        newResumes[i].skillCount = resumes[i].skillCount;
        newResumes[i].skills = new string[resumes[i].skillCount];
        for (int j = 0; j < resumes[i].skillCount; j++)
        {
            newResumes[i].skills[j] = resumes[i].skills[j];
        }
    }

    delete[] resumes;
    resumes = newResumes;
}

bool ResumeArray::loadFromCSV(const string &filename)
{
    cout << "=== Running Array Mode ===" << endl;
    cout << "Attempting to open: " << filename << endl;

    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "❌ Failed to open " << filename << endl;
        return false;
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

        size_t commaPos = line.find(',');
        if (commaPos == string::npos)
            continue;

        string idStr = line.substr(0, commaPos);
        string skillStr = line.substr(commaPos + 1);

        if (!skillStr.empty() && skillStr.front() == '"')
            skillStr.erase(0, 1);
        if (!skillStr.empty() && skillStr.back() == '"')
            skillStr.pop_back();

        int id = 0;
        try
        {
            id = stoi(idStr);
        }
        catch (...)
        {
            cerr << "⚠️ Invalid ID on line " << lineCount << ": " << idStr << endl;
            continue;
        }

        vector<string> skillsVec;
        stringstream skillSS(skillStr);
        string skill;
        while (getline(skillSS, skill, ','))
        {
            if (!skill.empty())
                skillsVec.push_back(skill);
        }

        int count = skillsVec.size();
        string *skillsArr = new string[count];
        for (int i = 0; i < count; i++)
            skillsArr[i] = skillsVec[i];

        addResume(id, skillsArr, count);
        delete[] skillsArr;
    }

    cout << "✅ Loaded total resumes: " << size << endl;
    file.close();
    return true;
}

void ResumeArray::addResume(int id, string *skills, int skillCount)
{
    if (size >= capacity)
        resize();

    resumes[size].id = id;
    resumes[size].skillCount = skillCount;
    resumes[size].skills = new string[skillCount];

    for (int i = 0; i < skillCount; i++)
    {
        resumes[size].skills[i] = skills[i];
    }

    size++;
}

void ResumeArray::printResumes()
{
    cout << "\n--- Resume Data ---\n";
    for (int i = 0; i < size; i++)
    {
        cout << resumes[i].id << " | ";
        for (int j = 0; j < resumes[i].skillCount; j++)
        {
            cout << resumes[i].skills[j];
            if (j < resumes[i].skillCount - 1)
                cout << ",";
        }
        cout << endl;
    }
    cout << "\n✅ Total resumes printed: " << size << endl;
}

// ======================= Linear Search =======================

ResumeArray ResumeArray::linearSearchBySkill(const string &skill)
{
    ResumeArray result;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < resumes[i].skillCount; j++)
        {
            if (resumes[i].skills[j] == skill)
            {
                result.addResume(resumes[i].id, resumes[i].skills, resumes[i].skillCount);
                break;
            }
        }
    }
    return result;
}

// ======================= Merge Sort =======================

bool ResumeArray::compareById(const Resume &a, const Resume &b)
{
    return a.id < b.id;
}

bool ResumeArray::compareBySkillCount(const Resume &a, const Resume &b)
{
    return a.skillCount < b.skillCount;
}

// new: sort alphabetically by first skill
bool ResumeArray::compareByFirstSkill(const Resume &a, const Resume &b)
{
    if (a.skillCount == 0)
        return true;
    if (b.skillCount == 0)
        return false;
    return a.skills[0] < b.skills[0];
}

void ResumeArray::merge(Resume *arr, int left, int mid, int right, CompareFn compare)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Resume *L = new Resume[n1];
    Resume *R = new Resume[n2];

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

void ResumeArray::mergeSortHelper(Resume *arr, int left, int right, CompareFn compare)
{
    if (left < right)
    {
        int mid = (left + right) / 2;
        mergeSortHelper(arr, left, mid, compare);
        mergeSortHelper(arr, mid + 1, right, compare);
        merge(arr, left, mid, right, compare);
    }
}

void ResumeArray::mergeSort(CompareFn compare)
{
    if (size > 1)
        mergeSortHelper(resumes, 0, size - 1, compare);
}

// quick sort
int ResumeArray::partition(int low, int high, bool (*cmp)(const Resume &, const Resume &))
{
    Resume pivot = resumes[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (cmp(resumes[j], pivot))
        {
            i++;
            swap(resumes[i], resumes[j]);
        }
    }

    swap(resumes[i + 1], resumes[high]);
    return i + 1;
}

// quickSort helper
void ResumeArray::quickSortHelper(int low, int high, bool (*cmp)(const Resume &, const Resume &))
{
    if (low < high)
    {
        int pi = partition(low, high, cmp);
        quickSortHelper(low, pi - 1, cmp);
        quickSortHelper(pi + 1, high, cmp);
    }
}

void ResumeArray::quickSortById()
{
    auto cmp = [](const Resume &a, const Resume &b)
    { return a.id < b.id; };
    if (size > 1)
        quickSortHelper(0, size - 1, cmp);
}

void ResumeArray::quickSortBySkill()
{
    auto cmp = [](const Resume &a, const Resume &b)
    {
        string skillA = (a.skillCount > 0) ? a.skills[0] : "";
        string skillB = (b.skillCount > 0) ? b.skills[0] : "";
        return skillA < skillB;
    };
    if (size > 1)
        quickSortHelper(0, size - 1, cmp);
}

void ResumeArray::quickSortBySkillCount()
{
    auto cmp = [](const Resume &a, const Resume &b)
    { return a.skillCount < b.skillCount; };
    if (size > 1)
        quickSortHelper(0, size - 1, cmp);
}

// binary search
ResumeArray ResumeArray::binarySearchBySkills(const string &skill)
{
    ResumeArray result;

    int left = 0, right = size - 1;
    int foundIndex = -1;

    while (left <= right)
    {
        int mid = (left + right) / 2;

        if (resumes[mid].skillCount == 0)
        {
            left = mid + 1;
            continue;
        }

        string firstSkill = resumes[mid].skills[0];

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
        return result;

    int i = foundIndex;
    while (i >= 0 && resumes[i].skillCount > 0 && resumes[i].skills[0] == skill)
    {
        result.addResume(resumes[i].id, resumes[i].skills, resumes[i].skillCount);
        i--;
    }

    i = foundIndex + 1;
    while (i < size && resumes[i].skillCount > 0 && resumes[i].skills[0] == skill)
    {
        result.addResume(resumes[i].id, resumes[i].skills, resumes[i].skillCount);
        i++;
    }

    return result;
}
