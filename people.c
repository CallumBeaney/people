// TODO: in-order insertion in the linked list
// TODO: \n in longer names in checkall function

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
char* concatArgumentVector(int argc, char*argv[]);
char* getLowercase(char* name);
void rewriteDirectory(person* head);

void setTimespan(int input);
int getTimespan();

const int *getTodaysDate(int *date);
int compareDates(int *today, int *comparisonDate);
time_t timeof(int mon, int day, int yr);

person* getLinkedListFromNAMEFILE(void);
void readNamefileToLinkedList(person* head);
void pushPersonToLinkedList(person * head, char* name, char* date);
void freeList(person *head);


// ___________________PROGRAM BEGINS___________________

int main(int argc, char *argv[])
{
    /* REMOVE PERSON FROM DIRECTORY: people forget name name */
    if (argc >= 3 && strcmp(argv[1], "forget") == 0)    
    {
        char* userInputtedName;
        char* lowercaseName;
        userInputtedName = concatArgumentVector(argc, argv);
        lowercaseName = getLowercase(userInputtedName);

        // Check whether NAMEFILE exists -- if exists, cycle a linkedlist built from it and check whether Name To Delete exists.
        person* directory = getLinkedListFromNAMEFILE();
        FILE* file = fopen(NAMEFILE, "r");
        if (file != NULL)
        {
            person* current = directory; //head node for loop-checking
            int found = 0;
            while(current->next != NULL) 
            {
                char* lowercaseLinkedListName;
                lowercaseLinkedListName = getLowercase(current->next->name);
                if(strcmp(lowercaseName, lowercaseLinkedListName) == 0)
                {   
                    // name is in the directory
                    found = 1;
                    free(lowercaseLinkedListName);
                    break;
                    
                }
                current = current->next; // move pointer onto next node. Without this your terminal goes into an endless loop and you stay up to 1AM repeatedly failing to notice this obvious error
            }
            if (found != 1) {
                printf("\n\tERROR: this name was not found in your user list. Use [./people check all] to check your user list.\n\n");
                free(userInputtedName);
                free(lowercaseName);
                freeList(directory);
                fclose(file);
                return 1;
            }
        }
        else if (file == NULL) {
            printf("\n\n\tERROR: Your People List file does not exist. Use [./people add forename surname] to create and add to your People List.\n\n");
            free(userInputtedName);
            free(lowercaseName);
            fclose(file);
            return 1;
        }
        fclose(file);

        // if the program gets here, the user's inputted an existing name and the People List NAMEFILE exists.
        // Rewrite the directory to the NAMEFILE by excluding the node with the unwanted matching ->name
        file = fopen(NAMEFILE,"w");
        person* current = directory;
        while (current->next != NULL)
        {
            char* lowercaseLinkedListName;
            lowercaseLinkedListName = getLowercase(current->next->name);
            if (strcmp(lowercaseName, lowercaseLinkedListName) != 0)
            {
                char buffer[100];
                snprintf(buffer, sizeof(buffer), "%s,%s\n", current->next->name, current->next->date);
                fprintf(file, "%s", buffer);
            }

            current = current->next;
        }
        fclose(file);
        freeList(directory);
        printf("\n\t%s has been removed from your People List\n\n", userInputtedName);
        return 0;
    }
    /* SET TIMESPAN: people days [number] */
    if (argc == 3 && strcmp(argv[1], "days") == 0) 
    {
        char *rawInput = argv[2];
        for (int i = 0; i < strlen(rawInput); i++)
        {
            if (!isdigit(rawInput[i]))
            {
                printf("\n\tERROR: %s is not a valid number", rawInput);
                printf("\n\tUsage: e.g.\tpeople\tdays\t69 [set timespan to 69 days]\n\n");
                exit(1);
            }
        }
        int userInput = atoi(rawInput);
        setTimespan(userInput);
        printf("\n\tInterval to compare dates set to: %d days \n\n", userInput);
        return 0;
    }
    else if ((argc >= 3) && strcmp(argv[1], "add") == 0) 
    {
        char* userInputtedName;
        userInputtedName = concatArgumentVector(argc, argv);
        char* lowercaseName;
        lowercaseName = getLowercase(userInputtedName);

        // Check whether NAMEFILE exists, if it does cycle through linked-list and check whether the Name To Add already exists
        FILE* file = fopen(NAMEFILE, "r");
        if (file != NULL)
        {
            person* directory = getLinkedListFromNAMEFILE();
            person* current = directory; // head node
            while (current->next != NULL) 
            {
                char* lowercaseLinkedListName;
                lowercaseLinkedListName = getLowercase(current->next->name);
                if(strcmp(lowercaseName, lowercaseLinkedListName) == 0)
                {          
                    printf("\n\t%s is already in your People List!\n\tTo check: [ ./people check %s ]\n\n", userInputtedName, userInputtedName);
                    free(lowercaseLinkedListName);
                    free(userInputtedName);
                    freeList(directory);
                    fclose(file);
                    return 1;
                }
                current = current->next;
            }
        }
        fclose(file);

        // If the program gets to here, then the user's inputted a new name. 
        // If the file == NULL, the fopen below is going to initialise that namefile.
        int dateToday[3];
        getTodaysDate(dateToday);
        char buffer[100];

        file = fopen(NAMEFILE, "a");
        snprintf(buffer, sizeof(buffer), "%s,%i/%i/%i\n", userInputtedName, dateToday[0], dateToday[1], dateToday[2]);
        fprintf(file, "%s", buffer);
        fclose(file);

        printf("\n\tAdded %s to your People List\n\n", userInputtedName);
        free(userInputtedName);
        free(lowercaseName);
        return 0;
    }
    /* CHECK ALL PEOPLE IN DIRECTORY: people check [all] */
    else if (argc == 3 && strcmp(argv[1], "check") == 0 && strcmp(argv[2], "all") == 0) 
    {
        // cycle through linked-list & compare lowercased names 
        person* directory = getLinkedListFromNAMEFILE();        
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
            int elapsedCheck = getTimespan();

            printf("\n\t%s\t- last checked\t%i\tdays ago", current->next->name, daysSinceLastChecked);

            if (daysSinceLastChecked > elapsedCheck) {
                printf("  ! IMPORTANT");
            }
    
            current = current->next; 
        }
        printf("\n\n");
        freeList(directory);
        return 0;
    }
    /* CHECK A SPECIFIC PERSON: people check forename surname */
    else if (strcmp(argv[1], "check") == 0 && argc >= 3) 
    {   
        // Ensure they aren't fudging a [./people check all]
        if (strcmp(argv[2], "all") == 0) {
            printf("\n\tERROR: To check for all people in the list, use: [./people check all]\n\n");
            return 1;
        }

        char* userInputtedName;
        char* lowercaseName;
        userInputtedName = concatArgumentVector(argc, argv);
        lowercaseName = getLowercase(userInputtedName);
        
        // cycle through linked-list & compare lowercased names 
        person* directory = getLinkedListFromNAMEFILE();
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
    
                printf("\n\t%s - last checked %i days ago\n\n", current->next->name, daysSinceLastChecked);

                // Check if user wants to reset date 
                printf("Reset number of days passed to 0?\nY/N: ");
                char yesNo;
                scanf(" %[^\n]%*c", &yesNo); // get a single char
                yesNo = toupper(yesNo);
                if (yesNo == 'Y') {
                    // Reset date in linked list
                    snprintf(current->next->date, sizeof(current->next->date), "%i/%i/%i", dateToday[0], dateToday[1], dateToday[2]);

                    rewriteDirectory(directory); // write linked list to NAMEFILE
                    printf("Changed lookup date for %s to today's date.\n\n", userInputtedName);    
                }
                
                free(lowercaseName);
                free(userInputtedName);
                free(lowercaseLinkedListName);
                freeList(directory);
                return 0;
            }
            current = current->next;
        }

        // If program gets here, the name user is checking isn't in the list.
        printf("\n\tERROR: \'%s\' not found in your People List.\n\tTo add them to your list, use: ./people add \'%s\'\n\n", userInputtedName, userInputtedName);
        free(lowercaseName);
        free(userInputtedName);
        freeList(directory);
        return 1;

    }
    else    
    {
        // User has put in a bad input
        printf("\n┌─┐┌─┐┌─┐┌─┐┬  ┌─┐\n├─┘├┤ │ │├─┘│  ├┤ \n┴  └─┘└─┘┴  ┴─┘└─┘\n\nSyntax:\tpeople\tadd\tforename surname\n\tpeople\tcheck\tall\n\tpeople\tcheck\tforename surname\n\tpeople\tforget\tforename surname\n\tpeople\tdays\tnumber [interval of days between checks]\n\nE.g.\tpeople\tadd\tAmy\n\tpeople\tcheck\tJohn Wick\n\tpeople\tdays\t96\n\n");
        exit(1);
    }    

    // the program should never get here
    return 1;
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
        printf("\n\tThe file 'timespan' appears to be missing from the data folder.\n\tCreating 'timespan' data file with default interval of 30 days.\n\tTo change, use: 'people time [number]\n");
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


