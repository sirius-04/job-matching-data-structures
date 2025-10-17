#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "LinkedList/JobLinkedList/JobLinkedList.hpp"
#include "CircularLinkedList/JobCircularLinkedList/JobCircularLinkedList.hpp"
#include "LinkedList/ResumeLinkedList/ResumeLinkedList.hpp"
#include "utils/CSVLoader/CSVLoader.hpp"
#include "Array/JobArray/JobArray.hpp"
#include "Array/ResumeArray/ResumeArray.hpp"
#include "JobMatching/JobMatching.hpp"
#include "Menu/MenuHandler.hpp"

using namespace std;

void displayMenu(JobArray &jobArray, ResumeArray &resumeArray, JobLinkedList &jobLinkedList, ResumeLinkedList &resumelinkedList, JobCircularLinkedList &jobCircularLinkedList)
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
            runCircularLinkedList(jobCircularLinkedList);
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

    createResumeLinkedList(resumelinkedList);
    createJobLinkedList(jobLinkedList);
    createJobCircularLinkedList(jobCircularLinkedList);
    createResumeArray(resumeArray);

    // JobLinkedList *matchedJobs = jobLinkedList.linearSearchJobBySkills(new string[4]{"Rest Apis", "Spring Boot", "Git", "Agile"}, 4, true);
    // JobLinkedList *matchedJobs = jobLinkedList.binarySearchJobBySkills(new string[4]{"Rest Apis", "Spring Boot", "Git", "Agile"}, 4, true);

    // JobLinkedList *matchedJobs = jobLinkedList.binarySearchJobByPosition("Software Engineer");
    // JobLinkedList *matchedJobs = jobLinkedList.linearSearchJobByPosition("Software Engineer");

    // string skills[] = {"Rest Apis", "Spring Boot", "Git", "Agile"};
    // ResumeLinkedList *matchedJobs = resumelinkedList.binarySearchResumeBySkills(skills, 4, true);
    // ResumeLinkedList *matchedJobs = resumelinkedList.linearSearchResumeBySkills(skills, 4, true);

    // resumelinkedList.quickSortBySkillCount();
    // matchedJobs->printSlice();

    // resumelinkedList.printSlice();
    // createJobArray(jobArray);
    // JobArray *result = jobArray.linearSearchByPosition("Data Analyst");
    // result->printJobs();

    // createResumeArray(resumeArray);
    // resumeArray.quickSortBySkill();
    // const string skills[] = {"python", "pandas"};
    // ResumeArray *resumeResult = resumeArray.binarySearchBySkills(skills, 2, true);
    // resumeResult->printResumes();

    displayMenu(jobArray, resumeArray, jobLinkedList, resumelinkedList, jobCircularLinkedList);
}
