#ifndef JOB_MATCHING_HPP
#define JOB_MATCHING_HPP

#include "../LinkedList/JobLinkedList/JobLinkedList.hpp"
#include "../LinkedList/ResumeLinkedList/ResumeLinkedList.hpp"
#include "../CircularLinkedList/JobCircularLinkedList/JobCircularLinkedList.hpp"
#include "../CircularLinkedList/ResumeCircularLinkedList/ResumeCircularLinkedList.hpp"
#include "../Array/JobArray/JobArray.hpp"
#include "../Array/ResumeArray/ResumeArray.hpp"
#include "../models/MatchResult/MatchResult.hpp"
#include "../models/SkillWeight/SkillWeight.hpp"
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <chrono>
#include <cstdlib>
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
    JobLinkedList *jobLinkedList;
    JobCircularLinkedList *jobCircular;

    ResumeArray *resumeArray;
    ResumeLinkedList *resumeLinkedList;
    ResumeCircularLinkedList *resumeCircular;

    MatchMode matchMode;
    DataStruct dataStruct;
    MatchStrategy matchStrategy;
    SearchAlgorithm searchAlgo;

    MatchResultList *results;
    double matchTime;
    size_t memoryUsed;

    size_t getCurrentMemoryUsage();

public:
    JobMatching(JobArray *jobArray, ResumeArray *resumeArray);
    JobMatching(JobLinkedList *jobLinkedList, ResumeLinkedList *resumeLinkedList);
    JobMatching(JobCircularLinkedList *jobCircular, ResumeCircularLinkedList *resumeCircular);
    ~JobMatching();

    void setMatchMode(MatchMode matchMode);
    void setDataStruct(DataStruct dataStruct);
    void setMatchStrategy(MatchStrategy strategy);
    void setSearchAlgorithm(SearchAlgorithm searchAlgo);

    // unified search function
    void *search(const string *skillSet, int skillCount, bool matchAll);

    // algorithms
    MatchResultList* ruleBasedMatch(const string *skillSet, int skillCount, bool matchAll);

    SkillWeightList* promptSkillWeights(const string* skillSet, int skillCount);
    double calculateWeightedScore(const string* inputSkills, int inputCount, const string* targetSkills, int targetCount, const SkillWeightList& weightList);
    MatchResultList* weightedScoringMatch(const string *skillSet, int skillCount, bool matchAll);

    MatchResultList* runMatching(const string* skillSet, int skillCount, bool matchAll);
    void printPerformance() const;
};

#endif