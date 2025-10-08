#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "../../LinkedList/JobLinkedList/JobLinkedList.hpp"
#include "../../LinkedList/ResumeLinkedList/ResumeLinkedList.hpp"
using namespace std;

// Export functions with extern linkage
void createJobLinkedList(JobLinkedList &jobLinkedList)
{
    ifstream file("dataset/cleaned_jobDescription.csv");

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

        jobLinkedList.append(id, position, skills, count);

        delete[] skills;
    }

    file.close();
}

void createResumeLinkedList(ResumeLinkedList &resumeLinkedList)
{
    ifstream file("dataset/cleaned_resume.csv");

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

        resumeLinkedList.append(id, skills, count);

        delete[] skills;
    }

    file.close();
}
