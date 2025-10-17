#include "Resume.hpp"

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
    skills = nullptr;
}

ResumeNode::ResumeNode(Resume data)
{
    this->data = data;
    this->next = nullptr;
}