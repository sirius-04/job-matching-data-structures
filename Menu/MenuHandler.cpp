#include "MenuHandler.hpp"
#include <limits>
#include "utils/CSVLoader/CSVLoader.hpp"
#include "models/PerformanceTracker/PerformanceTracker.hpp"
#include "JobMatching/JobMatching.hpp"
using namespace std;

// common method

int getUserChoice(int min, int max)
{
    int choice;
    while (true)
    {
        cout << "Enter choice: ";
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number." << endl;
        }
        else if (choice < min || choice > max)
        {
            cout << "Please enter a valid option!" << endl;
        }
        else
            return choice;
    }
}

int selectSearchAlgorithm()
{
    cout << "\n--- SELECT SEARCHING ALGORITHM ---" << endl;
    cout << "[1] Linear Search" << endl;
    cout << "[2] Binary Search" << endl;
    cout << "[3] Back" << endl;
    return getUserChoice(1, 3);
}

int selectSortAlgorithm()
{
    cout << "\n--- SELECT SORTING ALGORITHM ---" << endl;
    cout << "[1] Merge Sort" << endl;
    cout << "[2] Quick Sort" << endl;
    cout << "[3] Back" << endl;
    return getUserChoice(1, 3);
}

// Array Menu

int displayMainMenu(const string &mode)
{
    cout << "\n=== " << mode << " MODE ===" << endl;
    cout << "\nSelect operation: " << endl;
    cout << "[1] List data" << endl;
    cout << "[2] Search" << endl;
    cout << "[3] Sort" << endl;
    cout << "[4] Match jobs" << endl;
    cout << "[5] Back to main menu" << endl;
    return getUserChoice(1, 5);
}

int displaySearchMenu(const string &mode)
{
    cout << "\n--- SEARCH MENU (" << mode << ") ---" << endl;
    cout << "[1] Search Jobs" << endl;
    cout << "[2] Search Resumes" << endl;
    cout << "[3] Back" << endl;
    return getUserChoice(1, 3);
}

int displaySortMenu(const string &mode)
{
    cout << "\n--- SORT MENU (" << mode << ") ---" << endl;
    cout << "[1] Sort Jobs" << endl;
    cout << "[2] Sort Resumes" << endl;
    cout << "[3] Back" << endl;
    return getUserChoice(1, 3);
}

template <typename SearchFunc>
void performKeywordSearch(SearchFunc searchFunc, const string &label)
{
    string keywords[2];
    char proceed;

    cout << "[1] Enter " << label << " keyword 1: ";
    cin.ignore();
    getline(cin, keywords[0]);

    cout << "[2] Enter " << label << " keyword 2: ";
    getline(cin, keywords[1]);

    cout << "(Enter y to proceed; n to back): ";
    cin >> proceed;
    if (proceed == 'n' || proceed == 'N')
        return;

    searchFunc(keywords);
}

void handleMatch(JobMatching* matcher, DataStruct dataStruct)
{
    cout << "\n========== MATCHING CONFIGURATION ==========" << endl;

    // --- Select Match Mode ---
    cout << "\n--- SELECT MATCH MODE ---" << endl;
    cout << "[1] Find Job (Resumes -> Jobs)" << endl;
    cout << "[2] Find Resume (Jobs -> Resumes)" << endl;
    cout << "[3] Back" << endl;

    int modeChoice = getUserChoice(1, 3);
    if (modeChoice == 3)
        return;

    MatchMode matchMode = (modeChoice == 1) ? FIND_JOB : FIND_RESUME;
    matcher->setMatchMode(matchMode);

    // --- Select Match Strategy ---
    cout << "\n--- SELECT MATCH STRATEGY ---" << endl;
    cout << "[1] Keyword-Based Matching" << endl;
    cout << "[2] Weighted Scoring Matching" << endl;
    cout << "[3] Back" << endl;

    int strategyChoice = getUserChoice(1, 3);
    if (strategyChoice == 3)
        return;

    MatchStrategy matchStrategy = (strategyChoice == 1) ? KEYWORD_BASED : WEIGHTED_SCORING;
    matcher->setMatchStrategy(matchStrategy);

    // --- Select Search Algorithm ---
    int algoChoice = selectSearchAlgorithm();
    if (algoChoice == 3)
        return;

    SearchAlgorithm searchAlgo = (algoChoice == 1) ? LINEAR : BINARY;
    matcher->setSearchAlgorithm(searchAlgo);

    // --- Data Structure ---
    matcher->setDataStruct(dataStruct);

    cout << "\n--- Enter Skills for Matching ---" << endl;
    int skillCount;
    cout << "Enter number of skills: ";
    cin >> skillCount;
    if (cin.fail() || skillCount <= 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid skill count.\n";
        return;
    }

    string* skills = new string[skillCount];
    for (int i = 0; i < skillCount; i++)
    {
        cout << "Enter skill " << (i + 1) << ": ";
        cin >> ws;
        getline(cin, skills[i]);
    }

    bool matchAll;
    cout << "\nRequire all skills to match? (1 = Yes, 0 = No): ";
    cin >> matchAll;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        matchAll = false;
    }

    // --- Run Matching ---
    MatchResultList* results = matcher->runMatching(skills, skillCount, matchAll);

    // --- Print Results ---
    cout << "\n=====  MATCH RESULTS =====" << endl;
    results->printSlice();

    // --- Print Performance ---
    matcher->printPerformance();

    delete[] skills;
}

