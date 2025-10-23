#include "Job.hpp"

Job::Job()
{
    id = 0;
    position = "";
    skills = nullptr;
    skillCount = 0;
}

Job::Job(int id, string position, string *skills, int skillCount)
{
    this->id = id;
    this->position = position;
    this->skillCount = skillCount;
    this->skills = new string[skillCount];
    for (int i = 0; i < skillCount; i++)
        this->skills[i] = skills[i];
}

Job::Job(const Job &other)
{
    id = other.id;
    position = other.position;
    skillCount = other.skillCount;
    
    if (other.skills && other.skillCount > 0) {
        skills = new string[skillCount];
        for (int i = 0; i < skillCount; i++)
            skills[i] = other.skills[i];
    } else {
        skills = nullptr;
    }
}

Job &Job::operator=(const Job &other)
{
    if (this != &other)
    {
        delete[] skills;
        
        id = other.id;
        position = other.position;
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

Job::~Job()
{
    delete[] skills;
    skills = nullptr;
}

JobNode::JobNode(Job data)
{
    this->data = data;
    this->next = nullptr;
}