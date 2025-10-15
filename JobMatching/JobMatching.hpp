#ifndef JOB_MATCHING_HPP
#define JOB_MATCHING_HPP

#include "../LinkedList/JobLinkedList/JobLinkedList.hpp"
#include "../LinkedList/ResumeLinkedList/ResumeLinkedList.hpp"
#include "../Array/JobArray/JobArray.hpp"
#include "../Array/ResumeArray/ResumeArray.hpp"
#include "../models/MatchResult/MatchResult.hpp"
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

enum MatchMode
{
    FIND_JOB,
    FIND_RESUME
};

enum DataStruct
{
    ARRAY,
    SINGLY_LINKED_LIST,
    CIRCULAR_LINKED_LIST
};

enum MatchStrategy
{
    RULE_BASED,
    WEIGHTED_SCORING
};

enum SearchAlgorithm
{
    LINEAR,
    BINARY
};

class JobMatching
{
private:
    JobArray *jobArray;
    ResumeArray *resumeArray;
    JobLinkedList *jobs;
    ResumeLinkedList *resumes;
    MatchResultList *results;

    MatchMode matchMode;
    DataStruct dataStruct;
    MatchStrategy matchStrategy;
    SearchAlgorithm searchAlgo;

    double matchTime;

public:
    JobMatching(JobArray *jobs, ResumeArray *resumes);
    JobMatching(JobLinkedList *jobs, ResumeLinkedList *resumes);
    ~JobMatching();

    void setMatchMode(MatchMode matchMode);
    void setDataStruct(DataStruct dataStruct);
    void setMatchStrategy(MatchStrategy strategy);
    void setSearchAlgorithm(SearchAlgorithm searchAlgo);

    auto search(const string *skillSet, int skillCount, bool matchAll) -> MatchResultList;

    double ruleBasedMatch(Job job, Resume resume);
    double weightedScoringMatch(Job job, Resume resume);

    void runMatching();
    void printPerformance();
    void printMatches();
};

#endif