void handleListArray(JobArray &jobArray, ResumeArray &resumeArray)
{
    while (true)
    {
        cout << "\n--- LIST MENU (Array) ---\n";
        cout << "[1] List all jobs\n";
        cout << "[2] List all resumes\n";
        cout << "[3] Back\n";

        int choice = getUserChoice(1, 3);

        switch (choice)
        {
        case 1:
            cout << "\n--- JOBS ---\n";
            jobArray.printJobs();
            break;
        case 2:
            cout << "\n--- RESUMES ---\n";
            resumeArray.printResumes();
            break;
        case 3:
            return;
        }
    }
}

void handleSearchArray(JobArray &jobArray, ResumeArray &resumeArray)
{
    while (true)
    {
        int searchMenu = displaySearchMenu("ARRAY");
        if (searchMenu == 3)
            return;

        switch (searchMenu)
        {
        case 1: // Search Jobs
        {
            int jobSearch = 0;
            while (jobSearch != 3)
            {
                cout << "\n--- SEARCH JOBS (Array) ---\n";
                cout << "[1] Search by Position\n";
                cout << "[2] Search by Skills\n";
                cout << "[3] Back\n";
                jobSearch = getUserChoice(1, 3);
                if (jobSearch == 3)
                    break;

                int algo = selectSearchAlgorithm();
                if (algo == 3)
                    break;

                string keyword1, keyword2;
                cout << "Enter first position keyword: ";
                cin.ignore();
                getline(cin, keyword1);
                cout << "Enter second position keyword: ";
                getline(cin, keyword2);

                if (jobSearch == 1)
                {
                    cout << ((algo == 1) ? "Linear" : "Binary") << " Search by position\n";

                    // 🕒 Measure performance
                    PerformanceResult result = PerformanceTracker::measure([&]() {
                        JobArray *searchResult = (algo == 1)
                                                     ? jobArray.linearSearchByPosition(keyword1)
                                                     : jobArray.binarySearchByPosition(keyword1);

                        if (searchResult && searchResult->getSize() > 0)
                            searchResult->printJobs();
                        else
                            cout << "No matching jobs found.\n";

                        delete searchResult;
                    });

                    result.printPerformance("Search Job by Position");
                }
                else if (jobSearch == 2)
                {
                    const string skills[] = {keyword1, keyword2};

                    cout << ((algo == 1) ? "Linear" : "Binary") << " Search by skills\n";

                    // 🕒 Measure performance
                    PerformanceResult result = PerformanceTracker::measure([&]() {
                        JobArray *searchResult = (algo == 1)
                                                     ? jobArray.linearSearchBySkills(skills, 2, false)
                                                     : jobArray.binarySearchBySkills(skills, 2, false);

                        if (searchResult && searchResult->getSize() > 0)
                            searchResult->printJobs();
                        else
                            cout << "No matching jobs found.\n";

                        delete searchResult;
                    });

                    result.printPerformance("Search Job by Skills");
                }
            }
            break;
        }

        case 2: // Search Resumes
        {
            int algo = selectSearchAlgorithm();
            if (algo == 3)
                break;

            string skill1, skill2;
            cout << "Enter first skill keyword: ";
            cin.ignore();
            getline(cin, skill1);
            cout << "Enter second skill keyword: ";
            getline(cin, skill2);
            const string skills[] = {skill1, skill2};

            cout << ((algo == 1) ? "Linear" : "Binary") << " Search by skills\n";

            // 🕒 Measure performance
            PerformanceResult result = PerformanceTracker::measure([&]() {
                ResumeArray *searchResult = (algo == 1)
                                                ? resumeArray.linearSearchBySkills(skills, 2, false)
                                                : resumeArray.binarySearchBySkills(skills, 2, false);

                if (searchResult && searchResult->getSize() > 0)
                    searchResult->printResumes();
                else
                    cout << "No matching resumes found.\n";

                delete searchResult;
            });

            result.printPerformance("Search Resume by Skills");
            break;
        }
        }
    }
}

