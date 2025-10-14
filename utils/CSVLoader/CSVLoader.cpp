#include "CSVLoader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;

// Export functions with extern linkage
void createJobLinkedList(JobLinkedList &jobLinkedList)
{
    ifstream file("dataset/cleaned_jobDescription_v2.csv");
    if (!file.is_open())
    {
        cerr << "Error: Could not open file.\n";
        return;
    }

    string line;
    getline(file, line); // Skip header

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

        int id;
        try
        {
            id = stoi(idStr);
        }
        catch (...)
        {
            cerr << "Warning: Invalid ID \"" << idStr << "\", skipping.\n";
            continue;
        }

        string tempSkills[100];
        int count = 0;

        stringstream sk(skillStr);
        string skill;
        while (getline(sk, skill, ','))
        {
            skill = JobLinkedList::cleanString(skill);
            if (!skill.empty() && count < 100)
                tempSkills[count++] = skill;
        }

        // Create dynamic array of skills
        string *skills = new string[count];
        for (int i = 0; i < count; i++)
            skills[i] = tempSkills[i];

        // Construct Job object and append
        Job job(id, position, skills, count);
        jobLinkedList.append(job);

        delete[] skills; // cleanup temporary array
    }

    file.close();
}

void createResumeLinkedList(ResumeLinkedList &resumeLinkedList)
{
    ifstream file("dataset/cleaned_resume_v2.csv");
    if (!file.is_open())
    {
        cerr << "Error: Could not open file.\n";
        return;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line))
    {
        stringstream ss(line);
        string idStr, skillStr;

        getline(ss, idStr, ',');
        getline(ss, skillStr, '\n');

        idStr = ResumeLinkedList::cleanString(idStr);
        skillStr = ResumeLinkedList::cleanString(skillStr);

        int id;
        try
        {
            id = stoi(idStr);
        }
        catch (...)
        {
            cerr << "Warning: Invalid ID \"" << idStr << "\", skipping.\n";
            continue;
        }

        string tempSkills[100];
        int count = 0;

        stringstream sk(skillStr);
        string skill;
        while (getline(sk, skill, ','))
        {
            skill = ResumeLinkedList::cleanString(skill);
            if (!skill.empty() && count < 100)
                tempSkills[count++] = skill;
        }

        string *skills = new string[count];
        for (int i = 0; i < count; i++)
            skills[i] = tempSkills[i];

        Resume resume(id, skills, count);
        resumeLinkedList.append(resume);

        delete[] skills;
    }

    file.close();
}
