#include "includes.h"
#ifndef STR_UTILS
#define STR_UTILS

int free_array(void **arr, int size);
void clear_terminal(void);
void handle_cjson_error(void);
int read_lines(unsigned char *dst, FILE *file,
               int num_lines, int maxlen);
const char *stristr(const char *haystack,
                    const char *needle);
void disable_echo(struct termios *oldt, struct termios *newt) ;
void restore_echo(struct termios *oldt) ;
#endif // !STR_UTILS
