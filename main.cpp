#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "LinkedList/JobLinkedList/JobLinkedList.hpp"
#include "LinkedList/ResumeLinkedList/ResumeLinkedList.hpp"
using namespace std;

void runArray()
{
    cout << "array" << endl;
}

void runLinkedList()
{
    cout << "linkedlist: " << endl;
}

void summary()
{
    cout << "summary" << endl;
}

void displayMenu()
{
    int userChoice;

    while (true)
    {
        cout << "\nSelect operation: " << endl;
        cout << "[1] Array" << endl;
        cout << "[2] Linked List" << endl;
        cout << "[3] Summary" << endl;
        cout << "[4] Exit" << endl;
        cout << "Enter choice: ";

        if (!(cin >> userChoice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }

        switch (userChoice)
        {
        case 1:
            runArray();
            break;
        case 2:
            runLinkedList();
            break;
        case 3:
            summary();
            break;
        case 4:
            cout << "Exiting program..." << endl;
            return;
        default:
            cout << "Please enter a valid option!" << endl;
        }
    }
}

void createLinkedList(JobLinkedList &jobLinkedList)
{
    ifstream file("/Users/dede/Documents/APU/Degree Y1 S2/DSTR/job-matching-data-structures/dataset/cleaned_jobDescription.csv");

    if (!file.is_open())
    {
        cerr << "Error: Could not open file.\n";
        return;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line))
    {
        stringstream ss(line);
        string idStr, position, skillStr;

        getline(ss, idStr, ',');
        getline(ss, position, ',');
        getline(ss, skillStr, '\n');

        idStr = JobLinkedList::cleanString(idStr);
        position = JobLinkedList::cleanString(position);
        skillStr = JobLinkedList::cleanString(skillStr);

        int id = 0;
        try
        {
            id = stoi(idStr);
        }
        catch (...)
        {
            cerr << "Warning: invalid ID \"" << idStr << "\", skipping.\n";
            continue;
        }

        stringstream sk(skillStr);
        string skill;
        string tempSkills[100];
        int count = 0;

        while (getline(sk, skill, ','))
        {
            skill = JobLinkedList::cleanString(skill);
            if (!skill.empty())
                tempSkills[count++] = skill;
        }

        string *skills = new string[count];
        for (int i = 0; i < count; i++)
            skills[i] = tempSkills[i];

        jobLinkedList.insertLast(id, position, skills, count);

        delete[] skills;
    }

    file.close();
}

void phTest()
{
    JobLinkedList jobLinkedList;
    // cout << "\n===== Sorted by Skill Count =====\n";
    // jobLinkedList.quickSortBySkillCount();

    // cout << "\n===== Sorted by Position =====\n";
    // jobLinkedList.quickSortByPosition();

    // cout << "\n===== Sorted by First Skill (Alphabetical) =====\n";
    // jobLinkedList.quickSortBySkill();

    cout << "\n=== Binary Search by Position Position ===\n";
    JobLinkedList *result = jobLinkedList.binarySearchJobByPosition("software engineer");

    // cout << "\n=== Binary Search by Skill Set ===\n";
    // string skillSet[] = {"python", "mlops"};
    // // string skillSet[] = {"keras", "python", "mlops", "computer vision"};
    // JobLinkedList *result = jobLinkedList.binarySearchJobBySkills(skillSet, 2);

    if (result)
    {
        cout << "\nJobs matching all skills:\n";
        result->displaySlice();
        delete result; // free memory
    }
    else
    {
        cout << "\nNo jobs found with all given skills.\n";
    }
}

void createResumeLinkedList(ResumeLinkedList &resumeLinkedList)
{
    ifstream file("/Users/dede/Documents/APU/Degree Y1 S2/DSTR/job-matching-data-structures/dataset/cleaned_resume.csv");

    if (!file.is_open())
    {
        cerr << "Error: Could not open file.\n";
        return;
    }

    string line;
    getline(file, line); // skip header line

    while (getline(file, line))
    {
        stringstream ss(line);
        string idStr, skillStr;

        getline(ss, idStr, ',');
        getline(ss, skillStr, '\n');

        idStr = ResumeLinkedList::cleanString(idStr);
        skillStr = ResumeLinkedList::cleanString(skillStr);

        int id = 0;
        try
        {
            id = stoi(idStr);
        }
        catch (...)
        {
            cerr << "Warning: invalid ID \"" << idStr << "\", skipping.\n";
            continue;
        }

        // Parse skills (comma separated)
        stringstream sk(skillStr);
        string skill;
        string tempSkills[100];
        int count = 0;

        while (getline(sk, skill, ','))
        {
            skill = ResumeLinkedList::cleanString(skill);
            if (!skill.empty())
                tempSkills[count++] = skill;
        }

        string *skills = new string[count];
        for (int i = 0; i < count; i++)
            skills[i] = tempSkills[i];

        resumeLinkedList.insertLast(id, skills, count);

        delete[] skills;
    }

    file.close();
}

int main()
{
    JobLinkedList jobLinkedList;
    ResumeLinkedList resumelinkedList;

    createResumeLinkedList(resumelinkedList);
    createLinkedList(jobLinkedList);

    jobLinkedList.printList();
    // resumelinkedList.printList();
}
