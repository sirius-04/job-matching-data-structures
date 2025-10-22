#include "JobMatching.hpp"

#include "JobMatching.hpp"
#include <algorithm>

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

// ==================== Helper: Convert search results to array ====================
Job* JobMatching::convertJobsToArray(void* searchResult, int& outCount) {
    outCount = 0;
    
    if (!searchResult) return nullptr;
    
    switch (dataStruct) {
        case ARRAY: {
            JobArray* jobArr = (JobArray*)searchResult;
            outCount = jobArr->getSize();
            if (outCount == 0) return nullptr;
            
            Job* jobs = new Job[outCount];
            for (int i = 0; i < outCount; i++) {
                jobs[i] = jobArr->getJob(i);
            }
            return jobs;
        }
        
        case SINGLY_LINKED_LIST: {
            JobLinkedList* jobList = (JobLinkedList*)searchResult;
            if (!jobList->getHead()) return nullptr;

            JobNode* node = jobList->getHead();
            outCount = jobList->getLength();
            
            // Copy to array
            Job* jobs = new Job[outCount];
            node = jobList->getHead();
            for (int i = 0; i < outCount && node; i++) {
                jobs[i] = node->data;
                node = node->next;
            }
            return jobs;
        }
        
        case CIRCULAR_LINKED_LIST: {
            JobCircularLinkedList* jobCirc = (JobCircularLinkedList*)searchResult;
            if (!jobCirc->getHead()) return nullptr;
            
            // Count nodes
            JobNode* start = jobCirc->getHead();
            JobNode* node = start;
            do {
                outCount++;
                node = node->next;
            } while (node != start);
            
            // Copy to array
            Job* jobs = new Job[outCount];
            node = start;
            for (int i = 0; i < outCount; i++) {
                jobs[i] = node->data;
                node = node->next;
            }
            return jobs;
        }
        
        default:
            return nullptr;
    }
}

Resume* JobMatching::convertResumesToArray(void* searchResult, int& outCount) {
    outCount = 0;
    
    if (!searchResult) return nullptr;
    
    switch (dataStruct) {
        case ARRAY: {
            ResumeArray* resArr = (ResumeArray*)searchResult;
            outCount = resArr->getSize();
            if (outCount == 0) return nullptr;
            
            Resume* resumes = new Resume[outCount];
            for (int i = 0; i < outCount; i++) {
                resumes[i] = resArr->getResume(i);
            }
            return resumes;
        }
        
        case SINGLY_LINKED_LIST: {
            ResumeLinkedList* resList = (ResumeLinkedList*)searchResult;
            if (!resList->getHead()) return nullptr;
            
            // Count nodes
            ResumeNode* node = resList->getHead();
            outCount = resList->getLength();
            
            // Copy to array
            Resume* resumes = new Resume[outCount];
            node = resList->getHead();
            for (int i = 0; i < outCount && node; i++) {
                resumes[i] = node->data;
                node = node->next;
            }
            return resumes;
        }
        
        case CIRCULAR_LINKED_LIST: {
            ResumeCircularLinkedList* resCirc = (ResumeCircularLinkedList*)searchResult;
            if (!resCirc->getHead()) return nullptr;
            
            // Count nodes
            ResumeNode* start = resCirc->getHead();
            ResumeNode* node = start;
            do {
                outCount++;
                node = node->next;
            } while (node != start);
            
            // Copy to array
            Resume* resumes = new Resume[outCount];
            node = start;
            for (int i = 0; i < outCount; i++) {
                resumes[i] = node->data;
                node = node->next;
            }
            return resumes;
        }
        
        default:
            return nullptr;
    }
}

