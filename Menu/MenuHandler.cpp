#include "MenuHandler.hpp"
#include <limits>
#include "utils/CSVLoader/CSVLoader.hpp"
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
                    JobArray *result = (algo == 1)
                                           ? jobArray.linearSearchByPosition(keyword1)
                                           : jobArray.binarySearchByPosition(keyword1);

                    if (result && result->getSize() > 0)
                        result->printJobs();
                    else
                        cout << "No matching jobs found.\n";

                    delete result;
                }
                else if (jobSearch == 2)
                {
                    const string skills[] = {keyword1, keyword2};
                    JobArray *result = (algo == 1)
                                           ? jobArray.linearSearchBySkills(skills, 2, false)
                                           : jobArray.binarySearchBySkills(skills, 2, false);

                    if (result && result->getSize() > 0)
                        result->printJobs();
                    else
                        cout << "No matching jobs found.\n";

                    delete result;
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
            ResumeArray *result = (algo == 1)
                                      ? resumeArray.linearSearchBySkills(skills, 2, false)
                                      : resumeArray.binarySearchBySkills(skills, 2, false);

            if (result && result->getSize() > 0)
                result->printResumes();
            else
                cout << "No matching resumes found.\n";

            delete result;
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

            if (algo == 1) // Merge Sort
            {
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

            jobArray.printJobs();
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

            if (algo == 1)
            {
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

            resumeArray.printResumes();
            break;
        }
        }
    }
}

