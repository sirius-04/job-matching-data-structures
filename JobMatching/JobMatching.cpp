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
    this->memoryUsed = 0;
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
    this->memoryUsed = 0;
    this->matchMode = FIND_RESUME;
    this->dataStruct = SINGLY_LINKED_LIST;
    this->matchStrategy = RULE_BASED;
    this->searchAlgo = LINEAR;
}

JobMatching::JobMatching(JobCircularLinkedList* jobCircular, ResumeCircularLinkedList* resumeCircular) {
    this->jobArray = nullptr;
    this->resumeArray = nullptr;
    this->jobLinkedList = nullptr;
    this->resumeLinkedList = nullptr;
    this->jobCircular = jobCircular;
    this->resumeCircular = resumeCircular;

    this->results = new MatchResultList();
    this->matchTime = 0;
    this->memoryUsed = 0;
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

MatchResultList* JobMatching::ruleBasedMatch(const string* skillSet, int skillCount, bool matchAll) {
    delete results;
    results = new MatchResultList();

    void* searchResult = search(skillSet, skillCount, matchAll);
    if (!searchResult) {
        cerr << "❌ No search result found!" << endl;
        return results;
    }

    if (matchMode == FIND_JOB) {
        switch (dataStruct) {
            case ARRAY: {
                JobArray* matchedJobs = static_cast<JobArray*>(searchResult);
                for (int i = 0; i < matchedJobs->getSize(); i++) {
                    Job job = matchedJobs->getJob(i);
                    results->append(MatchResult(job.id, 0, 100.0));
                }
                break;
            }
            case SINGLY_LINKED_LIST: {
                JobLinkedList* matchedJobs = static_cast<JobLinkedList*>(searchResult);
                JobNode* node = matchedJobs->getHead();
                
                while (node) {
                    results->append(MatchResult(node->data.id, 0, 100.0));
                    node = node->next;
                }
                break;
            }
            case CIRCULAR_LINKED_LIST: {
                JobCircularLinkedList* matchedJobs = static_cast<JobCircularLinkedList*>(searchResult);
                JobNode* node = matchedJobs->getHead();
                if (node) {
                    JobNode* start = node;
                    do {
                        results->append(MatchResult(node->data.id, 0, 100.0));
                        node = node->next;
                    } while (node != start);
                }
                break;
            }
        }
    }

    else if (matchMode == FIND_RESUME) {
        switch (dataStruct) {
            case ARRAY: {
                ResumeArray* matchedResumes = static_cast<ResumeArray*>(searchResult);
                for (int i = 0; i < matchedResumes->getSize(); i++) {
                    Resume resume = matchedResumes->getResume(i);
                    results->append(MatchResult(0, resume.id, 100.0));
                }
                break;
            }
            case SINGLY_LINKED_LIST: {
                ResumeLinkedList* matchedResumes = static_cast<ResumeLinkedList*>(searchResult);
                ResumeNode* node = matchedResumes->getHead();
                while (node) {
                    results->append(MatchResult(0, node->data.id, 100.0));
                    node = node->next;
                }
                break;
            }
            case CIRCULAR_LINKED_LIST: {
                ResumeCircularLinkedList* matchedResumes = static_cast<ResumeCircularLinkedList*>(searchResult);
                ResumeNode* node = matchedResumes->getHead();
                if (node) {
                    ResumeNode* start = node;
                    do {
                        results->append(MatchResult(0, node->data.id, 100.0));
                        node = node->next;
                    } while (node != start);
                }
                break;
            }
        }
    }

    return results;
}

SkillWeightList* JobMatching::promptSkillWeights(const string* skillSet, int skillCount) {
    cout << "\n===== Skill Weight Configuration =====" << endl;

    SkillWeightList* weightList = new SkillWeightList(skillCount);

    for (int i = 0; i < skillCount; i++) {
        int weight;
        while (true) {
            cout << "Enter weight (1–10) for skill \"" << skillSet[i] << "\": ";
            cin >> weight;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "❌ Invalid input! Please enter a number between 1 and 10.\n";
                continue;
            }

            if (weight < 1 || weight > 10) {
                cout << "❌ Please enter a value between 1 and 10.\n";
            } else {
                break;
            }
        }

        weightList->add(skillSet[i], weight);
    }

    cout << "\n✅ Skill weights recorded successfully!\n" << endl;
    return weightList;
}

