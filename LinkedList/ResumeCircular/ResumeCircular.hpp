#ifndef RESUME_CIRCULAR_HPP
#define RESUME_CIRCULAR_HPP

#include <iostream>
#include "../../models/Resume/Resume.hpp"
using namespace std;

class ResumeNode
{
public:
    Resume data;
    ResumeNode *next;

    ResumeNode(Resume data);
};

class ResumeCircular
{
private:
    ResumeNode *head;
    ResumeNode *tail;
    int length;

public:
    // constructor & destructor
    ResumeCircular();
    ~ResumeCircular();
};

#endif