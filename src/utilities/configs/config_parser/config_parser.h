//the eror code is in src/headers/eror_handling/eror_handling.h
//all the globaly defined values are  src/headers/defined_values/defined_values.h

#include "includes.h"
#ifndef CONF_PARSE
#define CONF_PARSE

#define USER_CONFIG_FILE "/home/qbit/coding/c/cpassremake/src/utilities/configs/configs.cpassmann" 

/*each line has infotype and infovalue , 
each of maximum lengh MAXLEN plus # and 
 (2*MAXLEN +2) = 2*(MAXLEN+1)*/

#define LINE_MAX_LENGHT (2*(MAXLEN+1))

/*
 * parses a key value and a value from a line , 
 * and puts then in a pair struct
 * the line should be in the following format :
 * #key = value; */
int parse_file(FILE *file_pointer ,pair **array_of_pairs ,size_t line_max_len,size_t str_maxlengh,size_t max_numberoflines);
#endif //CONF_PARSE

