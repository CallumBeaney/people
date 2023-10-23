#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h>

#include "mainFunctions.h"


int main(int argc, char *argv[])
{
    int result;

    if (argc == 1) {   
        return result = checkAllPersons(); /// if they are just running ./people
    }
    else if (argc == 3 && strcmp(argv[1], "forget") == 0 && strcmp(argv[2], "all") == 0) {
        return result = forgetAll(); /// people forget all
    }
    else if (argc >= 3 && strcmp(argv[1], "forget") == 0) {
        return result = forgetPerson(argc, argv); /// people forget name name 
    } 
    else if (argc == 3 && strcmp(argv[1], "days") == 0) {
       return result = setNumberOfDaysBenchmark(argv); /// people days [number] 
    }
    else if ((argc >= 3) && strcmp(argv[1], "add") == 0) {
        return result = addPerson(argc, argv); /// people add name name
    }
    else if (argc == 3 && strcmp(argv[1], "check") == 0 && strcmp(argv[2], "all") == 0) { 
        return result = checkAllPersons();  /// people check [all]  
    }
    else if (strcmp(argv[1], "check") == 0 && argc >= 3) { 
        return result = checkSpecificPerson(argc, argv); /// people check [name OR substring thereof] 
    } 
    else if (argc == 2 && strcmp(argv[1], "info") == 0) {
        return result = errorMessage(0); // User doesn't know how to use the program and wants to know
    } 
    else {
        return result = errorMessage(1); // User doesn't know how to use the program and needs help
    }
}