const int* getTodaysDate(int *date)
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


person* getLinkedListFromNAMEFILE(void)
{
    // Check whether NAMEFILE exists and close if not (e.g. user tries to check a name before adding a name)
    FILE *file = fopen(NAMEFILE, "r");
    if (file == NULL) {
        printf("\n\tYou don't have anyone saved to your People List!\n\tAdd someone using [./people add forename surname] and try again!\n\n");
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
    // printf("TOLOWER   name: %s|\n", userInput);
    // printf("TOLOWER result: %s|\n", result);
    return result;
}

char* concatArgumentVector(int argc, char* argv[])
{
    char* buffer = (char *) malloc(100);
    strcpy(buffer, argv[2]); 
    // 1. if user writes just one name [./people check BatMan] or writes [./people check 'john wick'], strcpy() handles it. 
    // 2. strcpy initialises the above array and appends \0 at the end.
    if (argc > 3) 
    {
        int i = 3;
        while (argv[i] != NULL)
        {
            strcat(buffer, " ");
            strcat(buffer, argv[i]);
            i++;
        }
    }
    return buffer;
}

void rewriteDirectory(person* head) 
{
    // here you are basically rebuilding the yellowpages directory file from a linked list you have made an edit to e.g. updating the date of a Person
    FILE* file = fopen(NAMEFILE, "w");

    person* current = head;
    while (current->next != NULL)
    {
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "%s,%s\n", current->next->name, current->next->date);
        fprintf(file, "%s", buffer);

        current = current->next;
    }
    fclose(file);
}