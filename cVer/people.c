#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
// #include "helpers.c"
#include <time.h>

int _timeSpan;
#define TIMEFILE "timespan" // default value in timespan is 30
#define NAMEFILE "yellowpages" // where the user saves their friends' names

typedef struct node
{
    char name[50];
    char date[12];
    // char relationship[50];
    struct node *next;
} person;

// Function Definitions
void checkAll();
void checkName();
void parseArguments(int argc, char *argv[]);
void setTimespan(int input);
int getTimespan();
int compareDates(int *today, int *comparisonDate);
time_t timeof(int mon, int day, int yr);
const int *getTodaysDate(int *date);
void getLinkedList(void);
void readNamefileToLinkedList(person* head);
void pushPersonToLinkedList(person * head, char* name, char* date);
void freeList(person *head);



// ___________________PROGRAM BEGINS___________________

int main(int argc, char *argv[])
{
    parseArguments(argc, argv);

    getLinkedList();

    return 0;
}

void parseArguments(int argc, char *argv[])
{
    // Check for non-time reliant arguments first
    if (argc == 3 && strcmp(argv[1], "forget") == 0)    /* REMOVE PERSON FROM LOG e.g. people  forget  'name' */
    {
        printf("forgetting %s\n", argv[2]);
        exit(0);
    }
    if (argc == 3 && strcmp(argv[1], "span") == 0) /* SET TIMESPAN e.g. people span [number] */
    {
        char *rawInput = argv[2];
        for (int i = 0; i < strlen(rawInput); i++)
        {
            if (!isdigit(rawInput[i]))
            {
                printf("%s is not a valid number\n", rawInput);
                printf("Usage: e.g.\tpeople\tspan\t69 [set timespan to 69 days]");
                exit(1);
            }
        }
        int userInput = atoi(rawInput);
        setTimespan(userInput);
        printf("Interval to compare dates set to: %d days \n", userInput);
        exit(0);
    }

    // User has invoked time-reliant operations e.g. ./people check 'John Titor'
    int dateToday[3]; // [day, month, year]

    if ((argc == 3 || argc == 4) && strcmp(argv[1], "add") == 0) /* ADD PERSON: people add 'name' 'relationship' [optional] */
    {
        getTodaysDate(dateToday);
        char buffer[100];

        FILE* file = fopen(NAMEFILE, "r"); // a for Append
        if (file == NULL) {
            fclose(file);
            file = fopen(NAMEFILE, "w");
            snprintf(buffer, sizeof(buffer), "%s,%i/%i/%i\n", argv[2], dateToday[0], dateToday[1], dateToday[2]);
            fprintf(file, "%s", buffer);
            fclose(file);
        }
        else {
            fclose(file);
            file = fopen(NAMEFILE, "a");
            snprintf(buffer, sizeof(buffer), "%s,%i/%i/%i\n", argv[2], dateToday[0], dateToday[1], dateToday[2]);
            fprintf(file, "%s", buffer);
            fclose(file);
        }

        printf("Added %s to your People List\n", argv[2]);
        return;
    }
    else if (argc == 3 && strcmp(argv[1], "check") == 0 && strcmp(argv[2], "all") == 0) /* people check [all] */
    {
        printf("checking all\n");
    }
    else if (argc == 4 && strcmp(argv[1], "check") == 0 && strcmp(argv[2], "name") == 0) /* people check name [name] */
    {
        // TODO: load "comparator" from/into people dictionary

        printf("checking %s\n", argv[3]);

        getTodaysDate(dateToday);
        int comparator[3] = {15, 2, 2022}; // 413 days
        printf("Today's Date: %i/%i/%i\n", dateToday[0], dateToday[1], dateToday[2]);
        printf("Comparator Date: %i/%i/%i\n", comparator[0], comparator[1], comparator[2]);

        int daysSinceLastChecked = compareDates(dateToday, comparator);
        
        // // Check if user wants to return 
        // printf("It has been %i days since you last contacted %s.\nReset day counter to 0?\tY/N: ", daysSinceLastChecked, argv[3]);
        // char yesNo;
        // scanf(" %[^\n]%*c", &yesNo); // get a single char
        // yesNo = toupper(yesNo);
        // if (yesNo != 'Y') {
        //     // TODO: change stored value
        // }

        getLinkedList();

    }
    else    
    {
        // User has put in a bad input
        printf("\n┌─┐┌─┐┌─┐┌─┐┬  ┌─┐\n├─┘├┤ │ │├─┘│  ├┤ \n┴  └─┘└─┘┴  ┴─┘└─┘\n\nSyntax:\tpeople\tadd\t('forename surname' || name)\t'relationship' [optional]\n\tpeople\tcheck\tall\n\tpeople\tcheck\tname\t('forename surname' || name)\n\tpeople\tcheck\ttype\trelationship [e.g. work]\n\tpeople\tforget\t('forename surname' || name)\n\tpeople\tspan\tnumber [interval of days between checks]\n\nE.g.\tpeople\tadd\tAmy\twork\n\tpeople\tcheck\tJohn\n\tpeople\tspan\t69\n\nNote:\t\n\n");
        exit(1);
    }
}

