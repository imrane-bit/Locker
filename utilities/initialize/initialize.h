// char **initialize(char** listofwantedinfo , int numberofinfos , int maxlengh);
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBEROFINF 2 //number of thing that we will ask the user for
#define MAXLENGH 1000 //maximum lengh of the user input

//requests the stuff in a array from the user and returns an array that includes the user answer
char **getinfo(char** listofwantedinfo , int numberofinfos , int maxlengh);
char **initialize();
