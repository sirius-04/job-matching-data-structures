#ifndef MATCH_RESULT_HPP
#define MATCH_RESULT_HPP

#include <iostream>
using namespace std;

class MatchResult {
public:
    int jobId;
    int resumeId;
    double score;

    MatchResult();
    MatchResult(int jobId, int resumeId, double score);
};

class MatchResultNode {
public:
    MatchResult data;
    MatchResultNode* next;

    MatchResultNode(MatchResult data);
};

class MatchResultList {
private:
    MatchResultNode* head;
    MatchResultNode* tail;
    int length;

public:
    MatchResultList();
    ~MatchResultList();

    void printSlice();
    void append(MatchResult data);
    int getLength();
};


#endif