Resume* JobMatching::getAllResumesAsArray(int& outCount) {
    outCount = 0;
    
    switch (dataStruct) {
        case ARRAY: {
            outCount = resumeArray->getSize();
            if (outCount == 0) return nullptr;
            
            Resume* resumes = new Resume[outCount];
            for (int i = 0; i < outCount; i++) {
                resumes[i] = resumeArray->getResume(i);
            }
            return resumes;
        }
        
        case SINGLY_LINKED_LIST: {
            if (!resumeLinkedList->getHead()) return nullptr;
            
            ResumeNode* node = resumeLinkedList->getHead();
            outCount = resumeLinkedList->getLength();
            
            Resume* resumes = new Resume[outCount];
            node = resumeLinkedList->getHead();
            for (int i = 0; i < outCount && node; i++) {
                resumes[i] = node->data;
                node = node->next;
            }
            return resumes;
        }
        
        case CIRCULAR_LINKED_LIST: {
            if (!resumeCircular->getHead()) return nullptr;
            
            ResumeNode* start = resumeCircular->getHead();
            ResumeNode* node = start;
            do {
                outCount++;
                node = node->next;
            } while (node != start);
            
            Resume* resumes = new Resume[outCount];
            node = start;
            for (int i = 0; i < outCount; i++) {
                resumes[i] = node->data;
                node = node->next;
            }
            return resumes;
        }
        
        default:
            return nullptr;
    }
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
    if (jobSkillCount == 0 || resumeSkillCount == 0) return 0.0;

    int totalPossibleWeight = 0;
    int matchedWeight = 0;
    const int DEFAULT_WEIGHT = 1;

    // Total possible weight (sum of all job skills)
    for (int i = 0; i < jobSkillCount; i++) {
        int w = weightList.getWeight(jobSkills[i]);
        totalPossibleWeight += (w == 0) ? DEFAULT_WEIGHT : w;
    }
    if (totalPossibleWeight == 0) return 0.0;

    // Calculate matched weight
    for (int i = 0; i < jobSkillCount; i++) {
        int w = weightList.getWeight(jobSkills[i]);
        if (w == 0) w = DEFAULT_WEIGHT;

        for (int j = 0; j < resumeSkillCount; j++) {
            if (jobSkills[i] == resumeSkills[j]) {
                matchedWeight += w;
                break;
            }
        }
    }

    double score = (static_cast<double>(matchedWeight) / totalPossibleWeight) * 100.0;
    return (score > 100.0) ? 100.0 : score;
}

