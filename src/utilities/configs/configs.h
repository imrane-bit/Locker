//the eror code is in src/headers/eror_handling/eror_handling.h
//all the globaly defined values are  src/headers/defined_values/defined_values.h

#include <stdio.h>
#include <stdlib.h>
#include "eror_handling.h"
#include "defined_values.h"

#ifndef CONF_PARSE
#define CONF_PARSE

#define user_config_file "configs.cpassmann" 

/*each line has infotype and infovalue , 
each of maximum lengh MAXLEN plus # and 
 (2*MAXLEN +2) = 2*(MAXLEN+1)*/

#define LINE_MAX_LENGHT 2*(MAXLEN+1);

/*
 * parses a key value and a value from a line , 
 * and puts then in a pair struct
 * the line should be in the following format :
 * #key = value; */
int lineparse(pair *couple,char *line);

#endif //CONF_PARSE

