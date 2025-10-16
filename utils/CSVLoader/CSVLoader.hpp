#ifndef CSVLOADER_HPP
#define CSVLOADER_HPP

#include "../../LinkedList/JobLinkedList/JobLinkedList.hpp"
#include "../../CircularLinkedList/JobCircularLinkedList/JobCircularLinkedList.hpp"
#include "../../LinkedList/ResumeLinkedList/ResumeLinkedList.hpp"

// Function declarations
void createJobLinkedList(JobLinkedList &jobLinkedList);
void createResumeLinkedList(ResumeLinkedList &resumeLinkedList);

void createJobCircularLinkedList(JobCircularLinkedList &jobCircularLinkedList);

#endif
