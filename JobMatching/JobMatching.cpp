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

void* JobMatching::searchBySkills(const string* skillSet, int skillCount, bool matchAll) {
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

void* JobMatching::searchJobsByPosition(string position) {
    switch (dataStruct) {
        case ARRAY:
            return (searchAlgo == LINEAR)
                ? (void*)jobArray->linearSearchByPosition(position)
                : (void*)jobArray->binarySearchByPosition(position);

        case SINGLY_LINKED_LIST:
            return (searchAlgo == LINEAR)
                ? (void*)jobLinkedList->linearSearchJobByPosition(position)
                : (void*)jobLinkedList->binarySearchJobByPosition(position);

        case CIRCULAR_LINKED_LIST:
            return (searchAlgo == LINEAR)
                ? (void*)jobCircular->linearSearchJobByPosition(position)
                : (void*)jobCircular->binarySearchJobByPosition(position);

        default:
            cerr << "❌ Unknown data structure for job position search!" << endl;
            return nullptr;
    }
}

MatchResultList* JobMatching::ruleBasedMatch(const string* skillSet, int skillCount, bool matchAll) {
    delete results;
    results = new MatchResultList();

    cout << "\n===== Running Rule-Based Match (Filtered Evaluation) =====" << endl;
    bool findJobMode = (matchMode == FIND_JOB);

    string keyword;
    int jobId;

    // ===== STEP 1: Prepare filter =====
    if (findJobMode) {
        cout << "Enter position keyword to filter jobs: ";
        cin >> ws;
        getline(cin, keyword);
    } else {
        cout << "Enter Job ID to find matching resumes: ";
        cin >> jobId;
    }

    // ===== STEP 2: Match by data structure =====
    switch (dataStruct) {
    // ================= ARRAY =================
    case ARRAY: {
        if (findJobMode) {
            for (int j = 0; j < resumeArray->getSize(); j++) {
                Resume resume = resumeArray->getResume(j);

                for (int i = 0; i < jobArray->getSize(); i++) {
                    Job job = jobArray->getJob(i);
                    if (job.position.find(keyword) == string::npos)
                        continue;

                    int matchedSkills = 0;
                    for (int a = 0; a < job.skillCount; a++) {
                        for (int b = 0; b < resume.skillCount; b++) {
                            if (job.skills[a] == resume.skills[b]) {
                                matchedSkills++;
                                break;
                            }
                        }
                    }

                    bool isMatch = matchAll ? (matchedSkills == job.skillCount)
                                            : (matchedSkills > 0);
                    if (isMatch) {
                        double score = (static_cast<double>(matchedSkills) / job.skillCount) * 100.0;
                        results->append(MatchResult(job.id, resume.id, score));
                    }
                }
            }
        } else {
            Job* jobPtr = jobArray->findById(jobId);
            if (!jobPtr) {
                cout << "Job not found.\n";
                break;
            }
            Job job = *jobPtr;

            for (int j = 0; j < resumeArray->getSize(); j++) {
                Resume resume = resumeArray->getResume(j);
                int matchedSkills = 0;
                for (int a = 0; a < job.skillCount; a++) {
                    for (int b = 0; b < resume.skillCount; b++) {
                        if (job.skills[a] == resume.skills[b]) {
                            matchedSkills++;
                            break;
                        }
                    }
                }
                bool isMatch = matchAll ? (matchedSkills == job.skillCount)
                                        : (matchedSkills > 0);
                if (isMatch) {
                    double score = (static_cast<double>(matchedSkills) / job.skillCount) * 100.0;
                    results->append(MatchResult(job.id, resume.id, score));
                }
            }
        }
        break;
    }

    // ================= SINGLY LINKED LIST =================
    case SINGLY_LINKED_LIST: {
        if (findJobMode) {
            ResumeNode* resNode = resumeLinkedList->getHead();
            while (resNode) {
                JobNode* jobNode = jobLinkedList->getHead();
                while (jobNode) {
                    if (jobNode->data.position.find(keyword) == string::npos) {
                        jobNode = jobNode->next;
                        continue;
                    }

                    int matchedSkills = 0;
                    for (int a = 0; a < jobNode->data.skillCount; a++) {
                        for (int b = 0; b < resNode->data.skillCount; b++) {
                            if (jobNode->data.skills[a] == resNode->data.skills[b]) {
                                matchedSkills++;
                                break;
                            }
                        }
                    }

                    bool isMatch = matchAll ? (matchedSkills == jobNode->data.skillCount)
                                            : (matchedSkills > 0);
                    if (isMatch) {
                        double score = (static_cast<double>(matchedSkills) / jobNode->data.skillCount) * 100.0;
                        results->append(MatchResult(jobNode->data.id, resNode->data.id, score));
                    }

                    jobNode = jobNode->next;
                }
                resNode = resNode->next;
            }
        } else {
            JobNode* jobNode = jobLinkedList->getHead();
            Job* targetJob = nullptr;
            while (jobNode) {
                if (jobNode->data.id == jobId) {
                    targetJob = &jobNode->data;
                    break;
                }
                jobNode = jobNode->next;
            }
            if (!targetJob) {
                cout << "Job not found.\n";
                break;
            }

            ResumeNode* resNode = resumeLinkedList->getHead();
            while (resNode) {
                int matchedSkills = 0;
                for (int a = 0; a < targetJob->skillCount; a++) {
                    for (int b = 0; b < resNode->data.skillCount; b++) {
                        if (targetJob->skills[a] == resNode->data.skills[b]) {
                            matchedSkills++;
                            break;
                        }
                    }
                }
                bool isMatch = matchAll ? (matchedSkills == targetJob->skillCount)
                                        : (matchedSkills > 0);
                if (isMatch) {
                    double score = (static_cast<double>(matchedSkills) / targetJob->skillCount) * 100.0;
                    results->append(MatchResult(targetJob->id, resNode->data.id, score));
                }

                resNode = resNode->next;
            }
        }
        break;
    }

    // ================= CIRCULAR LINKED LIST =================
    case CIRCULAR_LINKED_LIST: {
        if (!jobCircular->getHead() || !resumeCircular->getHead()) return results;

        if (findJobMode) {
            ResumeNode* resStart = resumeCircular->getHead();
            ResumeNode* resNode = resStart;
            do {
                JobNode* jobStart = jobCircular->getHead();
                JobNode* jobNode = jobStart;
                do {
                    if (jobNode->data.position.find(keyword) == string::npos) {
                        jobNode = jobNode->next;
                        continue;
                    }

                    int matchedSkills = 0;
                    for (int a = 0; a < jobNode->data.skillCount; a++) {
                        for (int b = 0; b < resNode->data.skillCount; b++) {
                            if (jobNode->data.skills[a] == resNode->data.skills[b]) {
                                matchedSkills++;
                                break;
                            }
                        }
                    }

                    bool isMatch = matchAll ? (matchedSkills == jobNode->data.skillCount)
                                            : (matchedSkills > 0);
                    if (isMatch) {
                        double score = (static_cast<double>(matchedSkills) / jobNode->data.skillCount) * 100.0;
                        results->append(MatchResult(jobNode->data.id, resNode->data.id, score));
                    }

                    jobNode = jobNode->next;
                } while (jobNode != jobStart);
                resNode = resNode->next;
            } while (resNode != resStart);
        } else {
            JobNode* jobStart = jobCircular->getHead();
            JobNode* jobNode = jobStart;
            Job* targetJob = nullptr;
            do {
                if (jobNode->data.id == jobId) {
                    targetJob = &jobNode->data;
                    break;
                }
                jobNode = jobNode->next;
            } while (jobNode != jobStart);
            if (!targetJob) {
                cout << "Job not found.\n";
                break;
            }

            ResumeNode* resStart = resumeCircular->getHead();
            ResumeNode* resNode = resStart;
            do {
                int matchedSkills = 0;
                for (int a = 0; a < targetJob->skillCount; a++) {
                    for (int b = 0; b < resNode->data.skillCount; b++) {
                        if (targetJob->skills[a] == resNode->data.skills[b]) {
                            matchedSkills++;
                            break;
                        }
                    }
                }
                bool isMatch = matchAll ? (matchedSkills == targetJob->skillCount)
                                        : (matchedSkills > 0);
                if (isMatch) {
                    double score = (static_cast<double>(matchedSkills) / targetJob->skillCount) * 100.0;
                    results->append(MatchResult(targetJob->id, resNode->data.id, score));
                }
                resNode = resNode->next;
            } while (resNode != resStart);
        }
        break;
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
            cout << "Enter weight (1-10) for skill \"" << skillSet[i] << "\": ";
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
    if (inputCount == 0 || targetCount == 0)
        return 0.0;

    int totalPossibleWeight = 0;
    int matchedWeight = 0;

    // Calculate total weight from input skills only (the reference set)
    for (int i = 0; i < inputCount; i++)
        totalPossibleWeight += weightList.getWeight(inputSkills[i]);

    if (totalPossibleWeight == 0) return 0.0;

    // Calculate matched weight
    for (int i = 0; i < inputCount; i++) {
        int skillWeight = weightList.getWeight(inputSkills[i]);
        for (int j = 0; j < targetCount; j++) {
            if (inputSkills[i] == targetSkills[j]) {
                matchedWeight += skillWeight;
                break;
            }
        }
    }
    
    double score = (static_cast<double>(matchedWeight) / totalPossibleWeight) * 100.0;
    if (score > 100.0) score = 100.0;  // Cap at 100%
    
    return score;
}

MatchResultList* JobMatching::weightedScoringMatch(const string* skillSet, int skillCount, bool matchAll) {
    delete results;
    results = new MatchResultList();

    cout << "\n===== Weighted Scoring Match (Filtered Evaluation) =====" << endl;
    bool findJobMode = (matchMode == FIND_JOB);

    string keyword;
    int jobId;

    if (findJobMode) {
        cout << "Enter position keyword to filter jobs: ";
        cin >> ws;
        getline(cin, keyword);
    } else {
        cout << "Enter Job ID to find matching resumes: ";
        cin >> jobId;
    }

    SkillWeightList* weightList = promptSkillWeights(skillSet, skillCount);

    // Helper lambda to check if entity has required skills
    auto hasRequiredSkills = [&](const string* entitySkills, int entityCount) -> bool {
        if (matchAll) {
            // Must have ALL skills from skillSet
            for (int i = 0; i < skillCount; i++) {
                bool found = false;
                for (int j = 0; j < entityCount; j++) {
                    if (skillSet[i] == entitySkills[j]) {
                        found = true;
                        break;
                    }
                }
                if (!found) return false;  // Missing a required skill
            }
            return true;
        } else {
            // Must have AT LEAST ONE skill from skillSet
            for (int i = 0; i < skillCount; i++) {
                for (int j = 0; j < entityCount; j++) {
                    if (skillSet[i] == entitySkills[j]) {
                        return true;
                    }
                }
            }
            return false;
        }
    };

    auto isMatchScore = [&](double score) {
        return matchAll ? (score >= 90.0) : (score > 0.0);
    };

    // ========== ARRAY MODE ==========
    if (dataStruct == ARRAY) {
        if (findJobMode) {
            JobArray* filteredJobs = (JobArray*)searchJobsByPosition(keyword);
            
            if (filteredJobs && filteredJobs->getSize() > 0) {
                for (int i = 0; i < filteredJobs->getSize(); i++) {
                    Job job = filteredJobs->getJob(i);
                    
                    // FIND_JOB mode: Filter jobs by required skills
                    if (!hasRequiredSkills(job.skills, job.skillCount))
                        continue;
                    
                    for (int j = 0; j < resumeArray->getSize(); j++) {
                        Resume resume = resumeArray->getResume(j);
                        
                        double score = calculateWeightedScore(
                            job.skills, job.skillCount,
                            resume.skills, resume.skillCount,
                            *weightList
                        );
                        if (isMatchScore(score))
                            results->append(MatchResult(job.id, resume.id, score));
                    }
                }
            }
            delete filteredJobs;
        } else {
            // FIND_RESUME mode
            Job* jobPtr = jobArray->findById(jobId);
            if (!jobPtr) {
                cout << "Job not found.\n";
                delete weightList;
                return results;
            }
            
            for (int j = 0; j < resumeArray->getSize(); j++) {
                Resume resume = resumeArray->getResume(j);
                
                // FIND_RESUME mode: Filter resumes by required skills
                if (!hasRequiredSkills(resume.skills, resume.skillCount))
                    continue;
                
                double score = calculateWeightedScore(
                    jobPtr->skills, jobPtr->skillCount,
                    resume.skills, resume.skillCount,
                    *weightList
                );
                if (isMatchScore(score))
                    results->append(MatchResult(jobPtr->id, resume.id, score));
            }
        }
    }

    // ========== SINGLY LINKED LIST ==========
    else if (dataStruct == SINGLY_LINKED_LIST) {
        if (findJobMode) {
            JobLinkedList* filteredJobs = (JobLinkedList*)searchJobsByPosition(keyword);
            
            if (filteredJobs && filteredJobs->getHead()) {
                JobNode* jobNode = filteredJobs->getHead();
                int jobCount = 0;
                JobNode* temp = jobNode;
                while (temp) {
                    jobCount++;
                    temp = temp->next;
                }
                
                Job* jobsToProcess = new Job[jobCount];
                temp = jobNode;
                for (int i = 0; i < jobCount && temp; i++) {
                    jobsToProcess[i] = temp->data;
                    temp = temp->next;
                }
                
                for (int i = 0; i < jobCount; i++) {
                    Job job = jobsToProcess[i];
                    
                    // FIND_JOB mode: Filter jobs by required skills
                    if (!hasRequiredSkills(job.skills, job.skillCount))
                        continue;
                    
                    ResumeNode* resumeNode = resumeLinkedList->getHead();
                    while (resumeNode) {
                        Resume resume = resumeNode->data;
                        
                        double score = calculateWeightedScore(
                            job.skills, job.skillCount,
                            resume.skills, resume.skillCount,
                            *weightList
                        );
                        if (isMatchScore(score))
                            results->append(MatchResult(job.id, resume.id, score));
                        resumeNode = resumeNode->next;
                    }
                }
                delete[] jobsToProcess;
            }
            delete filteredJobs;
        } else {
            // FIND_RESUME mode
            Job* jobPtr = jobLinkedList->findById(jobId);
            if (!jobPtr) {
                cout << "Job not found.\n";
                delete weightList;
                return results;
            }
            
            ResumeNode* resumeNode = resumeLinkedList->getHead();
            while (resumeNode) {
                Resume resume = resumeNode->data;
                
                // FIND_RESUME mode: Filter resumes by required skills
                if (!hasRequiredSkills(resume.skills, resume.skillCount)) {
                    resumeNode = resumeNode->next;
                    continue;
                }
                
                double score = calculateWeightedScore(
                    jobPtr->skills, jobPtr->skillCount,
                    resume.skills, resume.skillCount,
                    *weightList
                );
                if (isMatchScore(score))
                    results->append(MatchResult(jobPtr->id, resume.id, score));
                resumeNode = resumeNode->next;
            }
        }
    }

    // ========== CIRCULAR LINKED LIST ==========
    else if (dataStruct == CIRCULAR_LINKED_LIST) {
        if (findJobMode) {
            JobCircularLinkedList* filteredJobs = (JobCircularLinkedList*)searchJobsByPosition(keyword);
            
            if (filteredJobs && filteredJobs->getHead()) {
                JobNode* jobNode = filteredJobs->getHead();
                JobNode* start = jobNode;
                
                int jobCount = 0;
                do {
                    jobCount++;
                    jobNode = jobNode->next;
                } while (jobNode != start);
                
                Job* jobsToProcess = new Job[jobCount];
                jobNode = start;
                for (int i = 0; i < jobCount; i++) {
                    jobsToProcess[i] = jobNode->data;
                    jobNode = jobNode->next;
                }
                
                for (int i = 0; i < jobCount; i++) {
                    Job job = jobsToProcess[i];
                    
                    // FIND_JOB mode: Filter jobs by required skills
                    if (!hasRequiredSkills(job.skills, job.skillCount))
                        continue;
                    
                    ResumeNode* resumeNode = resumeCircular->getHead();
                    if (resumeNode) {
                        ResumeNode* rStart = resumeNode;
                        do {
                            Resume resume = resumeNode->data;
                            
                            double score = calculateWeightedScore(
                                job.skills, job.skillCount,
                                resume.skills, resume.skillCount,
                                *weightList
                            );
                            if (isMatchScore(score))
                                results->append(MatchResult(job.id, resume.id, score));
                            
                            resumeNode = resumeNode->next;
                        } while (resumeNode != rStart);
                    }
                }
                delete[] jobsToProcess;
            }
            delete filteredJobs;
        } else {
            // FIND_RESUME mode
            Job* jobPtr = jobCircular->findById(jobId);
            if (!jobPtr) {
                cout << "Job not found.\n";
                delete weightList;
                return results;
            }
            
            ResumeNode* resumeNode = resumeCircular->getHead();
            if (resumeNode) {
                ResumeNode* start = resumeNode;
                do {
                    Resume resume = resumeNode->data;
                    
                    // FIND_RESUME mode: Filter resumes by required skills
                    if (hasRequiredSkills(resume.skills, resume.skillCount)) {
                        double score = calculateWeightedScore(
                            jobPtr->skills, jobPtr->skillCount,
                            resume.skills, resume.skillCount,
                            *weightList
                        );
                        if (isMatchScore(score))
                            results->append(MatchResult(jobPtr->id, resume.id, score));
                    }
                    
                    resumeNode = resumeNode->next;
                } while (resumeNode != start);
            }
        }
    }

    delete weightList;
    return results;
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
