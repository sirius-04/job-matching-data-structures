#include "ResumeArray.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

// normalize string
inline std::string normalizeString(std::string s)
{
    // Trim whitespace
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    // Convert to lowercase
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

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
        newResumes[i] = resumes[i];
    }

    delete[] resumes;
    
    resumes = newResumes;
}

int ResumeArray::getSize() const
{
    return size;
}

Resume* ResumeArray::findById(int id) {
    for (int i = 0; i < size; i++) {
        if (resumes[i].id == id) {
            return &resumes[i];
        }
    }
    return nullptr;
}

Resume ResumeArray::getResume(int index) const
{
    if (index >= 0 && index < size)
        return resumes[index];
    throw out_of_range("Index out of bounds in getResume()");
}

void ResumeArray::addResume(int id, string *skills, int skillCount)
{
    if (size >= capacity)
        resize();

    resumes[size].id = id;
    resumes[size].skillCount = skillCount;
    
    if (skills && skillCount > 0) {
        resumes[size].skills = new string[skillCount];
        for (int i = 0; i < skillCount; i++)
        {
            resumes[size].skills[i] = skills[i];
        }
    } else {
        resumes[size].skills = nullptr;
    }

    size++;
}

void ResumeArray::printResumes()
{
    cout << "\n--- Resume for Array ---\n";
    if (size <= 20)
    {
        for (int i = 0; i < size; i++)
        {
            cout << resumes[i].id << " | ";
            for (int j = 0; j < resumes[i].skillCount; j++)
            {
                cout << resumes[i].skills[j];
                if (j < resumes[i].skillCount - 1)
                    cout << ",";
            }
            cout << " | Total Skills: " << resumes[i].skillCount << endl;
        }
    }
    else
    {
        // Print first 10
        for (int i = 0; i < 10; i++)
        {
            cout << resumes[i].id << " | ";
            for (int j = 0; j < resumes[i].skillCount; j++)
            {
                cout << resumes[i].skills[j];
                if (j < resumes[i].skillCount - 1)
                    cout << ",";
            }
            cout << " | Total Skills: " << resumes[i].skillCount << endl;
        }

        cout << "...\n... (skipping " << (size - 20) << " resumes) ...\n...\n";

        // Print last 10
        for (int i = size - 10; i < size; i++)
        {
            cout << resumes[i].id << " | ";
            for (int j = 0; j < resumes[i].skillCount; j++)
            {
                cout << resumes[i].skills[j];
                if (j < resumes[i].skillCount - 1)
                    cout << ",";
            }
            cout << " | Total Skills: " << resumes[i].skillCount << endl;
        }
    }

    cout << "\nTotal resumes printed: " << (size <= 20 ? size : 20) << " out of " << size << endl;
}

// ======================= Linear Search =======================

ResumeArray *ResumeArray::linearSearchBySkills(const string *skillSet, int skillCount, bool matchAll)
{
    ResumeArray *result = new ResumeArray();

    for (int i = 0; i < size; i++)
    {
        int matchCount = 0;

        for (int k = 0; k < skillCount; k++)
        {
            string target = normalizeString(skillSet[k]);
            bool found = false;

            for (int j = 0; j < resumes[i].skillCount; j++)
            {
                string resumeSkill = normalizeString(resumes[i].skills[j]);

                if (resumeSkill == target)
                {
                    found = true;
                    matchCount++;
                    break;
                }
            }

            if (matchAll && !found)
                break;
        }

        bool isMatch = matchAll ? (matchCount == skillCount) : (matchCount > 0);

        if (isMatch)
        {
            result->addResume(resumes[i].id, resumes[i].skills, resumes[i].skillCount);
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
    const Resume& pivot = resumes[high];
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
        if (a.skillCount == 0)
            return true;
        if (b.skillCount == 0)
            return false;
        return a.skills[0] < b.skills[0];
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

void ResumeArray::quickSort(const std::string &criteria)
{
    if (size <= 1)
        return;

    if (criteria == "id")
    {
        quickSortById();
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

// Merge Sort for skills array
void ResumeArray::mergeSortSkills(string *skills, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortSkills(skills, left, mid);
        mergeSortSkills(skills, mid + 1, right);
        mergeSkills(skills, left, mid, right);
    }
}

void ResumeArray::mergeSkills(string *skills, int left, int mid, int right) {
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
        if (normalizeString(L[i]) <= normalizeString(R[j])) {
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
void ResumeArray::quickSortSkills(string *skills, int low, int high) {
    if (low < high) {
        int pi = partitionSkills(skills, low, high);
        quickSortSkills(skills, low, pi - 1);
        quickSortSkills(skills, pi + 1, high);
    }
}

int ResumeArray::partitionSkills(string *skills, int low, int high) {
    string pivot = normalizeString(skills[high]);
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (normalizeString(skills[j]) < pivot) {
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

// binary search
ResumeArray *ResumeArray::binarySearchBySkills(const string *skillSet, int skillCount, bool matchAll, SortAlgorithm sortAlgo)
{
    ResumeArray *result = new ResumeArray();

    for (int i = 0; i < size; i++)
    {
        // Sort each resume's skills array using the specified algorithm
        if (sortAlgo == MERGE) {
            ResumeArray::mergeSortSkills(resumes[i].skills, 0, resumes[i].skillCount - 1);
        } else {
            ResumeArray::quickSortSkills(resumes[i].skills, 0, resumes[i].skillCount - 1);
        }

        int matches = 0;

        for (int k = 0; k < skillCount; k++)
        {
            string target = normalizeString(skillSet[k]);
            int left = 0, right = resumes[i].skillCount - 1;
            bool found = false;

            while (left <= right)
            {
                int mid = (left + right) / 2;
                string midSkill = normalizeString(resumes[i].skills[mid]);

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
            result->addResume(resumes[i].id, resumes[i].skills, resumes[i].skillCount);
        }
    }

    return result;
}
