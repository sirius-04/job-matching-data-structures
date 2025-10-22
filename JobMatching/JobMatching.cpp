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

template<typename Func>
void JobMatching::processJobs(void* jobData, Func callback) {
    if (!jobData) return;
    
    switch (dataStruct) {
        case ARRAY: {
            JobArray* arr = (JobArray*)jobData;
            for (int i = 0; i < arr->getSize(); i++) {
                callback(arr->getJob(i));
            }
            break;
        }
        case SINGLY_LINKED_LIST: {
            JobLinkedList* list = (JobLinkedList*)jobData;
            JobNode* node = list->getHead();
            while (node) {
                callback(node->data);
                node = node->next;
            }
            break;
        }
        case CIRCULAR_LINKED_LIST: {
            JobCircularLinkedList* circ = (JobCircularLinkedList*)jobData;
            if (!circ->getHead()) break;
            JobNode* start = circ->getHead();
            JobNode* node = start;
            do {
                callback(node->data);
                node = node->next;
            } while (node != start);
            break;
        }
    }
}

// Generic resume processor - works with any data structure
template<typename Func>
void JobMatching::processResumes(void* resumeData, Func callback) {
    if (!resumeData) return;
    
    switch (dataStruct) {
        case ARRAY: {
            ResumeArray* arr = (ResumeArray*)resumeData;
            for (int i = 0; i < arr->getSize(); i++) {
                callback(arr->getResume(i));
            }
            break;
        }
        case SINGLY_LINKED_LIST: {
            ResumeLinkedList* list = (ResumeLinkedList*)resumeData;
            ResumeNode* node = list->getHead();
            while (node) {
                callback(node->data);
                node = node->next;
            }
            break;
        }
        case CIRCULAR_LINKED_LIST: {
            ResumeCircularLinkedList* circ = (ResumeCircularLinkedList*)resumeData;
            if (!circ->getHead()) break;
            ResumeNode* start = circ->getHead();
            ResumeNode* node = start;
            do {
                callback(node->data);
                node = node->next;
            } while (node != start);
            break;
        }
    }
}

// Get all resumes without conversion
void* JobMatching::getAllResumes() {
    switch (dataStruct) {
        case ARRAY: return (void*)resumeArray;
        case SINGLY_LINKED_LIST: return (void*)resumeLinkedList;
        case CIRCULAR_LINKED_LIST: return (void*)resumeCircular;
        default: return nullptr;
    }
}

// Clean up search results properly
void JobMatching::cleanupJobSearchResult(void* searchResult) {
    if (!searchResult) return;
    switch (dataStruct) {
        case ARRAY: delete (JobArray*)searchResult; break;
        case SINGLY_LINKED_LIST: delete (JobLinkedList*)searchResult; break;
        case CIRCULAR_LINKED_LIST: delete (JobCircularLinkedList*)searchResult; break;
    }
}

