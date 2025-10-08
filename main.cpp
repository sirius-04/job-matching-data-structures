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

void createJobLinkedList(JobLinkedList &jobLinkedList)
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
    createJobLinkedList(jobLinkedList);

    jobLinkedList.printList();
    // resumelinkedList.printList();
}
