/*this file is fo globaly defined arrays and the
 counts of their elements*/
#include "arrays_counters.h"

/*make sure to increase the number of initialization info 
 if the .h file whenever you add a new element to this list*/
const char *list_of_config_parameters[] = 
  {"encryption"};


const char *account_creds_list[] = 
  {"user","email", "password" };


const char *list_of_configs_clarifications[] = 
  {"you can chose one of those encryption scheme that \
will be used to encrypt your accounts :\n\n\t\
1) AES-256 \n\t2) AES-192\n\t3) AES-128\n\t4) \
CAMELIA 128\n\t5) CAMELIA 192\n\t6) CAMELIA 256\n\t\
","blah blah"};

const char *list_of_accounts_clarifications[] = 
  {"lulz","blah","blah"};
int read_lines(unsigned char * dst ,FILE *file, int num_lines, int maxlen) {
    if (NULL == file || NULL == dst) {
        return errno = ERROR_NULL_VALUE_GIVEN;  // Invalid file pointer or line count
  }
  if (num_lines <= 0)
    return num_lines;

  char line[MAXLEN] = "";
  int line_count = 0;

  while (line_count < num_lines) {
    if (  fgets(line, sizeof(line), file) != NULL )
    {
      strncat((char *)dst,line,maxlen) ; // Print or process each line here
      line_count++;
      continue;
    }
    break;
  }


    // Check if we read exactly the requested number of lines
    return line_count == num_lines ? errno=SUCCESS : (errno = ERROR_BAD_ARGUMENT);
}
