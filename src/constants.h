#ifndef CONSTANTS_H
#define CONSTANTS_H

#define TIMEFILE "timespan" // default value in timespan is 30 days
#define NAMEFILE "yellowpages" // where the user saves peoples' names

typedef struct node {
    char name[50];
    char date[12];
    struct node *next;
} person;

#endif