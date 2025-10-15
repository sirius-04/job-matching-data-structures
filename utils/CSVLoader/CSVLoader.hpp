#ifndef CSVLOADER_HPP
#define CSVLOADER_HPP

#include "../../LinkedList/JobLinkedList/JobLinkedList.hpp"
#include "../../LinkedList/ResumeLinkedList/ResumeLinkedList.hpp"
#include "../../Array/JobArray/JobArray.hpp"
#include "../../Array/ResumeArray/ResumeArray.hpp"

// Function declarations
void createJobLinkedList(JobLinkedList &jobLinkedList);
void createResumeLinkedList(ResumeLinkedList &resumeLinkedList);
void createJobArray(JobArray &jobArray);
void createResumeArray(ResumeArray &resumeArray);

#endif
