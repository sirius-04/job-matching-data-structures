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

void createJobCircularLinkedList(JobCircularLinkedList &jobCircularLinkedList)
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

        idStr = JobCircularLinkedList::cleanString(idStr);
        position = JobCircularLinkedList::cleanString(position);
        skillStr = JobCircularLinkedList::cleanString(skillStr);

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
            skill = JobCircularLinkedList::cleanString(skill);
            if (!skill.empty() && count < 100)
                tempSkills[count++] = skill;
        }

        // Create dynamic array of skills
        string *skills = new string[count];
        for (int i = 0; i < count; i++)
            skills[i] = tempSkills[i];

        // Construct Job object and append
        Job job(id, position, skills, count);
        jobCircularLinkedList.append(job);

        delete[] skills; // cleanup temporary array
    }

    file.close();
}

void createResumeCircularLinkedList(ResumeCircularLinkedList &resumeCircularLinkedList)
{
    ifstream file("dataset/cleaned_resume_v2.csv");
    if (!file.is_open())
    {
        cerr << "Error: Could not open file.\n";
        return;
    }

    string line;
    getline(file, line); // Skip header line

    while (getline(file, line))
    {
        stringstream ss(line);
        string idStr, skillStr;

        // Split columns
        getline(ss, idStr, ',');
        getline(ss, skillStr, '\n');

        // Clean the strings
        idStr = ResumeCircularLinkedList::cleanString(idStr);
        skillStr = ResumeCircularLinkedList::cleanString(skillStr);

        // Convert ID
void createJobArray(JobArray &jobArray)
{
    ifstream file("dataset/cleaned_jobDescription_v2.csv");
    if (!file.is_open())
    {
        cerr << "Failed to open file!" << endl;
        return;
    }

    cout << "File opened successfully!" << endl;

    string line;
    getline(file, line); // skip header

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string idStr, position, skillsStr;

        getline(ss, idStr, ',');
        getline(ss, position, ',');
        getline(ss, skillsStr, '\n');

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

        // Split skills
        string tempSkills[100];
        int count = 0;

        stringstream sk(skillStr);
        string skill;
        while (getline(sk, skill, ','))
        {
            skill = ResumeCircularLinkedList::cleanString(skill);
            if (!skill.empty() && count < 100)
                tempSkills[count++] = skill;
        }

        // Create dynamic array
        string *skills = new string[count];
        for (int i = 0; i < count; i++)
            skills[i] = tempSkills[i];

        // Create Resume object and append to circular linked list
        Resume resume(id, skills, count);
        resumeCircularLinkedList.append(resume);

        delete[] skills; // Cleanup
    }

    file.close();
            cerr << "Invalid ID: " << idStr << endl;
            continue;
        }

        if (!skillsStr.empty() && skillsStr.front() == '"')
            skillsStr.erase(0, 1);
        if (!skillsStr.empty() && skillsStr.back() == '"')
            skillsStr.pop_back();

        int skillCount = 1;
        for (char ch : skillsStr)
            if (ch == ',')
                skillCount++;

        string *skillsArr = new string[skillCount];

        int index = 0;
        size_t start = 0, end;
        while ((end = skillsStr.find(',', start)) != string::npos)
        {
            skillsArr[index++] = skillsStr.substr(start, end - start);
            start = end + 1;
        }
        skillsArr[index++] = skillsStr.substr(start); // last skill

        jobArray.addJob(id, position, skillsArr, skillCount);
        delete[] skillsArr;
    }

    file.close();
    return;
}

void createResumeArray(ResumeArray &resumeArray)
{
    ifstream file("dataset/cleaned_resume_v2.csv");
    if (!file.is_open())
    {
        cerr << "Failed to open file!" << endl;
        return;
    }

    cout << "File opened successfully!" << endl;

    string line;
    getline(file, line); // skip header

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string idStr, skillsStr;

        getline(ss, idStr, ',');
        getline(ss, skillsStr, '\n');

        int id;
        try
        {
            id = stoi(idStr);
        }
        catch (...)
        {
            cerr << "Invalid ID: " << idStr << endl;
            continue;
        }

        if (!skillsStr.empty() && skillsStr.front() == '"')
            skillsStr.erase(0, 1);
        if (!skillsStr.empty() && skillsStr.back() == '"')
            skillsStr.pop_back();

        int skillCount = 1;
        for (char ch : skillsStr)
            if (ch == ',')
                skillCount++;

        string *skillsArr = new string[skillCount];

        int index = 0;
        size_t start = 0, end;
        while ((end = skillsStr.find(',', start)) != string::npos)
        {
            skillsArr[index++] = skillsStr.substr(start, end - start);
            start = end + 1;
        }
        skillsArr[index++] = skillsStr.substr(start); // last skill

        resumeArray.addResume(id, skillsArr, skillCount);
        delete[] skillsArr;
    }

    file.close();
    return;
}
