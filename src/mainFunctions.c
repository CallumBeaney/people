#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h>

#include "mainFunctions.h"
#include "helpers.h"


int forgetAll() 
{
  printf("Do you want to delete entire people list? [y/n]:  ");
  char answer;
  scanf(" %[^\n]%*c", &answer);
  answer = toupper(answer);
  
  if(answer == 'Y') {
    int purge = remove(getNamefilePath());
    if(purge == 0) {
      printf("\nPeople List deleted successfully.\nA new one will be generated when you next use [./people add _____]\n\n");
      return 0;
    } else {
      printf("\nError: unable to delete the file. It may already have been deleted, or may be yet to be created.\nTry generating a new one by running [./people add _____]\n");
      return 1;
    }
  } else {
    printf("Operation cancelled.\n");
    return 0;
  }
}


int forgetPerson(int argc, char *argv[])
{
  char* userInputtedName;
  char* lowercaseName;
  userInputtedName = concatArgumentVector(argc, argv);
  lowercaseName = getLowercase(userInputtedName);

  // cycle through a linkedlist built from namefile and check whether Name To Delete exists.
  person* directory = getLinkedListFromNamefile();
  FILE* file = fopen(getNamefilePath(), "r");
  if (file != NULL) {
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
      current = current->next; // move pointer onto next node. 
      // Without this pointer relocation your terminal goes into an endless loop and, suffering from the flu, you stay up exhausted and confused till 2AM repeatedly failing to notice this obvious error and feel a shame that though viewed from afar may seem to be but a tiny twinkling speck in the cavernous abyss of the great night sky, is in actuality a celstial behemoth of furiously combusting solar disgrace that will burn deep within your soul forever, consuming it until finally the last oxidizable molecule of your spirit is but a pitiful ember drifting in the wind along the shale afore the gloaming. 
    }
    if (found != 1) {
      printf("\nERROR: this name was not found in your user list. Use [./people check all] to check your user list.\n\n");
      free(userInputtedName);
      free(lowercaseName);
      freeList(directory);
      fclose(file);
      return 0;
    }
  }
  else if (file == NULL) {
    printf("\n\nERROR: Your namefile does not exist. Use [./people add name to create and add to your People List.\n\n");
    free(userInputtedName);
    free(lowercaseName);
    return 1;
  }
  fclose(file);

  // if the program gets here, the user's inputted an existing name + the People List NAMEFILE exists.
  // Rewrite the directory to the NAMEFILE by excluding the node with the unwanted matching ->name
  file = fopen(getNamefilePath(), "w");
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
  printf("\n%s has been removed from your People List\n\n", userInputtedName);
  return 0;
}


int addPerson(int argc, char *argv[]) 
{
  char* userInputtedName;
  userInputtedName = concatArgumentVector(argc, argv);
  char* lowercaseName;
  lowercaseName = getLowercase(userInputtedName);

  // Check whether NAMEFILE exists, if it does cycle through linked-list and check whether the Name-To-Add already exists
  FILE* file = fopen(getNamefilePath(), "r");
  if (file != NULL) {
    person* directory = getLinkedListFromNamefile();
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
        return 0;
      }
      current = current->next;
    }
  }

  // If the program gets to here, then the user's inputted a new name. 
  // If for some reason the file == NULL, the fopen below is going to initialise that namefile anyway.
  int dateToday[3];
  getTodaysDate(dateToday);
  char buffer[100];

  file = fopen(getNamefilePath(), "a");
  snprintf(buffer, sizeof(buffer), "%s,%i/%i/%i\n", userInputtedName, dateToday[0], dateToday[1], dateToday[2]);
  fprintf(file, "%s", buffer);
  fclose(file);

  printf("\nAdded %s to your People List\n\n", userInputtedName);
  free(userInputtedName);
  free(lowercaseName);
  return 0;
}