void handleSortArray(JobArray &jobArray, ResumeArray &resumeArray)
{
    while (true)
    {
        int sortMenu = displaySortMenu("ARRAY");
        if (sortMenu == 3)
            return;

        switch (sortMenu)
        {
        case 1: // Sort Jobs
        {
            cout << "\n--- SORT JOBS ---\n";
            cout << "[1] Sort By ID\n";
            cout << "[2] Sort By Position\n";
            cout << "[3] Sort By Skill\n";
            cout << "[4] Sort By Skill Count\n";
            cout << "[5] Back\n";
            int jobSort = getUserChoice(1, 5);
            if (jobSort == 5)
                break;

            int algo = selectSortAlgorithm();
            if (algo == 3)
                break;

            string label;

            // 🕒 Measure performance for the selected algorithm
            PerformanceResult result = PerformanceTracker::measure([&]() {
                if (algo == 1) // Merge Sort
                {
                    label = "Merge Sort";
                    switch (jobSort)
                    {
                    case 1:
                        jobArray.mergeSort(JobArray::compareById);
                        break;
                    case 2:
                        jobArray.mergeSort(JobArray::compareByPosition);
                        break;
                    case 3:
                        jobArray.mergeSort(JobArray::compareByFirstSkill);
                        break;
                    case 4:
                        jobArray.mergeSort(JobArray::compareBySkillCount);
                        break;
                    }
                }
                else // Quick Sort
                {
                    label = "Quick Sort";
                    switch (jobSort)
                    {
                    case 1:
                        jobArray.quickSortById();
                        break;
                    case 2:
                        jobArray.quickSortByPosition();
                        break;
                    case 3:
                        jobArray.quickSortBySkill();
                        break;
                    case 4:
                        jobArray.quickSortBySkillCount();
                        break;
                    }
                }
            });

            jobArray.printJobs();
            result.printPerformance(label + " (Jobs)");
            break;
        }

        case 2: // Sort Resumes
        {
            cout << "\n--- SORT RESUMES ---\n";
            cout << "[1] Sort By ID\n";
            cout << "[2] Sort By First Skill\n";
            cout << "[3] Sort By Skill Count\n";
            cout << "[4] Back\n";
            int resumeSort = getUserChoice(1, 4);
            if (resumeSort == 4)
                break;

            int algo = selectSortAlgorithm();
            if (algo == 3)
                break;

            string label;

            // 🕒 Measure performance for resume sorting
            PerformanceResult result = PerformanceTracker::measure([&]() {
                if (algo == 1)
                {
                    label = "Merge Sort";
                    switch (resumeSort)
                    {
                    case 1:
                        resumeArray.mergeSort(ResumeArray::compareById);
                        break;
                    case 2:
                        resumeArray.mergeSort(ResumeArray::compareByFirstSkill);
                        break;
                    case 3:
                        resumeArray.mergeSort(ResumeArray::compareBySkillCount);
                        break;
                    }
                }
                else
                {
                    label = "Quick Sort";
                    switch (resumeSort)
                    {
                    case 1:
                        resumeArray.quickSortById();
                        break;
                    case 2:
                        resumeArray.quickSortBySkill();
                        break;
                    case 3:
                        resumeArray.quickSortBySkillCount();
                        break;
                    }
                }
            });

            resumeArray.printResumes();
            result.printPerformance(label + " (Resumes)");
            break;
        }
        }
    }
}

void handleMatchArray(JobArray &jobArray, ResumeArray &resumeArray)
{
    cout << "\n--- MATCH MENU (Array) ---\n";
    JobMatching matcher(&jobArray, &resumeArray);
    handleMatch(&matcher, ARRAY);
}