MatchResultList* JobMatching::ruleBasedMatch(const string* skillSet, int skillCount, bool matchAll) {
    delete results;
    results = new MatchResultList();

    cout << "\n===== Running Rule-Based Match (Filtered Evaluation) =====" << endl;
    if (dataStruct == ARRAY) cout << "ARRAY";
    else if (dataStruct == SINGLY_LINKED_LIST) cout << "SINGLY_LINKED_LIST";
    else if (dataStruct == CIRCULAR_LINKED_LIST) cout << "CIRCULAR_LINKED_LIST";
    cout << endl;
    
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

    if (findJobMode) {
        // Step 1: Search and convert jobs to array
        void* searchResult = searchJobsByPosition(keyword);
        int jobCount = 0;
        Job* jobs = convertJobsToArray(searchResult, jobCount);
        
        // Clean up search result based on data structure
        if (dataStruct == ARRAY) {
            delete (JobArray*)searchResult;
        } else if (dataStruct == SINGLY_LINKED_LIST) {
            delete (JobLinkedList*)searchResult;
        } else if (dataStruct == CIRCULAR_LINKED_LIST) {
            delete (JobCircularLinkedList*)searchResult;
        }
        
        if (!jobs || jobCount == 0) {
            cout << "No jobs found matching keyword: " << keyword << endl;
            if (jobs) delete[] jobs;
            return results;
        }
        
        cout << "Processing " << jobCount << " jobs..." << endl;
        
        // Step 2: For each job, search matching resumes and convert to array
        for (int i = 0; i < jobCount; i++) {
            Job job = jobs[i];
            
            // Search for matching resumes using dedicated function
            void* resumeSearchResult = nullptr;
            switch (dataStruct) {
                case ARRAY:
                    resumeSearchResult = resumeArray->linearSearchBySkills(job.skills, job.skillCount, matchAll);
                    break;
                case SINGLY_LINKED_LIST:
                    resumeSearchResult = resumeLinkedList->linearSearchResumeBySkills(job.skills, job.skillCount, matchAll);
                    break;
                case CIRCULAR_LINKED_LIST:
                    resumeSearchResult = resumeCircular->linearSearchResumeBySkills(job.skills, job.skillCount, matchAll);
                    break;
            }
            
            int resumeCount = 0;
            Resume* resumes = convertResumesToArray(resumeSearchResult, resumeCount);
            
            // Clean up resume search result
            if (dataStruct == ARRAY) {
                delete (ResumeArray*)resumeSearchResult;
            } else if (dataStruct == SINGLY_LINKED_LIST) {
                delete (ResumeLinkedList*)resumeSearchResult;
            } else if (dataStruct == CIRCULAR_LINKED_LIST) {
                delete (ResumeCircularLinkedList*)resumeSearchResult;
            }
            
            // Step 3: Calculate scores and add to results
            if (resumes && resumeCount > 0) {
                for (int j = 0; j < resumeCount; j++) {
                    Resume resume = resumes[j];
                    
                    // Calculate matched skills
                    int matchedSkills = 0;
                    for (int a = 0; a < job.skillCount; a++) {
                        for (int b = 0; b < resume.skillCount; b++) {
                            if (job.skills[a] == resume.skills[b]) {
                                matchedSkills++;
                                break;
                            }
                        }
                    }
                    
                    double score = (static_cast<double>(matchedSkills) / job.skillCount) * 100.0;

                    results->append(MatchResult(job.id, resume.id, score));
                }
            }
            
            if (resumes) delete[] resumes;
        }
        
        delete[] jobs;
        
    } else {
        // FIND_RESUME mode: Find one job, then search matching resumes
        Job* jobPtr = nullptr;
        switch (dataStruct) {
            case ARRAY:
                jobPtr = jobArray->findById(jobId);
                break;
            case SINGLY_LINKED_LIST:
                jobPtr = jobLinkedList->findById(jobId);
                break;
            case CIRCULAR_LINKED_LIST:
                jobPtr = jobCircular->findById(jobId);
                break;
        }
        
        if (!jobPtr) {
            cout << "Job not found.\n";
            return results;
        }
        
        cout << "Processing resumes for Job ID " << jobId << "..." << endl;
        
        // Search for matching resumes
        void* resumeSearchResult = nullptr;
        switch (dataStruct) {
            case ARRAY:
                resumeSearchResult = resumeArray->linearSearchBySkills(jobPtr->skills, jobPtr->skillCount, matchAll);
                break;
            case SINGLY_LINKED_LIST:
                resumeSearchResult = resumeLinkedList->linearSearchResumeBySkills(jobPtr->skills, jobPtr->skillCount, matchAll);
                break;
            case CIRCULAR_LINKED_LIST:
                resumeSearchResult = resumeCircular->linearSearchResumeBySkills(jobPtr->skills, jobPtr->skillCount, matchAll);
                break;
        }
        
        int resumeCount = 0;
        Resume* resumes = convertResumesToArray(resumeSearchResult, resumeCount);
        
        // Clean up search result
        if (dataStruct == ARRAY) {
            delete (ResumeArray*)resumeSearchResult;
        } else if (dataStruct == SINGLY_LINKED_LIST) {
            delete (ResumeLinkedList*)resumeSearchResult;
        } else if (dataStruct == CIRCULAR_LINKED_LIST) {
            delete (ResumeCircularLinkedList*)resumeSearchResult;
        }
        
        // Calculate scores
        if (resumes && resumeCount > 0) {
            for (int j = 0; j < resumeCount; j++) {
                Resume resume = resumes[j];
                
                int matchedSkills = 0;
                for (int a = 0; a < jobPtr->skillCount; a++) {
                    for (int b = 0; b < resume.skillCount; b++) {
                        if (jobPtr->skills[a] == resume.skills[b]) {
                            matchedSkills++;
                            break;
                        }
                    }
                }
                
                double score = (static_cast<double>(matchedSkills) / jobPtr->skillCount) * 100.0;

                results->append(MatchResult(jobPtr->id, resume.id, score));
            }
        }
        
        if (resumes) delete[] resumes;
    }

    cout << "Matching complete. Found " << results->getLength() << " matches." << endl;
    return results;
}