void handleMatchArray(JobArray &jobArray, ResumeArray &resumeArray)
{
    while (true)
    {
        cout << "\n--- MATCH MENU (Array) ---\n";
        cout << "[1] Match resumes by job\n";
        cout << "[2] Match jobs by resume\n";
        cout << "[3] Match all\n";
        cout << "[4] Back\n";
        int choice = getUserChoice(1, 4);

        switch (choice)
        {
        case 1:
            cout << "Matching resumes by job\n";
            break;
        case 2:
            cout << "Matching jobs by resume\n";
            break;
        case 3:
            cout << "Matching all...\n";
            break;
        case 4:
            return;
        }
    }
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

void handleListMenu(JobLinkedList &jobLinkedList, ResumeLinkedList &resumeLinkedList)
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

void handleJobSearch(JobLinkedList &jobLinkedList)
{
    cout << "\n--- SEARCH JOBS (LinkedList) ---" << endl;
    cout << "[1] Search By Position" << endl;
    cout << "[2] Search By Skills" << endl;
    cout << "[3] Back" << endl;

    int choice = getUserChoice(1, 3);
    if (choice == 3)
        return;

    int algo = selectSearchAlgorithm(); // You already have this function

    if (choice == 1)
    {
        string keyword;
        cout << "Enter job position keyword: ";
        cin.ignore();
        getline(cin, keyword);

        cout << ((algo == 1) ? "\nLinear Search..." : "\nBinary Search...") << endl;
        JobLinkedList *result = (algo == 1)
                                    ? jobLinkedList.linearSearchJobByPosition(keyword)
                                    : jobLinkedList.binarySearchJobByPosition(keyword);
        if (result)
            result->printSlice();
        else
            cout << "No jobs found.\n";
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
        JobLinkedList *result = (algo == 1)
                                    ? jobLinkedList.linearSearchJobBySkills(skills, 2, false)
                                    : jobLinkedList.binarySearchJobBySkills(skills, 2, false);
        if (result)
            result->printSlice();
        else
            cout << "No jobs found.\n";
    }
}

void handleResumeSearch(ResumeLinkedList &resumeLinkedList)
{
    cout << "\n--- SEARCH RESUMES (LinkedList) ---" << endl;
    cout << "[1] Search By Skills" << endl;
    cout << "[2] Back" << endl;

    int choice = getUserChoice(1, 2);
    if (choice == 2)
        return;

    int algo = selectSearchAlgorithm();

    string skills[2];
    cout << "Enter skill 1: ";
    cin.ignore();
    getline(cin, skills[0]);
    cout << "Enter skill 2: ";
    getline(cin, skills[1]);

    ResumeLinkedList *result = (algo == 1)
                                   ? resumeLinkedList.linearSearchResumeBySkills(skills, 2, false)
                                   : resumeLinkedList.binarySearchResumeBySkills(skills, 2, false);

    if (result)
        result->printSlice();
    else
        cout << "No resume found.\n";
}

void handleJobSort(JobLinkedList &jobLinkedList)
{
    cout << "\n--- SORT JOBS ---" << endl;
    cout << "[1] By Position\n[2] By Skills\n[3] By Skill Count\n[4] Back\n";
    int choice = getUserChoice(1, 4);
    if (choice == 4)
        return;

    int algo = selectSortAlgorithm();

    if (algo == 1)
        cout << "\nPerforming Merge Sort\n";
    else
        cout << "\nPerforming Quick Sort\n";

    if (choice == 1)
        (algo == 1) ? jobLinkedList.mergeSortBy("position") : jobLinkedList.quickSortByPosition();
    else if (choice == 2)
        (algo == 1) ? jobLinkedList.mergeSortBy("skill") : jobLinkedList.quickSortBySkill();
    else
        (algo == 1) ? jobLinkedList.mergeSortBy("skillCount") : jobLinkedList.quickSortBySkillCount();

    jobLinkedList.printSlice();
}

void handleResumeSort(ResumeLinkedList &resumeLinkedList)
{
    cout << "\n--- SORT RESUMES ---" << endl;
    cout << "[1] By Skills\n[2] By Skill Count\n[3] Back\n";
    int choice = getUserChoice(1, 3);
    if (choice == 3)
        return;

    int algo = selectSortAlgorithm();

    if (choice == 1)
        (algo == 1) ? resumeLinkedList.mergeSortBy("skill") : resumeLinkedList.quickSortBySkill();
    else
        (algo == 1) ? resumeLinkedList.mergeSortBy("skillCount") : resumeLinkedList.quickSortBySkillCount();

    resumeLinkedList.printSlice();
}

void handleMatchMenu(JobLinkedList &jobLinkedList, ResumeLinkedList &resumeLinkedList)
{
    bool inMatchMenu = true;
    while (inMatchMenu)
    {
        cout << "\n--- JOB MATCHING (LinkedList) ---" << endl;
        cout << "[1] Match resumes by job" << endl;
        cout << "[2] Match jobs by resume" << endl;
        cout << "[3] Match all" << endl;
        cout << "[4] Back" << endl;

        int matchChoice = getUserChoice(1, 4);
        switch (matchChoice)
        {
        case 1:
            cout << "Matching resumes by job...\n";
            break;
        case 2:
            cout << "Matching jobs by resume...\n";
            break;
        case 3:
            cout << "Matching all...\n";
            break;
        case 4:
            inMatchMenu = false;
            break;
        }
    }
}

void handleSearchMenu(JobLinkedList &jobLinkedList, ResumeLinkedList &resumeLinkedList)
{
    bool inSearchMenu = true;
    while (inSearchMenu)
    {
        cout << "\n--- SEARCH MENU (LinkedList) ---" << endl;
        cout << "[1] Search Jobs" << endl;
        cout << "[2] Search Resumes" << endl;
        cout << "[3] Back" << endl;

        int searchChoice = getUserChoice(1, 3);
        switch (searchChoice)
        {
        case 1:
            handleJobSearch(jobLinkedList);
            break;
        case 2:
            handleResumeSearch(resumeLinkedList);
            break;
        case 3:
            inSearchMenu = false;
            break;
        }
    }
}

void handleSortMenu(JobLinkedList &jobLinkedList, ResumeLinkedList &resumeLinkedList)
{
    bool inSortMenu = true;
    while (inSortMenu)
    {
        cout << "\n--- SORT MENU (LinkedList) ---" << endl;
        cout << "[1] Sort Jobs" << endl;
        cout << "[2] Sort Resumes" << endl;
        cout << "[3] Back" << endl;

        int choice = getUserChoice(1, 3);
        switch (choice)
        {
        case 1:
            handleJobSort(jobLinkedList);
            break;
        case 2:
            handleResumeSort(resumeLinkedList);
            break;
        case 3:
            inSortMenu = false;
            break;
        }
    }
}

void runLinkedList(JobLinkedList &jobLinkedList, ResumeLinkedList &resumeLinkedList)
{
    cout << "\n=== LINKED LIST MODE ===" << endl;
    bool running = true;

    while (running)
    {
        cout << "\nSelect operation: " << endl;
        cout << "[1] List data" << endl;
        cout << "[2] Search" << endl;
        cout << "[3] Sort" << endl;
        cout << "[4] Match jobs" << endl;
        cout << "[5] Back to main menu" << endl;

        int choice = getUserChoice(1, 5);
        switch (choice)
        {
        case 1:
            handleListMenu(jobLinkedList, resumeLinkedList);
            break;
        case 2:
            handleSearchMenu(jobLinkedList, resumeLinkedList);
            break;
        case 3:
            handleSortMenu(jobLinkedList, resumeLinkedList);
            break;
        case 4:
            handleMatchMenu(jobLinkedList, resumeLinkedList);
            break;
        case 5:
            running = false;
            break;
        }
    }
}

// Circular Linked List Menu

void runCircularLinkedList(JobCircularLinkedList &jobCircularLinkedList)
{
    cout << "\n=== CIRCULAR LINKED LIST MODE ===" << endl;
}

// Summary Report

void summary()
{
    cout << "summary" << endl;
}