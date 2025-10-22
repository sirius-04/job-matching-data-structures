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

    if (findJobMode) {
        cout << "Enter position keyword to filter jobs: ";
        cin >> ws;
        getline(cin, keyword);
    } else {
        cout << "Enter Job ID to find matching resumes: ";
        cin >> jobId;
    }

    // ========== ARRAY MODE ==========
    if (dataStruct == ARRAY) {
        if (findJobMode) {
            JobArray* filteredJobs = (JobArray*)searchJobsByPosition(keyword);
            
            if (filteredJobs && filteredJobs->getSize() > 0) {
                cout << "Processing " << filteredJobs->getSize() << " jobs..." << endl;
                
                for (int i = 0; i < filteredJobs->getSize(); i++) {
                    Job job = filteredJobs->getJob(i);
                    
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
            } else {
                cout << "No jobs found matching keyword: " << keyword << endl;
            }
            delete filteredJobs;
            
        } else {
            // FIND_RESUME mode
            Job* jobPtr = jobArray->findById(jobId);
            if (!jobPtr) {
                cout << "Job not found.\n";
                return results;
            }
            
            cout << "Processing resumes for Job ID " << jobId << "..." << endl;
            
            for (int j = 0; j < resumeArray->getSize(); j++) {
                Resume resume = resumeArray->getResume(j);
                
                int matchedSkills = 0;
                for (int a = 0; a < jobPtr->skillCount; a++) {
                    for (int b = 0; b < resume.skillCount; b++) {
                        if (jobPtr->skills[a] == resume.skills[b]) {
                            matchedSkills++;
                            break;
                        }
                    }
                }
                
                bool isMatch = matchAll ? (matchedSkills == jobPtr->skillCount)
                                        : (matchedSkills > 0);
                if (isMatch) {
                    double score = (static_cast<double>(matchedSkills) / jobPtr->skillCount) * 100.0;
                    results->append(MatchResult(jobPtr->id, resume.id, score));
                }
            }
        }
    }

    // ========== SINGLY LINKED LIST ==========
    else if (dataStruct == SINGLY_LINKED_LIST) {
        if (findJobMode) {
            JobLinkedList* filteredJobs = (JobLinkedList*)searchJobsByPosition(keyword);
            
            if (filteredJobs && filteredJobs->getHead()) {
                // Count jobs
                JobNode* jobNode = filteredJobs->getHead();
                int jobCount = 0;
                JobNode* temp = jobNode;
                
                while (temp) {
                    jobCount++;
                    temp = temp->next;
                }
                
                cout << "Processing " << jobCount << " jobs..." << endl;
                
                // Copy jobs to array for safe processing
                Job* jobsToProcess = new Job[jobCount];
                temp = jobNode;
                for (int i = 0; i < jobCount && temp; i++) {
                    jobsToProcess[i] = temp->data;
                    temp = temp->next;
                }
                
                // Process each job
                for (int i = 0; i < jobCount; i++) {
                    Job job = jobsToProcess[i];
                    
                    ResumeNode* resumeNode = resumeLinkedList->getHead();
                    while (resumeNode) {
                        Resume resume = resumeNode->data;
                        
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
                        
                        resumeNode = resumeNode->next;
                    }
                }
                
                delete[] jobsToProcess;
            } else {
                cout << "No jobs found matching keyword: " << keyword << endl;
            }
            delete filteredJobs;
            
        } else {
            // FIND_RESUME mode
            Job* jobPtr = jobLinkedList->findById(jobId);
            if (!jobPtr) {
                cout << "Job not found.\n";
                return results;
            }
            
            cout << "Processing resumes for Job ID " << jobId << "..." << endl;
            
            ResumeNode* resumeNode = resumeLinkedList->getHead();
            while (resumeNode) {
                Resume resume = resumeNode->data;
                
                int matchedSkills = 0;
                for (int a = 0; a < jobPtr->skillCount; a++) {
                    for (int b = 0; b < resume.skillCount; b++) {
                        if (jobPtr->skills[a] == resume.skills[b]) {
                            matchedSkills++;
                            break;
                        }
                    }
                }
                
                bool isMatch = matchAll ? (matchedSkills == jobPtr->skillCount)
                                        : (matchedSkills > 0);
                if (isMatch) {
                    double score = (static_cast<double>(matchedSkills) / jobPtr->skillCount) * 100.0;
                    results->append(MatchResult(jobPtr->id, resume.id, score));
                }
                
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
                
                // Count jobs in circular list
                int jobCount = 0;
                do {
                    jobCount++;
                    jobNode = jobNode->next;
                } while (jobNode != start);
                
                cout << "Processing " << jobCount << " jobs..." << endl;
                
                // Copy jobs to array
                Job* jobsToProcess = new Job[jobCount];
                jobNode = start;
                for (int i = 0; i < jobCount; i++) {
                    jobsToProcess[i] = jobNode->data;
                    jobNode = jobNode->next;
                }
                
                // Process each job
                for (int i = 0; i < jobCount; i++) {
                    Job job = jobsToProcess[i];
                    
                    ResumeNode* resumeNode = resumeCircular->getHead();
                    if (resumeNode) {
                        ResumeNode* rStart = resumeNode;
                        do {
                            Resume resume = resumeNode->data;
                            
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
                            
                            resumeNode = resumeNode->next;
                        } while (resumeNode != rStart);
                    }
                }
                
                delete[] jobsToProcess;
            } else {
                cout << "No jobs found matching keyword: " << keyword << endl;
            }
            delete filteredJobs;
            
        } else {
            // FIND_RESUME mode
            Job* jobPtr = jobCircular->findById(jobId);
            if (!jobPtr) {
                cout << "Job not found.\n";
                return results;
            }
            
            cout << "Processing resumes for Job ID " << jobId << "..." << endl;
            
            ResumeNode* resumeNode = resumeCircular->getHead();
            if (resumeNode) {
                ResumeNode* start = resumeNode;
                do {
                    Resume resume = resumeNode->data;
                    
                    int matchedSkills = 0;
                    for (int a = 0; a < jobPtr->skillCount; a++) {
                        for (int b = 0; b < resume.skillCount; b++) {
                            if (jobPtr->skills[a] == resume.skills[b]) {
                                matchedSkills++;
                                break;
                            }
                        }
                    }
                    
                    bool isMatch = matchAll ? (matchedSkills == jobPtr->skillCount)
                                            : (matchedSkills > 0);
                    if (isMatch) {
                        double score = (static_cast<double>(matchedSkills) / jobPtr->skillCount) * 100.0;
                        results->append(MatchResult(jobPtr->id, resume.id, score));
                    }
                    
                    resumeNode = resumeNode->next;
                } while (resumeNode != start);
            }
        }
    }

    cout << "Matching complete. Found " << results->getLength() << " matches." << endl;
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
    const string* jobSkills, int jobSkillCount,
    const string* resumeSkills, int resumeSkillCount,
    const SkillWeightList& weightList
) {
    if (jobSkillCount == 0 || resumeSkillCount == 0)
        return 0.0;

    int totalPossibleWeight = 0;
    int matchedWeight = 0;

    // FIXED: Calculate total weight from ALL job skills
    // If a job skill is not in weightList, assign default weight
    const int DEFAULT_WEIGHT = 1;  // Default weight for skills not in user's input
    
    for (int i = 0; i < jobSkillCount; i++) {
        int weight = weightList.getWeight(jobSkills[i]);
        if (weight == 0) {
            weight = DEFAULT_WEIGHT;  // Assign default weight to non-input skills
        }
        totalPossibleWeight += weight;
    }

    if (totalPossibleWeight == 0) return 0.0;

    // Calculate matched weight
    for (int i = 0; i < jobSkillCount; i++) {
        int skillWeight = weightList.getWeight(jobSkills[i]);
        if (skillWeight == 0) {
            skillWeight = DEFAULT_WEIGHT;
        }
        
        for (int j = 0; j < resumeSkillCount; j++) {
            if (jobSkills[i] == resumeSkills[j]) {
                matchedWeight += skillWeight;
                break;
            }
        }
    }
    
    double score = (static_cast<double>(matchedWeight) / totalPossibleWeight) * 100.0;
    if (score > 100.0) score = 100.0;
    
    return score;
}

