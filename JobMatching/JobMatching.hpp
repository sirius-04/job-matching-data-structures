#ifndef JOB_MATCHING_HPP
#define JOB_MATCHING_HPP

#include "../LinkedList/JobLinkedList/JobLinkedList.hpp"
#include "../LinkedList/ResumeLinkedList/ResumeLinkedList.hpp"
#include "../models/MatchResult/MatchResult.hpp"
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

enum MatchMode {
    FIND_JOB,
    FIND_RESUME
};

enum DataStruct {
    ARRAY,
    SINGLY_LINKED_LIST,
    CIRCULAR_LINKED_LIST
};

enum MatchStrategy {
    RULE_BASED,
    WEIGHTED
};

enum SearchAlgorithm {
    LINEAR,
    BINARY
};

enum SortAlgorithm {
    MERGE,
    QUICK
};

class JobMatching {
private:
    JobLinkedList* jobs;
    ResumeLinkedList* resumes;
    MatchResultList* results;

    MatchMode mode;
    MatchStrategy matchStrategy;
    SearchAlgorithm searchAlgo;
    SortAlgorithm sortAlgo;

    double matchTime;

public:
    JobMatching(JobLinkedList* jobs, ResumeLinkedList* resumes);
    ~JobMatching();

    void setMatchMode(MatchMode mode);
    void setDataStruct(DataStruct dataStruct);
    void setMatchingStrategy(MatchStrategy strategy);
    void setSearchAlgorithm(SearchAlgorithm searchAlgo);
    void setSortAlgorithm(SortAlgorithm sortAlgo);

    double ruleBasedMatch(JobNode* job, ResumeNode* resume);
    double weightedMatch(JobNode* job, ResumeNode* resume);

    void runMatching();
    void printPerformance();
    void printMatches();
};

#endif