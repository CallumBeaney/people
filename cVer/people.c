#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <time.h>

#define TIMEFILE "timespan" // default value in timespan is 30
#define NAMEFILE "yellowpages" // where the user saves peoples' names

typedef struct node
{
    char name[50];
    char date[12];
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
person* getLinkedList(void);
void readNamefileToLinkedList(person* head);
void pushPersonToLinkedList(person * head, char* name, char* date);
void freeList(person *head);
char* getLowercase(char* name);


// ___________________PROGRAM BEGINS___________________

int main(int argc, char *argv[])
{
    parseArguments(argc, argv);
    return 0;
}

void parseArguments(int argc, char *argv[])
{
    // Check for non-time reliant arguments first
    if (argc == 3 && strcmp(argv[1], "forget") == 0)    /* REMOVE PERSON FROM LOG e.g. people  forget  'name' */
    {
        // TODO: forget function
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
        printf("\n\tInterval to compare dates set to: %d days \n\n", userInput);
        exit(0);
    }

    // User has invoked time-reliant operations e.g. ./people check 'John Titor'
     // [day, month, year]

    if ((argc == 3) && strcmp(argv[1], "add") == 0) /* ADD PERSON: people add 'name' 'relationship' [optional] */
    {
        int dateToday[3];
        getTodaysDate(dateToday);
        char buffer[100];

        FILE* file = fopen(NAMEFILE, "r");
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

        printf("\n\tAdded %s to your People List\n\n", argv[2]);
        return;
    }
    else if (argc == 3 && strcmp(argv[1], "check") == 0 && strcmp(argv[2], "all") == 0) /* people check [all] */
    {
        // printf("checking all\n");
        person* directory = getLinkedList();
        
        // cycle through linked-list & compare lowercased names 
        person* current = directory; // head node
        while (current->next != NULL) 
        {
            int dateToday[3];
            getTodaysDate(dateToday);

            // parse dictionary date (e.g. 19/9/1999) into comparator
            char* token = strtok(current->next->date, "/");
            int comparator[3];                
            int i = 0;

            while (token != NULL) {
                comparator[i] = atoi(token);
                i++;
                token = strtok(NULL, "/");
            }

            int daysSinceLastChecked = compareDates(dateToday, comparator);

            printf("\n\t%s - last checked %i days ago", current->next->name, daysSinceLastChecked);

            current = current->next; // move pointer onto next node. Without this your terminal goes into an endless loop and you feel much shame for being a bad programmer
        }
        printf("\n\n\tend\n\n");
        free(directory);

    }
    else if (argc == 4 && strcmp(argv[1], "check") == 0 && strcmp(argv[2], "name") == 0) /* people check name [name] */
    {
        char* lowercaseName;
        lowercaseName = getLowercase(argv[3]);

        person* directory = getLinkedList();
        
        // cycle through linked-list & compare lowercased names 
        person* current = directory; // head node
        while (current->next != NULL) 
        {
            char* lowercaseLinkedListName;
            lowercaseLinkedListName = getLowercase(current->next->name);
            
            if(strcmp(lowercaseName, lowercaseLinkedListName) == 0)
            {  
                int dateToday[3];
                getTodaysDate(dateToday);

                // parse dictionary date (e.g. 19/9/1999) into comparator
                char* token = strtok(current->next->date, "/");
                int comparator[3];                
                int i = 0;

                while (token != NULL) {
                    comparator[i] = atoi(token);
                    i++;
                    token = strtok(NULL, "/");
                }

                int daysSinceLastChecked = compareDates(dateToday, comparator);
                
                printf("\n\t%s - last checked %i days ago\n\n", argv[3], daysSinceLastChecked);

                // Check if user wants to return 
                printf("Reset number of days passed to 0?\nY/N: ");
                char yesNo;
                scanf(" %[^\n]%*c", &yesNo); // get a single char
                yesNo = toupper(yesNo);
                if (yesNo == 'Y') {
                    // TODO: change stored value
                    printf("Changing values...\n\n");
                }
                
                free(lowercaseName);
                free(lowercaseLinkedListName);
                free(directory);
                exit(0);
            }
            current = current->next;
        }

        // If it gets here, the name user is checking isn't in the list.
        printf("\n\tERROR: \'%s\' not found in your People List.\n\tTo add them to your list, use: ./people add \'%s\'\n\n", argv[3], argv[3]);
        free(lowercaseName);
        free(directory);
        exit(1);

    }
    else    
    {
        // User has put in a bad input
        printf("\n┌─┐┌─┐┌─┐┌─┐┬  ┌─┐\n├─┘├┤ │ │├─┘│  ├┤ \n┴  └─┘└─┘┴  ┴─┘└─┘\n\nSyntax:\tpeople\tadd\t('forename surname' || name)\n\tpeople\tcheck\tall\n\tpeople\tcheck\t('forename surname' || name)\n\tpeople\tforget\t('forename surname' || name)\n\tpeople\tspan\tnumber [interval of days between checks]\n\nE.g.\tpeople\tadd\tAmy\n\tpeople\tcheck\t'John Wick'\n\tpeople\tspan\t69\n\nNote:\t\n\n");
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


person* getLinkedList(void)
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
        printf("ERROR: Memory allocation problem.\nThis is likely a system or memory management issue beyond the control of this program.\n");
        exit(1); // The memory allocation operation failed 
    }
    head->next = NULL; // if uninitialized, while loop in readPages() can sometimes point to a random memory location & segfault and that would be embarrassing
 
    person* current = head;
    while (current != NULL) {
        printf("%s", current->name);
        current = current->next;
    }

    file = fopen(NAMEFILE, "r");
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
    return head;
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

char* getLowercase(char* userInput)
{
    // allocate memory space the size of the user ARGV
    char* result = malloc(strlen(userInput) + 1);
    for (int i = 0; userInput[i] != '\0'; i++) 
    {
        result[i] = tolower(userInput[i]);
    }
    // printf("name: %s|\n", userInput);
    // printf("result: %s|\n", result);
        
    return result;
}