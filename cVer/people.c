#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include "helpers.c"

int _timeSpan;
#define FILENAME "timespan" // default value in timespan is 30

typedef struct node
{
    char name[50];
    char relationship[50];
    struct node *next;
} person;

// Function Definitions
void checkAll();
void checkName();
void parseArguments(int argc, char *argv[]);
void setTimespan(int input);
int getTimespan();

// ___________________PROGRAM BEGINS___________________

int main(int argc, char *argv[])
{
    parseArguments(argc, argv);

    char date[12];
    getDate(date);
    // printf("%s", date);
}

void parseArguments(int argc, char *argv[])
{
    /*
        ADD A PERSON+DATE TO LOG:       people  add     'name', 'type of relationship'
        LIST ALL PEOPLES' TIMESPAN:     people  check   [all]
        CHECK PERSON TIMESPAN:          people  check   'name'
        CHECK TYPE TIMESPAN:            people  check   'type'
        REMOVE PERSON FROM LOG:         people  forget  'name'
        SET TIMESPAN TO WARN:           people  days    'time'
    */

    if ((argc == 3 || argc == 4) && strcmp(argv[1], "add") == 0)
    {
        if (argc == 4)
        {
            // ./people add 'name', 'type of relationship'
            printf("adding %s to %s\n", argv[2], argv[3]);
        }
        else
        {
            // ./people add 'name'
            printf("adding %s\n", argv[2]);
        }
    }
    //     LIST ALL PEOPLES' TIMESPAN: people check [all]
    else if (argc == 3 && strcmp(argv[1], "check") == 0 && strcmp(argv[2], "all") == 0)
    {
        printf("checking all\n");
    }
    else if (argc == 4 && strcmp(argv[1], "check") == 0 && strcmp(argv[2], "name") == 0)
    {
        printf("checking %s\n", argv[3]);
    }
    //     REMOVE PERSON: people forget ('name name' || name)
    else if (argc == 3 && strcmp(argv[1], "forget") == 0)
    {
        printf("forgetting %s\n", argv[2]);
    }
    //     SET TIMESPAN TO WARN: people span [number]
    else if (argc == 3 && strcmp(argv[1], "span") == 0)
    {
        char* rawInput = argv[2];
        for (int i = 0; i < strlen(rawInput); i++) {
            if (!isdigit(rawInput[i])) {
                printf("%s is not a valid number\n", rawInput);
                printf("Usage: e.g.\tpeople\tspan\t69 [set timespan to 69 days]");
                exit(1);
            }
        }   
        int userInput = atoi(rawInput);
        setTimespan(userInput);
        printf("Interval to compare dates set to: %d days \n", userInput);
    }
    else 
    {   
        // User has put in a bad input
        printf("\nSyntax:\tpeople\tadd\t('forename surname' || name)\t'relationship' [optional]\n\tpeople\tcheck\tall\n\tpeople\tcheck\ttype\n\tpeople\tcheck\t('forename surname' || name)\n\tpeople\tforget\t('forename surname' || name)\n\tpeople\tspan\tnumber [interval of days between checks]\n\nE.g.\tpeople\tadd\tAmy\twork\n\tpeople\tcheck\tJohn\n\n");
        exit(1);
    }
}

void setTimespan(int input) 
{
    FILE *file = fopen(FILENAME, "w");
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
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        // File does not exist, initialize variable to default value
        printf("WARNING: the file 'timespan' appears to be missing from the data folder. Creating 'timespan' data file.");
    }
    else
    {
        // File exists, read variable value from file
        fscanf(file, "%d", &timespanData);
        fclose(file);
    }


    // Prompt user to enter a new value for the variable
    printf("Current value of variable is: %d\n", timespanData);
    printf("Enter a new value for the variable: ");
    scanf("%d", &timespanData);

    // Write  new value to file
    file = fopen(FILENAME, "w");
    if (file == NULL)
    {
        printf("Error opening file for writing\n");
        return 1;
    }
    fprintf(file, "%d", timespanData);
    fclose(file);

    return timespanData;
}