int checkAllPersons() 
{
  // cycle through linked-list & compare lowercased names 
  person* directory = getLinkedListFromNamefile();        
  person* current = directory; // head node

  sortLinkedListByName(current);

  while (current->next != NULL) {
    int dateToday[3];
    getTodaysDate(dateToday);

    // Make a copy of the date without touching the struct
    char* dateCopy = strdup(current->next->date);
    if (dateCopy == NULL) {
      printf("ERROR: Memory allocation error.\n");
      return 1;
    }

    //strtok() modifies the string it is called on and replaces the delimiter with a null terminator. 
    char* token = strtok(dateCopy, "/");

    int comparator[3];                
    int i = 0;

    while (token != NULL) {
      // parse node date (e.g. 19/9/1999) into comparator
      comparator[i] = atoi(token);
      i++;
      token = strtok(NULL, "/");
    }

    int elapsedCheck = getTimespan();
    int daysSinceLastChecked = compareDates(dateToday, comparator);

    // This just spaces the names appropriately
    int len = strlen(current->next->name);
    if (len >= 23) {
      printf("\n%s\t- last checked\t%i\tdays ago", current->next->name, daysSinceLastChecked);
    } else if (len >= 16) {
      printf("\n%s\t- last checked\t%i\tdays ago", current->next->name, daysSinceLastChecked);
    } else if (len < 8) {
      printf("\n%s\t\t\t- last checked\t%i\tdays ago", current->next->name, daysSinceLastChecked);
    } else {
      printf("\n%s\t\t- last checked\t%i\tdays ago", current->next->name, daysSinceLastChecked);
    }

    if (daysSinceLastChecked > elapsedCheck) {
      printf("  ! IMPORTANT");
    }
    current = current->next; 
  }

  // ask user if they want to reset dates
  printf("\n\nReset number of days passed for ALL entries to 0?\nY/N: ");
  char yesNo;
  scanf(" %[^\n]%*c", &yesNo); // get a single char
  yesNo = toupper(yesNo);
  if (yesNo == 'Y') {
    int dateToday[3];
    getTodaysDate(dateToday);
    person* current = directory; 
    while (current->next != NULL) {  // Reset date in linked list
      snprintf(current->next->date, sizeof(current->next->date), "%i/%i/%i", dateToday[0], dateToday[1], dateToday[2]);
      current = current->next; 
    }
    rewriteDirectory(directory); // write linked list to NAMEFILE
    printf("\nChanged lookup date for ALL to today's date.\n");    
  } else {
    rewriteDirectory(directory); 
  }

  printf("\n");
  freeList(directory);
  return 0;
}

int checkSpecificPerson(int argc, char *argv[]) 
{
  printf("\n");

  // Ensure they aren't fudging a [./people check all]
  if (strcmp(argv[2], "all") == 0) {
    printf("ERROR: To check for all people in the list, use: [./people check all]\n\n");
    return 1;
  }

  // cycle through linked-list & compare lowercased names:
  char* userInputtedName;
  userInputtedName = concatArgumentVector(argc, argv);
  
  person* directory = getLinkedListFromNamefile();
  person* current = directory; // head node

  int dateToday[3];
  getTodaysDate(dateToday);

  int found = 0;
  
  while (current->next != NULL) 
  {
    char* linkedListName = current->next->name;

    char* result = strcasestr(linkedListName, userInputtedName); // strcasestr(str, substr);
    if (result == NULL) {
      current = current->next;
      continue;
    } else {
      found = 1; // substring found

      char* dateCopy = strdup(current->next->date);
      if (dateCopy == NULL) {
        printf("ERROR: Memory allocation error.\n");
        return 1;
      }

      char* token = strtok(dateCopy, "/");
      int comparator[3];                
      int i = 0;
      while (token != NULL) {
        comparator[i] = atoi(token);
        i++;
        token = strtok(NULL, "/");
      }

      int daysSinceLastChecked = compareDates(dateToday, comparator);
      int elapsedCheck = getTimespan();


      int len = strlen(current->next->name);
      if (len >= 23) {
        printf("%s\t- last checked\t%i\tdays ago", current->next->name, daysSinceLastChecked);
      } else if (len >= 16) {
        printf("%s\t- last checked\t%i\tdays ago", current->next->name, daysSinceLastChecked);
      } else if (len < 8) {
        printf("%s\t\t\t- last checked\t%i\tdays ago", current->next->name, daysSinceLastChecked);
      } else {
        printf("%s\t\t- last checked\t%i\tdays ago", current->next->name, daysSinceLastChecked);
      }
      
      if (daysSinceLastChecked > elapsedCheck) {
        printf("  ! IMPORTANT");
      }
      printf("\n");
    }

    current = current->next;
  }

  if (found != 1) {
    // the name user is checking isn't in the list.
    printf("\nERROR: \'%s\' not found in your People List.\nTo add them to your list, use: ./people add \'%s\'\n\n", userInputtedName, userInputtedName);
    free(userInputtedName);
    freeList(directory);
    return 1;
  }

  // reset trackable node pointer to head position
  current = directory;
  
  printf("\nReset number of days passed for all entries shown to 0?\nY/N: ");
  char yesNo;
  scanf(" %[^\n]%*c", &yesNo);
  yesNo = toupper(yesNo);
  
  if (yesNo == 'Y') {
    while (current->next != NULL) 
    {
      char* linkedListName = current->next->name;

      char* result = strcasestr(linkedListName, userInputtedName);
      if (result == NULL) {
        current = current->next; 
        continue;
      } 
      else {
        // Reset date in linked list
        snprintf(current->next->date, sizeof(current->next->date), "%i/%i/%i", dateToday[0], dateToday[1], dateToday[2]);
      }
      // printf("\nChanged lookup date for %s to today's date.\n", linkedListName);  
      current = current->next;
    }
    printf("\nChanged lookup date for the above entries to today's date.\n\n");  
  } else {
    printf("\n");
  }

  rewriteDirectory(directory);
  free(userInputtedName);
  freeList(directory);    
  return 0;
}


