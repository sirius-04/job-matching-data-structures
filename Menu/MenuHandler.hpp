#ifndef MENU_HANDLER_HPP
#define MENU_HANDLER_HPP

#include <iostream>
#include <string>
#include "LinkedList/JobLinkedList/JobLinkedList.hpp"
#include "LinkedList/ResumeLinkedList/ResumeLinkedList.hpp"
#include "CircularLinkedList/JobCircularLinkedList/JobCircularLinkedList.hpp"
#include "CircularLinkedList/ResumeCircularLinkedList/ResumeCircularLinkedList.hpp"
#include "Array/JobArray/JobArray.hpp"
#include "Array/ResumeArray/ResumeArray.hpp"
#include "JobMatching/JobMatching.hpp"

using namespace std;

// Utility menu functions
int getUserChoice(int min, int max);
int selectSearchAlgorithm();
int selectSortAlgorithm();
int displayMainMenu(const string &mode);
int displaySearchMenu(const string &mode);
int displaySortMenu(const string &mode);

// Array handling
void handleListArray(JobArray &jobArray, ResumeArray &resumeArray);
void handleSearchArray(JobArray &jobArray, ResumeArray &resumeArray);
void handleSortArray(JobArray &jobArray, ResumeArray &resumeArray);
void handleMatchArray(JobArray &jobArray, ResumeArray &resumeArray);
void runArray(JobArray &jobArray, ResumeArray &resumeArray);

// LinkedList handling
void handleListLinkedList(JobLinkedList &jobLinkedList, ResumeLinkedList &resumeLinkedList);
void handleLinkedListSearch(JobLinkedList &jobLinkedList, ResumeLinkedList &resumeLinkedList);
void handleLinkedListSort(JobLinkedList &jobLinkedList, ResumeLinkedList &resumeLinkedList);
void handleLinkedListMatch(JobLinkedList &jobLinkedList, ResumeLinkedList &resumeLinkedList);
void runLinkedList(JobLinkedList &jobLinkedList, ResumeLinkedList &resumeLinkedList);

// Circular LinkedList handling
void handleCircularListLinkedList(JobCircularLinkedList &jobCircularLinkedList, ResumeCircularLinkedList &resumeCircularLinkedList);
void handleCircularLinkedListSearch(JobCircularLinkedList &jobCircularLinkedList, ResumeCircularLinkedList &resumeCircularLinkedList);
void handleCircularLinkedListSort(JobCircularLinkedList &jobCircularLinkedList, ResumeCircularLinkedList &resumeCircularLinkedList);
void handleCircularLinkedListMatch(JobCircularLinkedList &jobCircularLinkedList, ResumeCircularLinkedList &resumeCircularLinkedList);
void runCircularLinkedList(JobCircularLinkedList &jobCircularLinkedList, ResumeCircularLinkedList &resumeCircularLinkedList);

// summary report
void summary();

#endif
