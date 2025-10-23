#ifndef CONFIG_ENUM
#define CONFIG_ENUM

enum MatchMode
{
    FIND_JOB,
    FIND_RESUME
};

enum DataStruct
{
    ARRAY,
    SINGLY_LINKED_LIST,
    CIRCULAR_LINKED_LIST
};

enum MatchStrategy
{
    KEYWORD_BASED,
    WEIGHTED_SCORING
};

enum SearchAlgorithm
{
    LINEAR,
    BINARY
};

enum SearchTarget {
    SEARCH_JOB,
    SEARCH_RESUME
};

enum SortAlgorithm {
    MERGE,
    QUICK,
    NONE
};

#endif