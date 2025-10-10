#ifndef RESUME_HPP
#define RESUME_HPP

#include <iostream>
#include <string>
using namespace std;

class Resume
{
public:
    int id;
    string *skills;
    int skillCount;

    Resume();
    Resume(int id, string *skills, int skillCount);
    ~Resume();

    Resume(const Resume &other);
    Resume &operator=(const Resume &other);
};

#endif