#include <iostream>
#include "JobLinkedList/JobLinkedList.h"
using namespace std;

int main() {
    JobLinkedList* jobs = new JobLinkedList("1", "software engineer");

    jobs->append("2", "data");
    jobs->printList();

    return 0;
}