void runArray(JobArray &jobArray, ResumeArray &resumeArray)
{
    createJobArray(jobArray);
    createResumeArray(resumeArray);

    while (true)
    {
        int choice = displayMainMenu("ARRAY");

        switch (choice)
        {
        case 1:
            handleListArray(jobArray, resumeArray);
            break;
        case 2:
            handleSearchArray(jobArray, resumeArray);
            break;
        case 3:
            handleSortArray(jobArray, resumeArray);
            break;
        case 4:
            handleMatchArray(jobArray, resumeArray);
            break;
        case 5:
            return;
        }
    }
}

// Linked List Menu

void handleListLinkedList(JobLinkedList &jobLinkedList, ResumeLinkedList &resumeLinkedList)
{
    cout << "\n--- Linked List Data ---" << endl;
    cout << "[1] List Jobs" << endl;
    cout << "[2] List Resumes" << endl;
    cout << "[3] Back" << endl;

    int listChoice = getUserChoice(1, 3);
    switch (listChoice)
    {
    case 1:
        cout << "\n--- JOBS ---\n";
        jobLinkedList.printSlice();
        break;
    case 2:
        cout << "\n--- RESUMES ---\n";
        resumeLinkedList.printSlice();
        break;
    case 3:
        return;
    }
}

void handleLinkedListSearch(JobLinkedList &jobLinkedList, ResumeLinkedList &resumeLinkedList)
{
    while (true)
    {
        int searchMenu = displaySearchMenu("LINKED LIST");
        if (searchMenu == 3)
            return;

        switch (searchMenu)
        {
        case 1: // Search Jobs
        {
            int jobSearch = 0;
            while (jobSearch != 3)
            {
                cout << "\n--- SEARCH JOBS (LinkedList) ---" << endl;
                cout << "[1] Search By Position" << endl;
                cout << "[2] Search By Skills" << endl;
                cout << "[3] Back" << endl;

                int choice = getUserChoice(1, 3);
                if (choice == 3)
                    return;

                int algo = selectSearchAlgorithm();
                if (algo == 3)
                    break;

                if (choice == 1)
                {
                    string keyword;
                    cout << "Enter job position keyword: ";
                    cin.ignore();
                    getline(cin, keyword);

                    cout << ((algo == 1) ? "\nLinear" : "\nBinary") << " Search by position\n";

                    // 🕒 Measure performance
                    PerformanceResult result = PerformanceTracker::measure([&]() {
                        JobLinkedList *searchResult = (algo == 1)
                                                          ? jobLinkedList.linearSearchJobByPosition(keyword)
                                                          : jobLinkedList.binarySearchJobByPosition(keyword);

                        if (searchResult && searchResult->getLength() > 0)
                            searchResult->printSlice();
                        else
                            cout << "No matching jobs found.\n";

                        delete searchResult;
                    });

                    result.printPerformance("Search Job by Position");
                }
                else if (choice == 2)
                {
                    string skills[2];
                    cout << "Enter skill 1: ";
                    cin.ignore();
                    getline(cin, skills[0]);
                    cout << "Enter skill 2: ";
                    getline(cin, skills[1]);

                    cout << ((algo == 1) ? "\nLinear" : "\nBinary") << " Search by skills\n";

                    // 🕒 Measure performance
                    PerformanceResult result = PerformanceTracker::measure([&]() {
                        JobLinkedList *searchResult = (algo == 1)
                                                          ? jobLinkedList.linearSearchJobBySkills(skills, 2, false)
                                                          : jobLinkedList.binarySearchJobBySkills(skills, 2, false);

                        if (searchResult && searchResult->getLength() > 0)
                            searchResult->printSlice();
                        else
                            cout << "No matching jobs found.\n";

                        delete searchResult;
                    });

                    result.printPerformance("Search Job by Skills");
                }
            }
            break;
        }

        case 2: // Search Resumes
        {
            int algo = selectSearchAlgorithm();
            if (algo == 3)
                break;

            string skills[2];
            cout << "Enter skill 1: ";
            cin.ignore();
            getline(cin, skills[0]);
            cout << "Enter skill 2: ";
            getline(cin, skills[1]);

            cout << ((algo == 1) ? "\nLinear" : "\nBinary") << " Search by skills\n";

            // 🕒 Measure performance
            PerformanceResult result = PerformanceTracker::measure([&]() {
                ResumeLinkedList *searchResult = (algo == 1)
                                                     ? resumeLinkedList.linearSearchResumeBySkills(skills, 2, false)
                                                     : resumeLinkedList.binarySearchResumeBySkills(skills, 2, false);

                if (searchResult && searchResult->getLength() > 0)
                    searchResult->printSlice();
                else
                    cout << "No matching resumes found.\n";

                delete searchResult;
            });

            result.printPerformance("Search Resume by Skills");
            break;
        }
        }
    }
}