double JobMatching::calculateWeightedScore(
    const string* inputSkills, int inputCount,
    const string* targetSkills, int targetCount,
    const SkillWeightList& weightList
) {
    if (targetCount == 0) return 0.0;

    int totalWeight = 0;
    int matchedWeight = 0;

    for (int i = 0; i < inputCount; i++) {
        totalWeight += weightList.getWeight(inputSkills[i]);
    }

    for (int i = 0; i < inputCount; i++) {
        int skillWeight = weightList.getWeight(inputSkills[i]);
        for (int j = 0; j < targetCount; j++) {
            if (inputSkills[i] == targetSkills[j]) {
                matchedWeight += skillWeight;
                break;
            }
        }
    }

    if (totalWeight == 0) return 0.0;
    return (static_cast<double>(matchedWeight) / totalWeight) * 100.0;
}

MatchResultList* JobMatching::weightedScoringMatch(const string* skillSet, int skillCount, bool matchAll) {
    delete results;
    results = new MatchResultList();

    cout << "\n===== Weighted Scoring Configuration =====" << endl;
    SkillWeightList* weightList = promptSkillWeights(skillSet, skillCount);

    cout << "\n===== Running Weighted Scoring Match =====" << endl;

    void* searchResult = search(skillSet, skillCount, matchAll);
    if (!searchResult) {
        cerr << "❌ No search result found!" << endl;
        return results;
    }

    if (matchMode == FIND_JOB) {
        switch (dataStruct) {
            case ARRAY: {
                JobArray* matchedJobs = static_cast<JobArray*>(searchResult);
                for (int i = 0; i < matchedJobs->getSize(); i++) {
                    Job job = matchedJobs->getJob(i);
                    double score = calculateWeightedScore(skillSet, skillCount, job.skills, job.skillCount, *weightList);
                    results->append(MatchResult(job.id, 0, score));
                }
                break;
            }

            case SINGLY_LINKED_LIST: {
                JobLinkedList* matchedJobs = static_cast<JobLinkedList*>(searchResult);
                JobNode* node = matchedJobs->getHead();
                while (node) {
                    double score = calculateWeightedScore(skillSet, skillCount, node->data.skills, node->data.skillCount, *weightList);
                    results->append(MatchResult(node->data.id, 0, score));
                    node = node->next;
                }
                break;
            }

            case CIRCULAR_LINKED_LIST: {
                JobCircularLinkedList* matchedJobs = static_cast<JobCircularLinkedList*>(searchResult);
                JobNode* node = matchedJobs->getHead();
                if (node) {
                    JobNode* startNode = node;
                    do {
                        double score = calculateWeightedScore(skillSet, skillCount, node->data.skills, node->data.skillCount, *weightList);
                        results->append(MatchResult(node->data.id, 0, score));
                        node = node->next;
                    } while (node != startNode);
                }
                break;
            }
        }
    }

    else if (matchMode == FIND_RESUME) {
        switch (dataStruct) {
            case ARRAY: {
                ResumeArray* matchedResumes = static_cast<ResumeArray*>(searchResult);
                for (int i = 0; i < matchedResumes->getSize(); i++) {
                    Resume resume = matchedResumes->getResume(i);
                    double score = calculateWeightedScore(skillSet, skillCount, resume.skills, resume.skillCount, *weightList);
                    results->append(MatchResult(0, resume.id, score));
                }
                break;
            }

            case SINGLY_LINKED_LIST: {
                ResumeLinkedList* matchedResumes = static_cast<ResumeLinkedList*>(searchResult);
                ResumeNode* node = matchedResumes->getHead();
                while (node) {
                    double score = calculateWeightedScore(skillSet, skillCount, node->data.skills, node->data.skillCount, *weightList);
                    results->append(MatchResult(0, node->data.id, score));
                    node = node->next;
                }
                break;
            }

            case CIRCULAR_LINKED_LIST: {
                ResumeCircularLinkedList* matchedResumes = static_cast<ResumeCircularLinkedList*>(searchResult);
                ResumeNode* node = matchedResumes->getHead();
                if (node) {
                    ResumeNode* startNode = node;
                    do {
                        double score = calculateWeightedScore(skillSet, skillCount, node->data.skills, node->data.skillCount, *weightList);
                        results->append(MatchResult(0, node->data.id, score));
                        node = node->next;
                    } while (node != startNode);
                }
                break;
            }
        }
    }

    return results;
}