MatchResultList* JobMatching::weightedScoringMatch(const string* skillSet, int skillCount, bool matchAll) {
    delete results;
    results = new MatchResultList();

    cout << "\n===== Weighted Scoring Match (Pure Scoring) =====" << endl;
    bool findJobMode = (matchMode == FIND_JOB);

    string keyword;
    int jobId;
    double minScore = 30.0;  // Default threshold

    if (findJobMode) {
        cout << "Enter position keyword to filter jobs: ";
        cin >> ws;
        getline(cin, keyword);
    } else {
        cout << "Enter Job ID to find matching resumes: ";
        cin >> jobId;
    }

    // Ask for minimum score threshold
    cout << "Enter minimum match score (0-100, default 30): ";
    cin >> minScore;
    if (cin.fail() || minScore < 0 || minScore > 100) {
        cout << "Invalid input. Using default threshold: 30%" << endl;
        minScore = 30.0;
        cin.clear();
        cin.ignore(1000, '\n');
    }

    SkillWeightList* weightList = promptSkillWeights(skillSet, skillCount);

    // ========== ARRAY MODE ==========
    if (dataStruct == ARRAY) {
        if (findJobMode) {
            JobArray* filteredJobs = (JobArray*)searchJobsByPosition(keyword);
            
            if (filteredJobs && filteredJobs->getSize() > 0) {
                cout << "Processing " << filteredJobs->getSize() << " jobs..." << endl;
                
                for (int i = 0; i < filteredJobs->getSize(); i++) {
                    Job job = filteredJobs->getJob(i);
                    
                    for (int j = 0; j < resumeArray->getSize(); j++) {
                        Resume resume = resumeArray->getResume(j);
                        
                        double score = calculateWeightedScore(
                            job.skills, job.skillCount,
                            resume.skills, resume.skillCount,
                            *weightList
                        );
                        
                        if (score >= minScore)
                            results->append(MatchResult(job.id, resume.id, score));
                    }
                }
            } else {
                cout << "No jobs found matching keyword: " << keyword << endl;
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
            
            cout << "Processing resumes for Job ID " << jobId << "..." << endl;
            
            for (int j = 0; j < resumeArray->getSize(); j++) {
                Resume resume = resumeArray->getResume(j);
                
                double score = calculateWeightedScore(
                    jobPtr->skills, jobPtr->skillCount,
                    resume.skills, resume.skillCount,
                    *weightList
                );
                
                if (score >= minScore)
                    results->append(MatchResult(jobPtr->id, resume.id, score));
            }
        }
    }

    // ========== SINGLY LINKED LIST ==========
    else if (dataStruct == SINGLY_LINKED_LIST) {
        if (findJobMode) {
            JobLinkedList* filteredJobs = (JobLinkedList*)searchJobsByPosition(keyword);
            
            if (filteredJobs && filteredJobs->getHead()) {
                // Count jobs
                JobNode* jobNode = filteredJobs->getHead();
                int jobCount = 0;
                JobNode* temp = jobNode;
                
                while (temp) {
                    jobCount++;
                    temp = temp->next;
                }
                
                cout << "Processing " << jobCount << " jobs..." << endl;
                
                // Copy jobs to array for safe processing
                Job* jobsToProcess = new Job[jobCount];
                temp = jobNode;
                for (int i = 0; i < jobCount && temp; i++) {
                    jobsToProcess[i] = temp->data;
                    temp = temp->next;
                }
                
                // Process each job
                for (int i = 0; i < jobCount; i++) {
                    Job job = jobsToProcess[i];
                    
                    ResumeNode* resumeNode = resumeLinkedList->getHead();
                    while (resumeNode) {
                        Resume resume = resumeNode->data;
                        
                        double score = calculateWeightedScore(
                            job.skills, job.skillCount,
                            resume.skills, resume.skillCount,
                            *weightList
                        );
                        
                        if (score >= minScore)
                            results->append(MatchResult(job.id, resume.id, score));
                        
                        resumeNode = resumeNode->next;
                    }
                }
                
                delete[] jobsToProcess;
            } else {
                cout << "No jobs found matching keyword: " << keyword << endl;
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
            
            cout << "Processing resumes for Job ID " << jobId << "..." << endl;
            
            ResumeNode* resumeNode = resumeLinkedList->getHead();
            while (resumeNode) {
                Resume resume = resumeNode->data;
                
                double score = calculateWeightedScore(
                    jobPtr->skills, jobPtr->skillCount,
                    resume.skills, resume.skillCount,
                    *weightList
                );
                
                if (score >= minScore)
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
                
                // Count jobs in circular list
                int jobCount = 0;
                do {
                    jobCount++;
                    jobNode = jobNode->next;
                } while (jobNode != start);
                
                cout << "Processing " << jobCount << " jobs..." << endl;
                
                // Copy jobs to array
                Job* jobsToProcess = new Job[jobCount];
                jobNode = start;
                for (int i = 0; i < jobCount; i++) {
                    jobsToProcess[i] = jobNode->data;
                    jobNode = jobNode->next;
                }
                
                // Process each job
                for (int i = 0; i < jobCount; i++) {
                    Job job = jobsToProcess[i];
                    
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
                            
                            if (score >= minScore)
                                results->append(MatchResult(job.id, resume.id, score));
                            
                            resumeNode = resumeNode->next;
                        } while (resumeNode != rStart);
                    }
                }
                
                delete[] jobsToProcess;
            } else {
                cout << "No jobs found matching keyword: " << keyword << endl;
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
            
            cout << "Processing resumes for Job ID " << jobId << "..." << endl;
            
            ResumeNode* resumeNode = resumeCircular->getHead();
            if (resumeNode) {
                ResumeNode* start = resumeNode;
                do {
                    Resume resume = resumeNode->data;
                    
                    double score = calculateWeightedScore(
                        jobPtr->skills, jobPtr->skillCount,
                        resume.skills, resume.skillCount,
                        *weightList
                    );
                    
                    if (score >= minScore)
                        results->append(MatchResult(jobPtr->id, resume.id, score));
                    
                    resumeNode = resumeNode->next;
                } while (resumeNode != start);
            }
        }
    }

    cout << "Matching complete. Found " << results->getLength() << " matches above " 
         << minScore << "% threshold." << endl;

    delete weightList;
    return results;
}

MatchResultList* JobMatching::runMatching(const string* skillSet, int skillCount, bool matchAll)
{
    cout << "===== Running Match =====" << endl;

    MatchResultList* matchResults = nullptr;

    // Use PerformanceTracker to measure
    PerformanceResult perf = PerformanceTracker::measure([&]() {
        switch (matchStrategy)
        {
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
            matchResults = nullptr;
            break;
        }
    });

    // Store results for later reference or printing
    matchTime = perf.timeTakenMs;
    memoryUsed = perf.memoryUsedBytes;

    cout << "\n--- PERFORMANCE SUMMARY ---" << endl;
    perf.printPerformance("Matching Execution");

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