void handleLinkedListSort(JobLinkedList &jobLinkedList, ResumeLinkedList &resumeLinkedList)
{
    while (true)
    {
        int sortMenu = displaySortMenu("LINKED LIST");
        if (sortMenu == 3)
            return;

        switch (sortMenu)
        {
        case 1: // Sort Jobs
        {
            cout << "\n--- SORT JOBS ---" << endl;
            cout << "[1] Sort By Position\n";
            cout << "[2] Sort By Skills\n";
            cout << "[3] Sort By Skill Count\n";
            cout << "[4] Back\n";
            int choice = getUserChoice(1, 4);
            if (choice == 4)
                return;

            int algo = selectSortAlgorithm();
            if (algo == 3)
                break;

            cout << ((algo == 1) ? "\nPerforming Merge Sort\n" : "\nPerforming Quick Sort\n");

            // 🕒 Measure performance
            PerformanceResult result = PerformanceTracker::measure([&]() {
                if (choice == 1)
                {
                    (algo == 1)
                        ? jobLinkedList.mergeSortBy("position")
                        : jobLinkedList.quickSortByPosition();
                }
                else if (choice == 2)
                {
                    (algo == 1)
                        ? jobLinkedList.mergeSortBy("skill")
                        : jobLinkedList.quickSortBySkill();
                }
                else
                {
                    (algo == 1)
                        ? jobLinkedList.mergeSortBy("skillCount")
                        : jobLinkedList.quickSortBySkillCount();
                }

                jobLinkedList.printSlice();
            });

            result.printPerformance("Sort Jobs (LinkedList)");
            break;
        }

        case 2: // Sort Resumes
        {
            cout << "\n--- SORT RESUMES ---" << endl;
            cout << "[1] Sort By Skills\n";
            cout << "[2] Sort By Skill Count\n";
            cout << "[3] Back\n";
            int choice = getUserChoice(1, 3);
            if (choice == 3)
                return;

            int algo = selectSortAlgorithm();
            if (algo == 3)
                break;

            cout << ((algo == 1) ? "\nPerforming Merge Sort\n" : "\nPerforming Quick Sort\n");

            // 🕒 Measure performance
            PerformanceResult result = PerformanceTracker::measure([&]() {
                if (choice == 1)
                {
                    (algo == 1)
                        ? resumeLinkedList.mergeSortBy("skill")
                        : resumeLinkedList.quickSortBySkill();
                }
                else
                {
                    (algo == 1)
                        ? resumeLinkedList.mergeSortBy("skillCount")
                        : resumeLinkedList.quickSortBySkillCount();
                }

                resumeLinkedList.printSlice();
            });

            result.printPerformance("Sort Resumes (LinkedList)");
            break;
        }
        }
    }
}

void handleLinkedListMatch(JobLinkedList &jobLinkedList, ResumeLinkedList &resumeLinkedList)
{
    cout << "\n--- MATCH MENU (Singly Linked List) ---\n";
    JobMatching matcher(&jobLinkedList, &resumeLinkedList);
    handleMatch(&matcher, SINGLY_LINKED_LIST);
}

void runLinkedList(JobLinkedList &jobLinkedList, ResumeLinkedList &resumeLinkedList)
{
    createJobLinkedList(jobLinkedList);
    createResumeLinkedList(resumeLinkedList);

    while (true)
    {
        int choice = displayMainMenu("LINKED LIST");
        switch (choice)
        {
        case 1:
            handleListLinkedList(jobLinkedList, resumeLinkedList);
            break;
        case 2:
            handleLinkedListSearch(jobLinkedList, resumeLinkedList);
            break;
        case 3:
            handleLinkedListSort(jobLinkedList, resumeLinkedList);
            break;
        case 4:
            handleLinkedListMatch(jobLinkedList, resumeLinkedList);
            break;
        case 5:
            return;
        }
    }
}

