#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h>

#include "mainFunctions.h"


int main(int argc, char *argv[])
{
    if (argc == 1) {   
        return checkAllPersons(); /// if they are just running ./people
    }
    else if (argc == 3 && strcmp(argv[1], "forget") == 0 && strcmp(argv[2], "all") == 0) {
        return forgetAll(); /// people forget all
    }
    else if (argc >= 3 && strcmp(argv[1], "forget") == 0) {
        return forgetPerson(argc, argv); /// people forget name name 
    } 
    else if (argc == 3 && strcmp(argv[1], "days") == 0) {
       return setNumberOfDaysBenchmark(argv); /// people days [number] 
    }
    else if ((argc >= 3) && strcmp(argv[1], "add") == 0) {
        return addPerson(argc, argv); /// people add name name
    }
    else if (argc == 3 && strcmp(argv[1], "check") == 0 && strcmp(argv[2], "all") == 0) { 
        return checkAllPersons();  /// people check [all]  
    }
    else if (strcmp(argv[1], "check") == 0 && argc >= 3) { 
        return checkSpecificPerson(argc, argv); /// people check [name OR substring thereof] 
    } 
    else if (argc == 2 && strcmp(argv[1], "info") == 0) {
        return errorMessage(0); // User doesn't know how to use the program and wants to know
    } 
    else {
        return errorMessage(1); // User doesn't know how to use the program and needs help
    }

    /// the program should never get here
    char* url = "https://github.com/CallumBeaney/people";
    printf("ERROR: Your input command has fallen through the parsing tree.\nIf you have the time, please kindly report an issue to the link below: \033]8;%s\007%s\033]8;;\n\033[0m \n\n", url, url); 
    return errorMessage(1);
}
