#include "Job.hpp"

Job::Job()
{
    skills = new string[50];
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

// Copy constructor
Job::Job(const Job &other)
{
    id = other.id;
    position = other.position;
    skillCount = other.skillCount;
    skills = new string[skillCount];
    for (int i = 0; i < skillCount; i++)
        skills[i] = other.skills[i];
}

// Copy assignment
Job &Job::operator=(const Job &other)
{
    if (this != &other)
    {
        delete[] skills;
        id = other.id;
        position = other.position;
        skillCount = other.skillCount;
        skills = new string[skillCount];
        for (int i = 0; i < skillCount; i++)
            skills[i] = other.skills[i];
    }
    return *this;
}

Job::~Job()
{
    delete[] skills;
    skills = nullptr;
}