int setNumberOfDaysBenchmark(char *argv[]) {
   char *rawInput = argv[2];
  for (int i = 0; i < strlen(rawInput); i++)
  {
    if (!isdigit(rawInput[i]))
    {
      printf("\nERROR: %s is not a valid number", rawInput);
      printf("\nUsage: e.g.\tpeople\tdays\t69 [set timespan to 69 days]\n\n");
      return 1;
    }
  }
  int userInput = atoi(rawInput);
  setTimespan(userInput);
  printf("\nInterval to compare dates set to: %d days \n\n", userInput);
  return 0;
}


int errorMessage(int info) 
{
    printf("\n┌─┐┌─┐┌─┐┌─┐┬  ┌─┐\n├─┘├┤ │ │├─┘│  ├┤ \n┴  └─┘└─┘┴  ┴─┘└─┘\n");

    if(info == 0) {   
        printf("\nThis program was made by Callum Beaney in 2023, distributed under an MIT License.");
        printf("\nPeople was written in C because Callum got bored doing it in Python.");

        // if the below method isn't supported by the user terminal the link will be displayed as plain text.
        char* url = "https://github.com/CallumBeaney/people";
        printf("\nTo read the source code or report an issue, click here: \033]8;%s\007%s\033]8;;\n\033[0m", url, url); 
        // must have "\n\033[0m" including the newline after the hyperlink-mode code to "unset" said mode & avoid terminal display problems
        printf("\n");
    }

    printf("\nSyntax:\tpeople\tadd\t[person's name]\n\tpeople\tcheck\t[person's name OR substring thereof]\n\tpeople\tcheck\tall\n\tpeople\tforget\t[person's name]\n\tpeople\tforget\tall\n\tpeople\tdays\t[number]\n\tpeople\tinfo\n\nE.g.\tpeople\tadd\tAmy from work\n\tpeople\tcheck\tJohn Wick\n\tpeople\tcheck\twork\n\tpeople\tdays\t96\n\n");
    printf("ADD:\tDesired case and spelling is important.\nCHECK:\tcase-insensitive substring OK\n\te.g. 'jon' picks up 'Jonathan Smith' and 'JON from work'.\nFORGET:\tCase not important; correct spelling of the full name important.\nDAYS:\tUsed as a benchmark against the numbers of days since you last contacted people in your list.\n\tWhen it has been too long, you will get a ! WARNING\n\n");
    
    return info;
}