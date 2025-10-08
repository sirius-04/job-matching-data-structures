#ifndef JOB_MATCHING_HPP
#define JOB_MATCHING_HPP

#include "../LinkedList/JobLinkedList/JobLinkedList.hpp"
#include "../LinkedList/ResumeLinkedList/ResumeLinkedList.hpp"
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

enum SearchAlgorithm {
    LINEAR,
    BINARY
};

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

    void printList();
    void append(int jobId, int resumeId, double score);
    int getLength();
};

class JobMatching {
private:
    JobLinkedList* jobs;
    ResumeLinkedList* resumes;
    MatchResultList* results;
    SearchAlgorithm selectedAlgorithm;
    double matchTime;

    double calculateSkillMatch(JobNode* job, ResumeNode* resume);
    double calculateFinalScore(JobNode* job, ResumeNode* resume);

    void matchResumesUsingLinear(JobNode* job);
    void matchResumesUsingBinary(JobNode* job);

    void matchJobsUsingLinear(ResumeNode* resume);
    void matchJobsUsingBinary(ResumeNode* resume);

public:
    JobMatching(JobLinkedList* jobs, ResumeLinkedList* resumes, SearchAlgorithm selectedAlgorithm);
    ~JobMatching();

    void setAlgorithm(SearchAlgorithm selectedAlgorithm);
    void runMatching();
    void printPerformance();
    void printMatches();
};

#endif