// Circular Linked List Menu

void handleCircularListLinkedList(JobCircularLinkedList &jobCircularLinkedList, ResumeCircularLinkedList &resumeCircularLinkedList)
{
    cout << "\n--- Circular Linked List Data ---" << endl;
    cout << "[1] List Jobs" << endl;
    cout << "[2] List Resumes" << endl;
    cout << "[3] Back" << endl;

    int listChoice = getUserChoice(1, 3);
    switch (listChoice)
    {
    case 1:
        cout << "\n--- JOBS ---\n";
        jobCircularLinkedList.printSlice();
        break;
    case 2:
        cout << "\n--- RESUMES ---\n";
        resumeCircularLinkedList.printSlice();
        break;
    case 3:
        return;
    }
}

void handleCircularLinkedListSearch(JobCircularLinkedList &jobCircularLinkedList, ResumeCircularLinkedList &resumeCircularLinkedList)
{
    while (true)
    {
        int searchMenu = displaySearchMenu("CIRCULAR LINKED LIST");
        if (searchMenu == 3)
            return;

        switch (searchMenu)
        {
        case 1: // Search Jobs
        {
            int jobSearch = 0;
            while (jobSearch != 3)
            {
                cout << "\n--- SEARCH JOBS (CircularLinkedList) ---" << endl;
                cout << "[1] Search By Position" << endl;
                cout << "[2] Search By Skills" << endl;
                cout << "[3] Back" << endl;

                int choice = getUserChoice(1, 3);
                if (choice == 3)
                    return;

                int algo = selectSearchAlgorithm();
                if (algo == 3)
                    break;

                if (choice == 1)
                {
                    string keyword;
                    cout << "Enter job position keyword: ";
                    cin.ignore();
                    getline(cin, keyword);

                    cout << ((algo == 1) ? "\nLinear Search..." : "\nBinary Search...") << endl;

                    // 🕒 Measure performance
                    PerformanceResult result = PerformanceTracker::measure([&]() {
                        JobCircularLinkedList *searchResult = (algo == 1)
                                                                 ? jobCircularLinkedList.linearSearchJobByPosition(keyword)
                                                                 : jobCircularLinkedList.binarySearchJobByPosition(keyword);

                        if (searchResult)
                            searchResult->printSlice();
                        else
                            cout << "No jobs found.\n";

                        delete searchResult;
                    });

                    result.printPerformance("Search Job by Position (CircularLinkedList)");
                }
                else if (choice == 2)
                {
                    string skills[2];
                    cout << "Enter skill 1: ";
                    cin.ignore();
                    getline(cin, skills[0]);
                    cout << "Enter skill 2: ";
                    getline(cin, skills[1]);

                    cout << ((algo == 1) ? "\nLinear Search..." : "\nBinary Search...") << endl;

                    // 🕒 Measure performance
                    PerformanceResult result = PerformanceTracker::measure([&]() {
                        JobCircularLinkedList *searchResult = (algo == 1)
                                                                 ? jobCircularLinkedList.linearSearchJobBySkills(skills, 2, false)
                                                                 : jobCircularLinkedList.binarySearchJobBySkills(skills, 2, false);

                        if (searchResult)
                            searchResult->printSlice();
                        else
                            cout << "No jobs found.\n";

                        delete searchResult;
                    });

                    result.printPerformance("Search Job by Skills (CircularLinkedList)");
                }
            }
            break;
        }

        case 2: // Search Resumes
        {
            int algo = selectSearchAlgorithm();
            if (algo == 3)
                break;

            string skills[2];
            cout << "Enter skill 1: ";
            cin.ignore();
            getline(cin, skills[0]);
            cout << "Enter skill 2: ";
            getline(cin, skills[1]);

            cout << ((algo == 1) ? "\nLinear Search..." : "\nBinary Search...") << endl;

            // 🕒 Measure performance
            PerformanceResult result = PerformanceTracker::measure([&]() {
                ResumeCircularLinkedList *searchResult = (algo == 1)
                                                             ? resumeCircularLinkedList.linearSearchResumeBySkills(skills, 2, false)
                                                             : resumeCircularLinkedList.binarySearchResumeBySkills(skills, 2, false);

                if (searchResult)
                    searchResult->printSlice();
                else
                    cout << "No resume found.\n";

                delete searchResult;
            });

            result.printPerformance("Search Resume by Skills (CircularLinkedList)");
            break;
        }
        }
    }
}