size_t JobMatching::getCurrentMemoryUsage() {
    PROCESS_MEMORY_COUNTERS info;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info))) {
        return static_cast<size_t>(info.WorkingSetSize) / 1024; // in KB
    }
    return 0;
}

MatchResultList* JobMatching::runMatching(const string* skillSet, int skillCount, bool matchAll) {
    cout << "===== Running Match =====" << endl;

    auto startTime = high_resolution_clock::now();
    size_t startMem = getCurrentMemoryUsage();

    MatchResultList* matchResults = nullptr;

    switch (matchStrategy) {
        case RULE_BASED:
            cout << "[Strategy] Rule-Based Matching" << endl;
            matchResults = ruleBasedMatch(skillSet, skillCount, matchAll);
            break;

        case WEIGHTED_SCORING:
            cout << "[Strategy] Weighted Scoring Matching" << endl;
            matchResults = weightedScoringMatch(skillSet, skillCount, matchAll);
            break;

        default:
            cerr << "❌ Invalid match strategy!" << endl;
            return nullptr;
    }

    auto endTime = high_resolution_clock::now();
    size_t endMem = getCurrentMemoryUsage();

    matchTime = duration_cast<milliseconds>(endTime - startTime).count();
    memoryUsed = (endMem > startMem) ? (endMem - startMem) : 0;

    return matchResults;
}

void JobMatching::printPerformance() const {
    cout << "\n===== 🧩 Matching Configuration & Performance =====" << endl;

    // --- Match Mode ---
    cout << "🎯 Match Mode: ";
    switch (matchMode) {
        case FIND_JOB: cout << "Find Job (Resumes → Jobs)"; break;
        case FIND_RESUME: cout << "Find Resume (Jobs → Resumes)"; break;
        default: cout << "Unknown"; break;
    }
    cout << endl;

    // --- Match Strategy ---
    cout << "🧠 Match Strategy: ";
    switch (matchStrategy) {
        case RULE_BASED: cout << "Rule-Based"; break;
        case WEIGHTED_SCORING: cout << "Weighted Scoring"; break;
        default: cout << "Unknown"; break;
    }
    cout << endl;

    // --- Search Algorithm ---
    cout << "🔍 Search Algorithm: ";
    switch (searchAlgo) {
        case LINEAR: cout << "Linear Search"; break;
        case BINARY: cout << "Binary Search"; break;
        default: cout << "Unknown"; break;
    }
    cout << endl;

    // --- Data Structure ---
    cout << "📦 Data Structure: ";
    switch (dataStruct) {
        case ARRAY: cout << "Array"; break;
        case SINGLY_LINKED_LIST: cout << "Singly Linked List"; break;
        case CIRCULAR_LINKED_LIST: cout << "Circular Linked List"; break;
        default: cout << "Unknown"; break;
    }
    cout << endl;

    // --- Performance ---
    cout << "-----------------------------------------------" << endl;
    cout << "⏱️  Time Taken: " << matchTime << " ms" << endl;
    cout << "💾 Memory Used: " << memoryUsed << " KB" << endl;
    cout << "===============================================" << endl;
}
