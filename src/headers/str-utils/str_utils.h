#ifndef STR_UTILS_H
#define STR_UTILS_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "eror_handling.h"

int trimspaces(char *str ,int maxlengh);

int scan_to_char(char *dest,char *src ,char character ,int maxlengh);

int skiptochar(char *tracker,char *str,char character,int maxlengh);
#endif // !STR_UTILS_H

