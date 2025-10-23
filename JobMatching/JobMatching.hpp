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
    KEYWORD_BASED,
    WEIGHTED_SCORING
};

enum SearchAlgorithm
{
    LINEAR,
    BINARY
};

enum SearchTarget {
    SEARCH_JOB,
    SEARCH_RESUME
};

enum SortAlgorithm {
    MERGE,
    QUICK
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
    SortAlgorithm sortAlgo;

    MatchResultList *results;
    double matchTime;
    size_t memoryUsed;

    // unified search function
    void *searchBySkills(const string* skillSet, int skillCount, bool matchAll, void* dataSource, SearchTarget target);
    void *searchJobsByPosition(string position);

    template<typename Func>
    void processJobs(void* jobData, Func callback);
    template<typename Func>
    void processResumes(void* resumeData, Func callback);

    void* getAllResumes();
    void cleanupJobSearchResult(void* searchResult);
    void cleanupResumeSearchResult(void* searchResult);

    SkillWeightList *promptSkillWeights(const string *skillSet, int skillCount);
    double calculateWeightedScore(const string *inputSkills, int inputCount, const string *targetSkills, int targetCount, const SkillWeightList &weightList);
    void addUniqueMatch(int jobId, int resumeId, double score);

    // algorithms
    MatchResultList *keywordBasedMatch(const string *skillSet, int skillCount, bool matchAll);
    MatchResultList *weightedScoringMatch(const string *skillSet, int skillCount, bool matchAll);

public:
    JobMatching(JobArray *jobArray, ResumeArray *resumeArray);
    JobMatching(JobLinkedList *jobLinkedList, ResumeLinkedList *resumeLinkedList);
    JobMatching(JobCircularLinkedList *jobCircular, ResumeCircularLinkedList *resumeCircular);
    ~JobMatching();

    void setMatchMode(MatchMode matchMode);
    void setDataStruct(DataStruct dataStruct);
    void setMatchStrategy(MatchStrategy strategy);
    void setSearchAlgorithm(SearchAlgorithm searchAlgo);
    void setSortAlgorithm(SortAlgorithm sortAlgo);

    MatchResultList *runMatching(const string *skillSet, int skillCount, bool matchAll);
    void printPerformance() const;
};

#endif