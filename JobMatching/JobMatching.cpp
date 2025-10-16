#include "JobMatching.hpp"

#include "JobMatching.hpp"

JobMatching::JobMatching(JobArray* jobArray, ResumeArray* resumeArray) {
    this->jobArray = jobArray;
    this->resumeArray = resumeArray;
    this->jobLinkedList = nullptr;
    this->resumeLinkedList = nullptr;
    this->jobCircular = nullptr;
    this->resumeCircular = nullptr;

    this->results = new MatchResultList();
    this->matchTime = 0;
    this->matchMode = FIND_RESUME;
    this->dataStruct = ARRAY;
    this->matchStrategy = RULE_BASED;
    this->searchAlgo = LINEAR;
}

JobMatching::JobMatching(JobLinkedList* jobLinkedList, ResumeLinkedList* resumeLinkedList) {
    this->jobArray = nullptr;
    this->resumeArray = nullptr;
    this->jobLinkedList = jobLinkedList;
    this->resumeLinkedList = resumeLinkedList;
    this->jobCircular = nullptr;
    this->resumeCircular = nullptr;

    this->results = new MatchResultList();
    this->matchTime = 0;
    this->matchMode = FIND_RESUME;
    this->dataStruct = SINGLY_LINKED_LIST;
    this->matchStrategy = RULE_BASED;
    this->searchAlgo = LINEAR;
}

JobMatching::JobMatching(JobCircular* jobCircular, ResumeCircular* resumeCircular) {
    this->jobArray = nullptr;
    this->resumeArray = nullptr;
    this->jobLinkedList = nullptr;
    this->resumeLinkedList = nullptr;
    this->jobCircular = jobCircular;
    this->resumeCircular = resumeCircular;

    this->results = new MatchResultList();
    this->matchTime = 0;
    this->matchMode = FIND_RESUME;
    this->dataStruct = CIRCULAR_LINKED_LIST;
    this->matchStrategy = RULE_BASED;
    this->searchAlgo = LINEAR;
}

JobMatching::~JobMatching() {
    delete results;
}

void JobMatching::setMatchMode(MatchMode matchMode) {
    this->matchMode = matchMode;
}

void JobMatching::setDataStruct(DataStruct dataStruct) {
    this->dataStruct = dataStruct;
}

void JobMatching::setMatchStrategy(MatchStrategy matchStrategy) {
    this->matchStrategy = matchStrategy;
}

void JobMatching::setSearchAlgorithm(SearchAlgorithm searchAlgo) {
    this->searchAlgo = searchAlgo;
}

void* JobMatching::search(const string* skillSet, int skillCount, bool matchAll) {
    if (matchMode == FIND_RESUME) {
        switch (dataStruct) {
            case ARRAY:
                return (searchAlgo == LINEAR)
                    ? (void*)resumeArray->linearSearchBySkills(skillSet, skillCount, matchAll)
                    : (void*)resumeArray->binarySearchBySkills(skillSet, skillCount, matchAll);

            case SINGLY_LINKED_LIST:
                return (searchAlgo == LINEAR)
                    ? (void*)resumeLinkedList->linearSearchResumeBySkills(skillSet, skillCount, matchAll)
                    : (void*)resumeLinkedList->binarySearchResumeBySkills(skillSet, skillCount, matchAll);

            case CIRCULAR_LINKED_LIST:
                return (searchAlgo == LINEAR)
                    ? (void*)resumeCircular->linearSearchResumeBySkills(skillSet, skillCount, matchAll)
                    : (void*)resumeCircular->binarySearchResumeBySkills(skillSet, skillCount, matchAll);

            default:
                cerr << "❌ Unknown data structure for resume search!" << endl;
                return nullptr;
        }
    }

    else if (matchMode == FIND_JOB) {
        switch (dataStruct) {
            case ARRAY:
                return (searchAlgo == LINEAR)
                    ? (void*)jobArray->linearSearchBySkills(skillSet, skillCount, matchAll)
                    : (void*)jobArray->binarySearchBySkills(skillSet, skillCount, matchAll);

            case SINGLY_LINKED_LIST:
                return (searchAlgo == LINEAR)
                    ? (void*)jobLinkedList->linearSearchJobBySkills(skillSet, skillCount, matchAll)
                    : (void*)jobLinkedList->binarySearchJobBySkills(skillSet, skillCount, matchAll);

            case CIRCULAR_LINKED_LIST:
                return (searchAlgo == LINEAR)
                    ? (void*)jobCircular->linearSearchJobBySkills(skillSet, skillCount, matchAll)
                    : (void*)jobCircular->binarySearchJobBySkills(skillSet, skillCount, matchAll);

            default:
                cerr << "❌ Unknown data structure for job search!" << endl;
                return nullptr;
        }
    }

    cerr << "❌ Invalid search configuration!" << endl;
    return nullptr;
}

double JobMatching::ruleBasedMatch(Job job, Resume resume) {
    // 1. Search for matching skills (using selected DataStruct + SearchAlgorithm)
    auto searchResults = search(job.skills, job.skillCount, /*matchAll=*/false);

    // 2. Use results count to determine overlap
    int overlapCount = searchResults.size(); // or length, depending on your class

    if (job.skillCount == 0) return 0.0;

    // 3. Calculate match percentage
    double score = (static_cast<double>(overlapCount) / job.skillCount) * 100.0;

    return score;
}

double JobMatching::weightedScoringMatch(Job job, Resume resume) {
    if (job.skillCount == 0) return 0.0;

    double totalWeight = 0.0;
    double matchedWeight = 0.0;

    // Generate weight for each skill
    for (int i = 0; i < job.skillCount; i++) {
        double weight = 1.0 - (i * 0.1);
        if (weight < 0.1) weight = 0.1;
        totalWeight += weight;
    }

    // Use search() function to find overlapping skills
    auto searchResults = search(job.skills, job.skillCount, /*matchAll=*/false);
    int overlapCount = searchResults.size(); // adjust based on your structure

    // Estimate matched weight proportionally
    matchedWeight = (overlapCount / static_cast<double>(job.skillCount)) * totalWeight;

    double score = (matchedWeight / totalWeight) * 100.0;
    return score;
}

// how to determine weight