MatchResultList* JobMatching::weightedScoringMatch(const string* skillSet, int skillCount, bool matchAll) {
    delete results;
    results = new MatchResultList();

    cout << "\n===== Weighted Scoring Match (Optimized) =====" << endl;
    bool findJobMode = (matchMode == FIND_JOB);

    string keyword;
    int jobId;
    double minScore = 30.0;

    if (findJobMode) {
        cout << "Enter position keyword to filter jobs: ";
        cin >> ws;
        getline(cin, keyword);
    } else {
        cout << "Enter Job ID to find matching resumes: ";
        cin >> jobId;
    }

    cout << "Enter minimum match score (0-100, default 30): ";
    cin >> minScore;
    if (cin.fail() || minScore < 0 || minScore > 100) {
        cout << "Invalid input. Using default threshold: 30%" << endl;
        minScore = 30.0;
        cin.clear();
        cin.ignore(1000, '\n');
    }

    SkillWeightList* weightList = promptSkillWeights(skillSet, skillCount);

    auto addMatchUnique = [this](int jobId, int resumeId, double score) {
        MatchResultNode* node = results->getHead();
        while (node) {
            if (node->data.jobId == jobId && node->data.resumeId == resumeId)
                return; // Skip duplicate
            node = node->next;
        }
        results->append(MatchResult(jobId, resumeId, score));
    };

    if (findJobMode) {
        // Step 1: Search and convert jobs to array
        void* searchResult = searchJobsByPosition(keyword);
        int jobCount = 0;
        Job* jobs = convertJobsToArray(searchResult, jobCount);

        if (dataStruct == ARRAY) delete (JobArray*)searchResult;
        else if (dataStruct == SINGLY_LINKED_LIST) delete (JobLinkedList*)searchResult;
        else if (dataStruct == CIRCULAR_LINKED_LIST) delete (JobCircularLinkedList*)searchResult;

        if (!jobs || jobCount == 0) {
            cout << "No jobs found matching keyword: " << keyword << endl;
            if (jobs) delete[] jobs;
            delete weightList;
            return results;
        }

        // Step 2: Get all resumes
        int resumeCount = 0;
        Resume* resumes = getAllResumesAsArray(resumeCount);
        if (!resumes || resumeCount == 0) {
            cout << "No resumes found." << endl;
            delete[] jobs;
            if (resumes) delete[] resumes;
            delete weightList;
            return results;
        }

        // Step 3: Sort each job’s skills once
        string** sortedJobSkills = new string*[jobCount];
        for (int i = 0; i < jobCount; i++) {
            sortedJobSkills[i] = new string[jobs[i].skillCount];
            cout << "[DEBUG] Job " << jobs[i].id << " has " << jobs[i].skillCount << " skills:" << endl;
            for (int k = 0; k < jobs[i].skillCount; k++)
                sortedJobSkills[i][k] = jobs[i].skills[k];
            sort(sortedJobSkills[i], sortedJobSkills[i] + jobs[i].skillCount);
        }

        // Step 4: Calculate weighted scores
        for (int i = 0; i < jobCount; i++) {
            Job jobOrig = jobs[i];

            for (int j = 0; j < resumeCount; j++) {
                Resume resumeOrig = resumes[j];

                string* resumeSkillsCopy = new string[resumeOrig.skillCount];
                for (int k = 0; k < resumeOrig.skillCount; k++)
                    resumeSkillsCopy[k] = resumeOrig.skills[k];
                sort(resumeSkillsCopy, resumeSkillsCopy + resumeOrig.skillCount);

                double score = calculateWeightedScore(
                    sortedJobSkills[i], jobOrig.skillCount,
                    resumeSkillsCopy, resumeOrig.skillCount,
                    *weightList
                );

                delete[] resumeSkillsCopy;

                if (score >= minScore) {
                    addMatchUnique(jobOrig.id, resumeOrig.id, score);
                }
            }
        }

        // Cleanup
        for (int i = 0; i < jobCount; i++)
            delete[] sortedJobSkills[i];
        delete[] sortedJobSkills;
        delete[] jobs;
        delete[] resumes;

    } else {
        // FIND_RESUME mode
        Job* jobPtr = nullptr;
        switch (dataStruct) {
            case ARRAY: jobPtr = jobArray->findById(jobId); break;
            case SINGLY_LINKED_LIST: jobPtr = jobLinkedList->findById(jobId); break;
            case CIRCULAR_LINKED_LIST: jobPtr = jobCircular->findById(jobId); break;
        }

        if (!jobPtr) {
            cout << "Job not found.\n";
            delete weightList;
            return results;
        }

        string* sortedJobSkills = new string[jobPtr->skillCount];
        for (int k = 0; k < jobPtr->skillCount; k++)
            sortedJobSkills[k] = jobPtr->skills[k];
        sort(sortedJobSkills, sortedJobSkills + jobPtr->skillCount);

        int resumeCount = 0;
        Resume* resumes = getAllResumesAsArray(resumeCount);

        if (resumes && resumeCount > 0) {
            for (int j = 0; j < resumeCount; j++) {
                Resume resumeOrig = resumes[j];

                string* resumeSkillsCopy = new string[resumeOrig.skillCount];
                for (int k = 0; k < resumeOrig.skillCount; k++)
                    resumeSkillsCopy[k] = resumeOrig.skills[k];
                sort(resumeSkillsCopy, resumeSkillsCopy + resumeOrig.skillCount);

                double score = calculateWeightedScore(
                    sortedJobSkills, jobPtr->skillCount,
                    resumeSkillsCopy, resumeOrig.skillCount,
                    *weightList
                );

                delete[] resumeSkillsCopy;

                if (score >= minScore) {
                    addMatchUnique(jobPtr->id, resumeOrig.id, score);
                }
            }
        }

        delete[] sortedJobSkills;
        if (resumes) delete[] resumes;
    }

    cout << "Matching complete. Found " << results->getLength() 
         << " matches above " << minScore << "% threshold." << endl;

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
