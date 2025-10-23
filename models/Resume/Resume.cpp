#include "Resume.hpp"

Resume::Resume()
{
    id = 0;
    skills = nullptr;
    skillCount = 0;
}

Resume::Resume(int id, string *skills, int skillCount)
{
    this->id = id;
    this->skillCount = skillCount;
    
    if (skills && skillCount > 0) {
        this->skills = new string[skillCount];
        for (int i = 0; i < skillCount; i++)
        {
            this->skills[i] = skills[i];
        }
    } else {
        this->skills = nullptr;
    }
}

Resume::Resume(const Resume &other)
{
    id = other.id;
    skillCount = other.skillCount;
    
    if (other.skills && other.skillCount > 0) {
        skills = new string[skillCount];
        for (int i = 0; i < skillCount; i++)
            skills[i] = other.skills[i];
    } else {
        skills = nullptr;
    }
}

Resume &Resume::operator=(const Resume &other)
{
    if (this != &other)
    {
        delete[] skills;
        
        id = other.id;
        skillCount = other.skillCount;
        
        if (other.skills && other.skillCount > 0) {
            skills = new string[skillCount];
            for (int i = 0; i < skillCount; i++)
                skills[i] = other.skills[i];
        } else {
            skills = nullptr;
        }
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