void handleCircularLinkedListSort(JobCircularLinkedList &jobCircularLinkedList, ResumeCircularLinkedList &resumeCircularLinkedList)
{
    while (true)
    {
        int sortMenu = displaySortMenu("CIRCULAR LINKED LIST");
        if (sortMenu == 3)
            return;

        switch (sortMenu)
        {
        case 1: // Sort Jobs
        {
            cout << "\n--- SORT JOBS (CircularLinkedList) ---" << endl;
            cout << "[1] Sort By Position\n";
            cout << "[2] Sort By Skills\n";
            cout << "[3] Sort By Skill Count\n";
            cout << "[4] Back\n";
            int choice = getUserChoice(1, 4);
            if (choice == 4)
                return;

            int algo = selectSortAlgorithm();
            if (algo == 3)
                break;

            cout << ((algo == 1) ? "\nPerforming Merge Sort\n" : "\nPerforming Quick Sort\n");

            // 🕒 Measure performance
            PerformanceResult result = PerformanceTracker::measure([&]() {
                if (choice == 1)
                {
                    (algo == 1)
                        ? jobCircularLinkedList.mergeSortBy("position")
                        : jobCircularLinkedList.quickSortByPosition();
                }
                else if (choice == 2)
                {
                    (algo == 1)
                        ? jobCircularLinkedList.mergeSortBy("skill")
                        : jobCircularLinkedList.quickSortBySkill();
                }
                else
                {
                    (algo == 1)
                        ? jobCircularLinkedList.mergeSortBy("skillCount")
                        : jobCircularLinkedList.quickSortBySkillCount();
                }

                jobCircularLinkedList.printSlice();
            });

            result.printPerformance("Sort Jobs (CircularLinkedList)");
            break;
        }

        case 2: // Sort Resumes
        {
            cout << "\n--- SORT RESUMES (CircularLinkedList) ---" << endl;
            cout << "[1] Sort By Skills\n";
            cout << "[2] Sort By Skill Count\n";
            cout << "[3] Back\n";
            int choice = getUserChoice(1, 3);
            if (choice == 3)
                return;

            int algo = selectSortAlgorithm();
            if (algo == 3)
                break;

            cout << ((algo == 1) ? "\nPerforming Merge Sort\n" : "\nPerforming Quick Sort\n");

            // 🕒 Measure performance
            PerformanceResult result = PerformanceTracker::measure([&]() {
                if (choice == 1)
                {
                    (algo == 1)
                        ? resumeCircularLinkedList.mergeSortBy("skill")
                        : resumeCircularLinkedList.quickSortBySkill();
                }
                else
                {
                    (algo == 1)
                        ? resumeCircularLinkedList.mergeSortBy("skillCount")
                        : resumeCircularLinkedList.quickSortBySkillCount();
                }

                resumeCircularLinkedList.printSlice();
            });

            result.printPerformance("Sort Resumes (CircularLinkedList)");
            break;
        }
        }
    }
}

void handleCircularLinkedListMatch(JobCircularLinkedList &jobCircularLinkedList, ResumeCircularLinkedList &resumeCircularLinkedList)
{
    cout << "\n--- MATCH MENU (Circular Linked List) ---\n";
    JobMatching matcher(&jobCircularLinkedList, &resumeCircularLinkedList);
    handleMatch(&matcher, CIRCULAR_LINKED_LIST);
}

void runCircularLinkedList(JobCircularLinkedList &jobCircularLinkedList, ResumeCircularLinkedList &resumeCircularLinkedList)
{
    createJobCircularLinkedList(jobCircularLinkedList);
    createResumeCircularLinkedList(resumeCircularLinkedList);

    while (true)
    {
        int choice = displayMainMenu("CIRCULAR LINKED LIST");
        switch (choice)
        {
        case 1:
            handleCircularListLinkedList(jobCircularLinkedList, resumeCircularLinkedList);
            break;
        case 2:
            handleCircularLinkedListSearch(jobCircularLinkedList, resumeCircularLinkedList);
            break;
        case 3:
            handleCircularLinkedListSort(jobCircularLinkedList, resumeCircularLinkedList);
            break;
        case 4:
            handleCircularLinkedListMatch(jobCircularLinkedList, resumeCircularLinkedList);
            break;
        case 5:
            return;
        }
    }
}
