#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "LinkedList/JobLinkedList/JobLinkedList.hpp"
#include "CircularLinkedList/JobCircularLinkedList/JobCircularLinkedList.hpp"
#include "LinkedList/ResumeLinkedList/ResumeLinkedList.hpp"
#include "CircularLinkedList/ResumeCircularLinkedList/ResumeCircularLinkedList.hpp"
#include "utils/CSVLoader/CSVLoader.hpp"
#include "Array/JobArray/JobArray.hpp"
#include "Array/ResumeArray/ResumeArray.hpp"
#include "JobMatching/JobMatching.hpp"
#include "Menu/MenuHandler.hpp"

using namespace std;

void displayMenu(JobArray &jobArray, ResumeArray &resumeArray, JobLinkedList &jobLinkedList, ResumeLinkedList &resumelinkedList, JobCircularLinkedList &jobCircularLinkedList, ResumeCircularLinkedList &resumeCircularLinkedList)
{
    int userChoice;

    while (true)
    {
        cout << "\nSelect operation: " << endl;
        cout << "[1] Array" << endl;
        cout << "[2] Linked List" << endl;
        cout << "[3] Circular Linked List" << endl;
        cout << "[4] Summary" << endl;
        cout << "[5] Exit" << endl;
        cout << "Enter choice: ";

        if (!(cin >> userChoice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }

        switch (userChoice)
        {
        case 1:
            runArray(jobArray, resumeArray);
            break;
        case 2:
            runLinkedList(jobLinkedList, resumelinkedList);
            break;
        case 3:
            runCircularLinkedList(jobCircularLinkedList, resumeCircularLinkedList);
            break;
        case 4:
            summary();
            break;
        case 5:
            cout << "Exiting program..." << endl;
            return;
        default:
            cout << "Please enter a valid option!" << endl;
        }
    }
}

int main()
{
    JobArray jobArray;
    ResumeArray resumeArray;
    JobLinkedList jobLinkedList;
    ResumeLinkedList resumelinkedList;
    JobCircularLinkedList jobCircularLinkedList;
    ResumeCircularLinkedList resumeCircularLinkedList;

    createResumeLinkedList(resumelinkedList);
    createJobLinkedList(jobLinkedList);
    // createJobCircularLinkedList(jobCircularLinkedList);
    // createResumeCircularLinkedList(resumeCircularLinkedList);

    // JobLinkedList *matchedJobs = jobLinkedList.linearSearchJobBySkills(new string[2]{"Python", "Pandas"}, 2, false);
    // JobLinkedList *matchedJobs = jobLinkedList.binarySearchJobBySkills(new string[2]{"Python", "Pandas"}, 2, false);

    // JobLinkedList *matchedJobs = jobLinkedList.binarySearchJobByPosition("Data Analyst");
    // JobLinkedList *matchedJobs = jobLinkedList.linearSearchJobByPosition("Data Analyst");
    // JobCircularLinkedList *matchedJobs = jobCircularLinkedList.binarySearchJobByPosition("Data Analyst");

    // string skills[] = {"Python", "Pandas"};
    // ResumeLinkedList *matchedJobs = resumelinkedList.binarySearchResumeBySkills(skills, 2, false);
    // ResumeLinkedList *matchedJobs = resumelinkedList.linearSearchResumeBySkills(skills, 2, false);

    // resumelinkedList.quickSortBySkillCount();
    // matchedJobs->printSlice();

    // resumelinkedList.printSlice();
    createJobArray(jobArray);
    createResumeArray(resumeArray);

    // jobArray.quickSortByPosition();
    // JobArray *result = jobArray.linearSearchByPosition("data analyst");
    // JobArray *result = jobArray.binarySearchByPosition("data analyst");
    // string skills[] = {"python", "pandas"};
    // JobArray *result = jobArray.linearSearchBySkills(skills, 2, false);
    // JobArray *result = jobArray.binarySearchBySkills(skills, 2, false);
    // jobArray.mergeSort(jobArray.compareByPosition);
    // jobArray.compareByFirstSkill();
    // jobArray.quickSortByPosition();
    // jobArray.quickSort("position");
    // jobArray.printJobs();
    // resumeArray.quickSort("skillCount");
    resumeArray.mergeSort(resumeArray.compareBySkillCount);
    resumeArray.printResumes();
    // result->printJobs();

    // const string skills[] = {"Python", "Pandas"};
    // ResumeArray *resumeResult = resumeArray.linearSearchBySkills(skills, 2, false);
    // ResumeArray *resumeResult = resumeArray.binarySearchBySkills(skills, 2, false);
    // resumeResult->printResumes();

    // displayMenu(jobArray, resumeArray, jobLinkedList, resumelinkedList, jobCircularLinkedList, resumeCircularLinkedList);
}
