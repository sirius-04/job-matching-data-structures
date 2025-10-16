#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "LinkedList/JobLinkedList/JobLinkedList.hpp"
#include "CircularLinkedList/JobCircularLinkedList/JobCircularLinkedList.hpp"
#include "LinkedList/ResumeLinkedList/ResumeLinkedList.hpp"
#include "utils/CSVLoader/CSVLoader.hpp"

using namespace std;

void runArray()
{
    cout << "array" << endl;
}

void runLinkedList()
{
    cout << "linkedlist: " << endl;
}

void summary()
{
    cout << "summary" << endl;
}

void displayMenu()
{
    int userChoice;

    while (true)
    {
        cout << "\nSelect operation: " << endl;
        cout << "[1] Array" << endl;
        cout << "[2] Linked List" << endl;
        cout << "[3] Summary" << endl;
        cout << "[4] Exit" << endl;
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
            runArray();
            break;
        case 2:
            runLinkedList();
            break;
        case 3:
            summary();
            break;
        case 4:
            cout << "Exiting program..." << endl;
            return;
        default:
            cout << "Please enter a valid option!" << endl;
        }
    }
}

int main()
{
    JobLinkedList jobLinkedList;
    ResumeLinkedList resumelinkedList;
    JobCircularLinkedList jobCircularLinkedList;

    // createResumeLinkedList(resumelinkedList);
    // createJobLinkedList(jobLinkedList);

    // jobLinkedList.mergeSortBy("position");
    // jobLinkedList.printSlice();
    // resumelinkedList.printSlice();

    createJobCircularLinkedList(jobCircularLinkedList);
    jobCircularLinkedList.printSlice();
}
