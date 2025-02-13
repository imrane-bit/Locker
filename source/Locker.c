#include "Locker.h"

int main(int argc, char *argv[]) {
  /*this program works by first scannig the files containing
   * the encrypted accounts place them in an array ,
   * decrypting them and do any reading or modification
   * needed then rewriting them into the file*/


  /*used when getting the password in order not to print
   * it while the user enters it*/
  struct termios oldt, newt;

  /* those handle cased where the user interrupts the program*/
  signal(SIGINT, signal_handler);  // Ctrl+C
  signal(SIGTSTP, signal_handler); // Ctrl+Z
  
  /*the accs are stored in a json array the json data
   * structure is provided by the Cjson library*/

  long MAX_PATH_LENGHT = get_system_max_path();
  int number_of_accounts;
  /* those two are the arrays , the latter is used in the
   * encrypting and writing process while the first is what
   * we read/modify during the runtime*/
  cJSON **items = calloc(ACCOUNTS_INFO , sizeof(cJSON *));

  cJSON **json_accounts_array = NULL;
  cJSON **json_accounts_array_temp = NULL;
  /*this will have the configs , encryption options etc*/
  cJSON *configs_json = NULL;
  /*the folder that has the backup account files
   * of different users*/
  char *backup_folder =
      calloc(MAX_PATH_LENGHT , sizeof(char));
  /*the file of the backup*/
  char *user_accounts_backup =
      calloc(MAX_PATH_LENGHT , sizeof(char));
  /*the folder that has the users accounts*/
  char *user_accounts =
      calloc(MAX_PATH_LENGHT , sizeof(char));
  /*same with configs folder*/
  char *accounts_folder =
      calloc(MAX_PATH_LENGHT , sizeof(char));
  /*the path to the file of the accont of that specific
   * users*/
  FILE *accounts_file = NULL;
  /*accounts backup file*/
  FILE *backup_accounts_file = NULL;
  /*same*/
  FILE *configs_file = NULL;
  /*this is for storing the parsed strings from the jsons*/
  char *json_str = NULL;
  /*same but with the configs jsons instead of the
   * accounts*/
  char *configs_json_str = malloc(
      MAXLEN * NUMBER_OF_CONFIGS * 2 * sizeof(char) + 2);
  /*to silence valgrind*/
  memset(configs_json_str, 0,
         MAXLEN * NUMBER_OF_CONFIGS * 2 * sizeof(char));
  /*the folder that has the configs of each user
   * ie :encryption type*/
  char *configs_folder = NULL;
  configs_folder = calloc(MAX_PATH_LENGHT , sizeof(char));
  /*the actual file of that specific user's config file*/
  char *user_configs = NULL;
  user_configs = calloc(MAX_PATH_LENGHT , sizeof(char));
  /*the encryption scheme that this user have chosen
   * during the creating of the user*/

  const EVP_CIPHER *(*encryption_scheme)(void) = NULL;
  /*see the usage of those two */
  cJSON *encryption_item = NULL;
  /* this is used to store a choice in the modify acc
   * function*/
  char *modify_choice = NULL;
  char *account_choice = NULL;
  long account_num, num = 0;
  char *endptr;

  /*this is for the case of deleting an account*/
  char *delete_acc_number_str = malloc(100 * sizeof(char));
  int delete_acc_number = 0;

  /* this handles the commandline arguments*/

  /*keeps track of the commandline options */
  int c;
  /*those will hold the values later for login*/
  char *username, *password, *password_length_str,
      *random_password, *searchword;
  username = calloc(MAXLEN , sizeof(char) + 1);
  password = calloc(MAXLEN , sizeof(char) + 1);
  searchword = calloc(MAXLEN , sizeof(char) + 1);
  password_length_str = calloc(10 , sizeof(char));
  int password_length = 0;

  /*this is a key derived from the password*/
  unsigned char *key;
  key = calloc(KEY_SIZE_256 , sizeof(char));
  /*flags to check if an option is given*/
  int uflg = 0, Pflg = 0, rflg = 0;
  int mflg = 0, vflg = 0, bflg = 0;
  int iflg = 0, nflg = 0, dflg = 0;
  int sflg = 0, Rflg = 0, Dflg = 0;

  /*getting commandline options and inceasing flags*/
  while ((c = getopt(argc, argv, ":u:P:ar:mvbinds:RD:")) !=
         -1) {
    switch (c) {
    case 'u':
      uflg++;
      /*if -u get username*/
      strncpy(username, optarg, MAXLEN);
      break;
    case 'P':
      Pflg++;
      break;
    case 'b':
      bflg++;
      break;
    case 'm':
      mflg++;
      break;
    case 'n':
      nflg++;
      break;
    case 'r':
      /*getting the lengh of a random password to
       * generate it*/
      strncpy(password_length_str, optarg, 10);
      password_length =
          strtol(password_length_str, &endptr, 10);
      if (*endptr != '\0' || num <= 0) {
        printf("Invalid argument: %s\n",
               password_length_str);
        goto free_stuff;
      }
      rflg++;
      break;
    case 'D':
      /*getting the number the account to delete*/
      strncpy(delete_acc_number_str, optarg, 100);
      delete_acc_number =
          strtol(delete_acc_number_str, &endptr, 10);
      if (*endptr != '\0') {
        printf("Invalid argument: %s\n",
               delete_acc_number_str);
        goto free_stuff;
      }
      Dflg++;
      break;
    case 'v':
      vflg++;
      break;
    case 'i':
      iflg++;
      break;
    case 'd':
      dflg++;
      break;
    case 'R':
      Rflg++;
      break;
    case 's':
      sflg++;
      strncpy(searchword, optarg, MAXLEN);
      break;
    /*misuse handling*/
    case ':':
      fprintf(stderr, "Option -%c requires an operand\n",
              optopt);
      goto free_stuff;
      break;
    case '?':
      fprintf(stderr, "Unrecognized option: '-%c'\n",
              optopt);
      goto free_stuff;
      break;
    }
  }

  

  /*handling flags */

  switch (vflg) {
  case 0:
    break;
  default:
    printf("version %.2f\n", VERSION);
    goto free_stuff;
    return SUCCESS;
    break;
  }
  if (rflg != 0) {
    random_password =
        /* +1 for the null character*/
        malloc((password_length + 1) * sizeof(char));
    if (NULL == random_password) {
      errno = ERROR_MEMORY_ALLOCATION;
      goto free_stuff;
    }
    randpass(password_length, random_password, MAXLEN);
    if (SUCCESS != errno) {
      goto free_stuff;
      return errno;
    }
    printf("\n randomized password :\t%s\n",
           random_password);
    free(random_password);
    goto free_stuff;
  }
  /*checking that a username and a password were created
   * before preceeding */
  if (uflg != 1  && iflg == 0) {
    log_error("provide one username ");
    goto free_stuff;
    return SUCCESS;
  }
  /*initializing the program at first use to create
   * necessary folders and such*/
  if (iflg != 0) {
    simple_initialize();
    goto free_stuff;
  }

  /*getting the users password*/


  /*this is to hide the password 
   * when the user types*/
disable_echo(&oldt, &newt);
get_password:
  printf("hello %s,please provide the \
  password\npassword>",username);
  fgets(password,MAXLEN,stdin);
  if(2 > strlen(password))
  {
    printf("\npassword too short\n");
    goto get_password;
  }
restore_echo(&oldt);
  /*authentication of the user */

  simple_login(username, password);
  switch (errno) {
  case SUCCESS:
    printf("\nlogin success\n");
    break;
  default:
    goto free_stuff;
    return errno;
    break;
  }
  /*creating the encryption key from the user password*/

  hashing_global((const char *)password,
                 (const unsigned char *)username,
                 SHA256_SALT_SIZE, 1, key, KEY_SIZE_256,
                 EVP_ripemd160);
  if (SUCCESS != errno) {
    goto free_stuff;
    return errno;
  }
  memset(password, 0, MAXLEN * sizeof(char));

  /*defining the path of the folder of users accs
   * previously declared*/
  if (SUCCESS != define_paths(NULL, NULL, configs_folder,
                              accounts_folder,
                              backup_folder, MAXLEN, pwd))
    goto free_stuff;
  /*defining the path to the exact file that has that
   * user's accs */
  make_file_path(user_accounts, accounts_folder,
                 (const char *)username, MAXLEN);
  if (SUCCESS != errno)
    goto free_stuff;
  make_file_path(user_accounts_backup, backup_folder,
                 (const char *)username, MAXLEN);

  if (SUCCESS != errno)
    goto free_stuff;

  make_file_path(user_configs, configs_folder,
                 (const char *)username, MAXLEN);

  if (SUCCESS != errno) {
    goto free_stuff;
  }
  if (Rflg != 0) {
    accounts_file = fopen(user_accounts, "w");
    if (NULL == accounts_file) {
      goto free_stuff;
    }
    backup_accounts_file = fopen(user_accounts_backup, "r");
    if (NULL == accounts_file) {
      goto free_stuff;
    }

    size_t bytes_read;
    char buffer[MAXLEN];
    while ((bytes_read = fread(buffer, 1, MAXLEN,
                               backup_accounts_file)) > 0) {
      size_t bytes_written =
          fwrite(buffer, 1, bytes_read, accounts_file);
      if (bytes_written != bytes_read) {
        perror("error while copying backup");
        goto free_stuff;
      }
    }
    fclose(accounts_file);
    fclose(backup_accounts_file);
    accounts_file = NULL;
    backup_accounts_file = NULL;
  }

  /*now opening the accounts file and doing the decryption
   */
  accounts_file = fopen(user_accounts, "r");
  if (NULL == accounts_file) {
    goto free_stuff;
  }

  configs_file = fopen(user_configs, "r");
  if (NULL == configs_file) {
    goto free_stuff;
  }
  /*getting the json that has the configs*/
  /*that +2 is for the brackets of the json*/
  read_lines((unsigned char *)configs_json_str,
             configs_file, NUMBER_OF_CONFIGS + 2, MAXLEN);
  if (errno != SUCCESS)
    goto free_stuff;

  fclose(configs_file);
  // Parse the JSON
  configs_json =
      cJSON_Parse((const char *)configs_json_str);
  if (NULL == configs_json) {
    handle_cjson_error();
    goto free_stuff;
  }

  encryption_item = cJSON_GetObjectItemCaseSensitive(
      configs_json, "encryption");

  if (NULL == encryption_item) {
    handle_cjson_error();
    goto free_stuff;
  }
  if (SUCCESS != errno)
    goto free_stuff;
  /*setting the encryption_scheme after we parsed the
   * configs*/

  switch (atoi(encryption_item->valuestring)) {
  case 1:
    encryption_scheme = EVP_aes_256_cbc;
    break;
  case 2:
    encryption_scheme = EVP_aes_192_cbc;
    break;
  case 3:
    encryption_scheme = EVP_aes_128_cbc;
    break;
  case 4:
    encryption_scheme = EVP_camellia_128_cbc;
    break;
  case 5:
    encryption_scheme = EVP_camellia_192_cbc;
    break;
  case 6:
    encryption_scheme = EVP_camellia_256_cbc;
    break;
  }

  for (number_of_accounts = 0; SUCCESS == errno;
       ++number_of_accounts) {
    if (NULL ==
        (json_accounts_array = realloc(
             json_accounts_array,
             (number_of_accounts + 1) * sizeof(cJSON *)))) {
      errno = ERROR_MEMORY_ALLOCATION;
      goto free_stuff;
    }
    get_next_json_from_file(
        &json_accounts_array[number_of_accounts],
        (unsigned char *)username, key, encryption_scheme,
        accounts_file);

    /* number_of_accounts+1 since we need some memory at
     * first when the number_of_accounts is 0 , then
     * there's always one allocation ahead*/
  }
  errno = SUCCESS;
  number_of_accounts -= 1;
  /*cause it fails once but the number.. still increases
   * by one until the next check the error isnt flaged*/

  fclose(accounts_file);

  /*handling the creation of a new user*/
  if (nflg != 0) {
    if (NULL == json_accounts_array &&
        NULL == (json_accounts_array =
                     realloc(json_accounts_array,
                             (number_of_accounts + 1) *
                                 sizeof(cJSON *)))) {
      return ERROR_MEMORY_ALLOCATION;
    }

    new_account(json_accounts_array, &number_of_accounts);

    if (SUCCESS != errno) {
      goto free_stuff;
    }
    number_of_accounts += 1;
    printf("new acc have been created m there are now %d "
           "accs\n",
           number_of_accounts);
    json_str = (char *)cJSON_Print(
        json_accounts_array[number_of_accounts - 1]);
    if (NULL == json_str) {
      errno = ERROR_JSON_PRINTING;
      goto free_stuff;
    }
    free(json_str);
  }

  /*displaying the accounts*/
  if (dflg != 0) {
    display_accounts(json_accounts_array,
                     number_of_accounts, account_creds_list,
                     ACCOUNTS_INFO, NULL);
    sleep(SLEEP_DURATION);
    clear_terminal();
  }
  if (sflg != 0) {
    display_accounts(json_accounts_array,
                     number_of_accounts, account_creds_list,
                     ACCOUNTS_INFO, searchword);
    sleep(SLEEP_DURATION);
    clear_terminal();
  }
  if (Dflg != 0) {
    if (delete_acc_number > number_of_accounts ||
        delete_acc_number < 0) {
      printf("account number out of range\n");
      errno = ERROR_BAD_ARGUMENT;
      goto free_stuff;
    }
    printf("deleting account %d:\n", delete_acc_number);
    display_account(json_accounts_array[delete_acc_number],
                    account_creds_list, ACCOUNTS_INFO);
    if (SUCCESS != errno)
      goto free_stuff;
    cJSON_Delete(json_accounts_array[delete_acc_number]);
    json_accounts_array[delete_acc_number] = NULL;
  }

  if (mflg != 0) {
    account_choice = malloc(10 * sizeof(char *));
    modify_choice = malloc(10 * sizeof(char *));
    char *newline;
    if (NULL == account_choice) {
      errno = ERROR_MEMORY_ALLOCATION;
      goto free_stuff;
    }
    if (NULL == modify_choice) {
      errno = ERROR_MEMORY_ALLOCATION;
      goto free_stuff;
    }
  get_mod_number:
    printf("What do you want to modify :\n");
    for (int i = 0; i < ACCOUNTS_INFO; i++) {
      printf("\t%d)%s\n", i, account_creds_list[i]);
    }
    printf(">");
    fgets(modify_choice, 10, stdin);

    num = strtol(modify_choice, &endptr, 10); // Base 10
    newline = strchr(modify_choice, '\n');
    if (newline) {
      *newline = '\0';
    }
    if (*endptr != '\0' || num >= ACCOUNTS_INFO ||
        num < 0) {
      clear_terminal();
      printf("Invalid input: %s\n", modify_choice);
      goto get_mod_number;
    }
  get_account_number:
    printf("What account you wanna modify\
    (provide its number) : ");
    fgets(account_choice, 10, stdin);

    char *endptr;
    account_num =
        strtol(account_choice, &endptr, 10); // Base 10

    newline = strchr(account_choice, '\n');
    if (newline) {
      *newline = '\0';
    }
    if (*endptr != '\0' ||
        account_num > number_of_accounts || num < 0) {
      clear_terminal();
      printf("Invalid input: %s\n", account_choice);

      if (NULL == account_choice) {
        errno = ERROR_MEMORY_ALLOCATION;
        goto free_stuff;
      }
      goto get_account_number;
    }

    char *newvalue = malloc(MAXLEN * sizeof(char));
    printf("enter the new value >");
    fgets(newvalue, MAXLEN, stdin);

    modify_json(json_accounts_array[account_num],
                account_creds_list[num], newvalue, MAXLEN);
    if (newvalue)
      free(newvalue);
    if (account_choice)
      free(account_choice);
    if (modify_choice)
      free(modify_choice);
  }

  /* encrypting the accounts,
   * opening the accounts file
   * and then wrinting them to it  */

  if (NULL != json_accounts_array) {
    for (int i = 0; i < number_of_accounts; i++) {
      if (NULL == (json_accounts_array_temp =
                       realloc(json_accounts_array_temp,
                               (i + 1) * sizeof(cJSON *))))
        return ERROR_MEMORY_ALLOCATION;

      if (NULL == json_accounts_array[i])
        continue;
      encrypt_json(json_accounts_array[i],
                   json_accounts_array_temp + i,
                   (unsigned char *)username, key,
                   encryption_scheme);

      if (SUCCESS != errno) {
        goto free_stuff;
      }

      for (int j = 0; j < ACCOUNTS_INFO; j++) {
        items[j] = cJSON_GetObjectItemCaseSensitive(
            json_accounts_array[i], account_creds_list[j]);
        if (NULL == items[j]) {
          printf("null\n");
          handle_cjson_error();
          continue;
        }
        memset(items[j]->valuestring, 0,
               strlen(items[j]->valuestring) *
                   sizeof(char));
      }
    }
    accounts_file = fopen(user_accounts, "w");
    for (int i = 0; i < number_of_accounts; i++) {
      if (NULL == json_accounts_array[i]) {
        continue;
      }
      json_str =
          (char *)cJSON_Print(json_accounts_array_temp[i]);
      if (NULL == json_str) {
        handle_cjson_error();
        goto free_stuff;
      }
      fputs(json_str, accounts_file);
      fputs("\n", accounts_file);
      free(json_str);
    }
    if (bflg != 0) {
      backup_accounts_file =
          fopen(user_accounts_backup, "w");
      for (int i = 0; i < number_of_accounts; i++) {
        if (NULL == json_accounts_array[i])
          continue;
        json_str = (char *)cJSON_Print(
            json_accounts_array_temp[i]);
        if (NULL == json_str) {
          handle_cjson_error();
          goto free_stuff;
        }
        fputs(json_str, backup_accounts_file);
        fputs("\n", backup_accounts_file);
        free(json_str);
      }
    }
    free_cjson_array(json_accounts_array,
                     number_of_accounts);
    free_cjson_array(json_accounts_array_temp,
                     number_of_accounts);
  }
free_stuff:
  if (items)
    free(items);
  if (accounts_file)
    fclose(accounts_file);
  if (backup_accounts_file)
    fclose(backup_accounts_file);
  if (username)
    free(username);
  memset(key, 0, KEY_SIZE_256);
  if (key)
    free(key);
  if (password)
    free(password);
  if (searchword)
    free(searchword);
  if (accounts_folder)
    free(accounts_folder);
  if (backup_folder)
    free(backup_folder);
  if (user_accounts_backup)
    free(user_accounts_backup);
  if (user_configs)
    free(user_configs);
  if (user_accounts)
    free(user_accounts);
  if (configs_folder)
    free(configs_folder);
  if (configs_json_str)
    free(configs_json_str);
  if (password_length_str)
    free(password_length_str);
  if (configs_json)
    cJSON_Delete(configs_json);
  if (delete_acc_number_str)
    free(delete_acc_number_str);
  printf("%d\n", errno);
  return errno;
}
