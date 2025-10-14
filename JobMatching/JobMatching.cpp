#include "JobMatching.hpp"

JobMatching::JobMatching(JobLinkedList* jobs, ResumeLinkedList* resumes) {
    this->jobs = jobs;
    this->resumes = resumes;
    this->results = new MatchResultList();
    this->matchTime = 0;
}

JobMatching::~JobMatching() {
    delete results;
    results = nullptr;
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

MatchResultList JobMatching::search(const string* skillSet, int skillCount, bool matchAll) {
    if (matchMode == FIND_RESUME) {
        switch (dataStruct) {
            case ARRAY:
                if (searchAlgo == LINEAR)
                    return resumes->linearSearchBySkills(skillSet, skillCount, matchAll);
                else if (searchAlgo == BINARY)
                    return resumes->binarySearchBySkills(skillSet, skillCount, matchAll);
                break;

            case SINGLY_LINKED_LIST:
                if (searchAlgo == LINEAR)
                    return resumes->linearSearchResumeBySkills(skillSet, skillCount, matchAll);
                else if (searchAlgo == BINARY)
                    return resumes->binarySearchResumeBySkills(skillSet, skillCount, matchAll);
                break;

            case CIRCULAR_LINKED_LIST:
                if (searchAlgo == LINEAR)
                    return resumes->linearSearchResumeBySkills(skillSet, skillCount, matchAll);
                else if (searchAlgo == BINARY)
                    return resumes->binarySearchResumeBySkills(skillSet, skillCount, matchAll);
                break;

            default:
                cerr << "Unknown data structure for resume search!" << endl;
                break;
        }
    }
    else if (matchMode == FIND_JOB) {
        switch (dataStruct) {
            case ARRAY:
                if (searchAlgo == LINEAR)
                    return jobs->linearSearchBySkills(skillSet, skillCount, matchAll);
                else if (searchAlgo == BINARY)
                    return jobs->binarySearchBySkills(skillSet, skillCount, matchAll);
                break;

            case SINGLY_LINKED_LIST:
                if (searchAlgo == LINEAR)
                    return jobs->linearSearchJobBySkills(skillSet, skillCount, matchAll);
                else if (searchAlgo == BINARY)
                    return jobs->binarySearchJobBySkills(skillSet, skillCount, matchAll);
                break;

            case CIRCULAR_LINKED_LIST:
                if (searchAlgo == LINEAR)
                    return jobs->linearSearchJobBySkills(skillSet, skillCount, matchAll);
                else if (searchAlgo == BINARY)
                    return jobs->binarySearchJobBySkills(skillSet, skillCount, matchAll);
                break;

            default:
                cerr << "Unknown data structure for job search!" << endl;
                break;
        }
    }

    cerr << "Search configuration invalid!" << endl;
    return MatchResultList(); // return empty
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