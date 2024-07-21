
#include "includes.h"
#ifndef EROR_HANDLING_H

#define EROR_HANDLING_H




enum ErrorCode {
  LIBSSL_SUCCESS = 1,
  SUCCESS = 0,
  ERROR_INDEX_OUT_OF_BOUNDS = -1,
  ERROR_MEMORY_ALLOCATION = -2,
  ERROR_INVALID_STRING = -3,
  ERROR_INVALID_INTEGER = -4,
  ERROR_UNKNOWN_TYPE = -5,
  ERROR_NULL_VALUE_GIVEN =  -6,
  UNINITIALIZED_ACCOUNT_GIVEN = -7,
  ERROR_TOO_LONG_STRING = -8,
  ERROR_EMPTY_STRING = -9,
  ERROR_FILE_OPENING_FAILED = -10,
  ERROR_INVALID_CONFIG_LINE = -11,
  ERROR_JSON_ADDING_ITEM_TO_OBJ = -12,
  ERROR_JSON_GETTING_ITEM_TO_OBJ = -13,
  ERROR_JSON_OBJECT_CREATION = -14,
  ERROR_JSON_NOT_STR = -15,
  ERROR_NULL_JSON_ITEM = -16,
  ERROR_STDLIB_FUNTION_FAILURE = -17,
  ERROR_LIBSSL_FAILURE = -18,
  ERROR_INVALID_CREDENTIALS = -19,
  ERROR_FILE_DOESNT_EXIST = -20,
  ERROR_CANNOT_READ_FILE = -23,
  ERROR_USER_DOESNT_EXIST =-24,
  ERROR_INVALID_USER_FILE = -25,
  ERROR_REGISTER_USER_EXISTS = -26,
  ERROR_CANNOT_WRITE_TO_FILE = -27,

};

void log_error( const char *message);
#endif 
