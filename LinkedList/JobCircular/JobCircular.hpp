#ifndef JOB_CIRCULAR_HPP
#define JOB_CIRCULAR_HPP

#include <iostream>
#include "../../models/Job/Job.hpp"
using namespace std;

class JobNode
{
public:
    Job data;
    JobNode *next;

    JobNode(Job data);
};

class JobCircular
{
private:
    //

public:
    // constructor & destructor
    JobCircular();
    ~JobCircular();
};

#endif