void setTimespan(int input)
{
    FILE *file = fopen(TIMEFILE, "w");
    if (file == NULL)
    {
        printf("\nNo 'timespan' file present in program directory. Creating file with chosen value...\n");
        return;
    }
    fprintf(file, "%d", input);
    fclose(file);

    return;
}

int getTimespan(void)
{
    int timespanData;
    FILE *file = fopen(TIMEFILE, "r");
    if (file == NULL)
    {
        // File does not exist, initialize variable to default value
        printf("WARNING: the file 'timespan' appears to be missing from the data folder. Creating 'timespan' data file with default interval of 30 days. To change, use: 'people span [number]\n");
        fclose(file);
        timespanData = 30;
        file = fopen(TIMEFILE, "w");
        fprintf(file, "%d", timespanData);
        fclose(file);
        return timespanData;
    }
    else
    {
        // File exists, read variable value from file
        fscanf(file, "%d", &timespanData);
        fclose(file);
    }

    return timespanData;
}


const int *getTodaysDate(int *date)
{
    time_t t = time(NULL);               // get current time data
    struct tm *timeData = localtime(&t); // parse into fields using time.h lib fn

    date[0] = timeData->tm_mday;
    date[1] = timeData->tm_mon + 1;    // add 1 to the tm_mon field
    date[2] = timeData->tm_year + 1900; // add 1900 to tm_year field to get actual year value
    // printf("Today's Date: %i - %i - %i\n", date[2], date[1], date[0]);

    return date;
}



int compareDates(int* today, int* comparisonDate)
{
    time_t presentDate;
    time_t pastDate;
    time_t difference;

    presentDate = timeof(today[0], today[1], today[2]);
    pastDate = timeof(comparisonDate[0], comparisonDate[1], comparisonDate[2]);

    difference = presentDate - pastDate;
    difference /= (24 * 60 * 60);

    return (int) difference;
}

time_t timeof(int day, int mon, int yr)
{
    struct tm tm;
    time_t tod;

    memset(&tm, 0, sizeof(tm));

    tm.tm_year = yr - 1900;
    tm.tm_mon = mon - 1;
    tm.tm_mday = day;

    // minimize issues with DST crossover or leap seconds
    tm.tm_hour = 1;

    tod = mktime(&tm);

    return tod;
}




void getLinkedList(void)
{
    // Check whether NAMEFILE exists and close if not (e.g. user tries to check a name before adding a name)
    FILE *file = fopen(NAMEFILE, "r");
    if (file == NULL) {
        printf("You don't have anyone saved to your People List!\nAdd someone using [./people add 'name name'] and try again!\n");
        fclose(file);
        exit(1);
    }
    fclose(file);

    // Make head of linked list
    person* head = NULL;
    head = (person *) malloc(sizeof(person)); // allocate pointer to malloc'd space the size of a "person" struct
    if (head == NULL) {
        exit(1); // The memory allocation operation failed
    }
    head->next = NULL; // if uninitialized, while loop in readPages() can sometimes point to a random memory location & segfault

    readNamefileToLinkedList(head);
    // head -> name = 
    // head -> date =
    // head -> next = NULL;
    
}



void readNamefileToLinkedList(person* head) 
{
    person* current = head;

    while (current != NULL) {
        printf("%s", current->name);
        current = current->next;
    }


    FILE *file = fopen(NAMEFILE, "r");
    if (file == NULL) {
        printf("Error: failed to open file\n");
        fclose(file);
        exit(1);
    }

    char line[1024];
    char name[50];
    char date[12];

    while (fgets(line, sizeof(line), file) != NULL) {
        // Parse the values from the CSV line using sscanf
        // printf("%s", line);
        if (sscanf(line, "%[^,],%[^\n]", name, date) != 2) {
            printf("Error: invalid input format\n");
            exit(1);
        }
        pushPersonToLinkedList(head, name, date);
    }

    fclose(file);
    // freeList(head);
}

void pushPersonToLinkedList(person* head, char* name, char* date) 
{             
    // set current to be the last node in the list
    person* current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    // Add new variable to linked list by creating a new node and copying data into it
    // remember: cannot assign to arrays; must strcpy
    current->next = (person *) malloc(sizeof(person));
    strcpy(current->next->name, name); 
    strcpy(current->next->date, date); 
    current->next->next = NULL;

    // printf("%s, ", current->next->name);
    // printf("%s\n", current->next->date);
}

void freeList(person * head)
{
   struct node* tmp;

   while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }

}