#ifndef JOB_HPP
#define JOB_HPP

#include <iostream>
#include <string>
using namespace std;

class Job
{
public:
    int id;
    string position;
    string *skills;
    int skillCount;

    Job();
    Job(int id, string position, string *skills, int skillCount);
    Job(const Job &other);
    Job &operator=(const Job &other);
    ~Job();
};

#endif