void JobMatching::cleanupResumeSearchResult(void* searchResult) {
    if (!searchResult) return;
    switch (dataStruct) {
        case ARRAY: delete (ResumeArray*)searchResult; break;
        case SINGLY_LINKED_LIST: delete (ResumeLinkedList*)searchResult; break;
        case CIRCULAR_LINKED_LIST: delete (ResumeCircularLinkedList*)searchResult; break;
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

void JobMatching::addUniqueMatch(int jobId, int resumeId, double score) {
    MatchResultNode* node = results->getHead();
    while (node) {
        if (node->data.jobId == jobId && node->data.resumeId == resumeId)
            return;
        node = node->next;
    }
    results->append(MatchResult(jobId, resumeId, score));
}

MatchResultList* JobMatching::ruleBasedMatch(const string* skillSet, int skillCount, bool matchAll) {
    delete results;
    results = new MatchResultList();

    cout << "\n===== Running Rule-Based Match (No Conversion) =====" << endl;
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
        // FIND_JOB mode: Search jobs, then for each job find matching resumes
        void* jobSearchResult = searchJobsByPosition(keyword);
        
        if (!jobSearchResult) {
            cout << "No jobs found matching keyword: " << keyword << endl;
            return results;
        }
        
        // Process each job directly without conversion
        processJobs(jobSearchResult, [&](const Job& job) {
            // Search for matching resumes for this job
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
            
            // Process each matching resume directly
            processResumes(resumeSearchResult, [&](const Resume& resume) {
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
                addUniqueMatch(job.id, resume.id, score);  // Changed from results->append
            });
            
            cleanupResumeSearchResult(resumeSearchResult);
        });
        
        cleanupJobSearchResult(jobSearchResult);
        
    } else {
        // FIND_RESUME mode: Find one job, then search matching resumes
        Job* jobPtr = nullptr;
        switch (dataStruct) {
            case ARRAY: jobPtr = jobArray->findById(jobId); break;
            case SINGLY_LINKED_LIST: jobPtr = jobLinkedList->findById(jobId); break;
            case CIRCULAR_LINKED_LIST: jobPtr = jobCircular->findById(jobId); break;
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
        
        // Process resumes directly
        processResumes(resumeSearchResult, [&](const Resume& resume) {
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
            addUniqueMatch(jobPtr->id, resume.id, score);  // Changed from results->append
        });
        
        cleanupResumeSearchResult(resumeSearchResult);
    }

    cout << "Matching complete. Found " << results->getLength() << " matches." << endl;
    return results;
}

MatchResultList* JobMatching::weightedScoringMatch(const string* skillSet, int skillCount, bool matchAll) {
    delete results;
    results = new MatchResultList();

    cout << "\n===== Weighted Scoring Match (No Conversion) =====" << endl;
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

    if (findJobMode) {
        // FIND_JOB mode
        void* jobSearchResult = searchJobsByPosition(keyword);
        
        if (!jobSearchResult) {
            cout << "No jobs found matching keyword: " << keyword << endl;
            delete weightList;
            return results;
        }
        
        void* allResumes = getAllResumes();
        
        // Process each job
        processJobs(jobSearchResult, [&](const Job& job) {
            // Sort job skills once per job
            string* sortedJobSkills = new string[job.skillCount];
            for (int k = 0; k < job.skillCount; k++)
                sortedJobSkills[k] = job.skills[k];
            sort(sortedJobSkills, sortedJobSkills + job.skillCount);
            
            // Process all resumes for this job
            processResumes(allResumes, [&](const Resume& resume) {
                // Sort resume skills
                string* sortedResumeSkills = new string[resume.skillCount];
                for (int k = 0; k < resume.skillCount; k++)
                    sortedResumeSkills[k] = resume.skills[k];
                sort(sortedResumeSkills, sortedResumeSkills + resume.skillCount);
                
                double score = calculateWeightedScore(
                    sortedJobSkills, job.skillCount,
                    sortedResumeSkills, resume.skillCount,
                    *weightList
                );
                
                delete[] sortedResumeSkills;
                
                if (score >= minScore) {
                    addUniqueMatch(job.id, resume.id, score);  // Changed from lambda
                }
            });
            
            delete[] sortedJobSkills;
        });
        
        cleanupJobSearchResult(jobSearchResult);
        
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
        
        // Sort job skills once
        string* sortedJobSkills = new string[jobPtr->skillCount];
        for (int k = 0; k < jobPtr->skillCount; k++)
            sortedJobSkills[k] = jobPtr->skills[k];
        sort(sortedJobSkills, sortedJobSkills + jobPtr->skillCount);
        
        void* allResumes = getAllResumes();
        
        // Process all resumes
        processResumes(allResumes, [&](const Resume& resume) {
            string* sortedResumeSkills = new string[resume.skillCount];
            for (int k = 0; k < resume.skillCount; k++)
                sortedResumeSkills[k] = resume.skills[k];
            sort(sortedResumeSkills, sortedResumeSkills + resume.skillCount);
            
            double score = calculateWeightedScore(
                sortedJobSkills, jobPtr->skillCount,
                sortedResumeSkills, resume.skillCount,
                *weightList
            );
            
            delete[] sortedResumeSkills;
            
            if (score >= minScore) {
                addUniqueMatch(jobPtr->id, resume.id, score);  // Changed from lambda
            }
        });
        
        delete[] sortedJobSkills;
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
