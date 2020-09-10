#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"
#include "file.h"

#define NUM_OF_REQUIRED_ARGUMENTS 3

int main(int argc, char *argv[]) {

  if (argc < NUM_OF_REQUIRED_ARGUMENTS) {
    printf("Usage: %s infile outfile [keys]\n", argv[0]);
    return 0;
  }

  // load in the file and parse the dictionary
  FILE *stream = fopen(argv[1], "r");
  if (stream == NULL) {
    printf("Failed to open %s\n", argv[1]);
    return 1;
  }
  Dictionary *dict = parse_file(stream);
  if (dict == NULL) {
    printf("Failed to parse dictionary\n");
    return 1;
  }
  fclose(stream);

  // get all the searches
  char **searches;
  int num_of_searches = 0;

  // if the number of arguments is greater than 3 we get the searches from the
  // command line else get the searches from the stdint
  if (argc == NUM_OF_REQUIRED_ARGUMENTS) {
    // allocate memory for all the searches
    int memory_bound = 4;
    searches = malloc(memory_bound * sizeof(char *));

    // read through the searches from stdint one by one
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, stdin)) {
      // copy the search string and strim the new line off it
      char *text = rtrim(strdup(buffer), '\n');

      // allocate memory for the search string and store it
      searches[num_of_searches] = (char *)malloc((strlen(text) * sizeof(char)));
      strcpy(searches[num_of_searches], text);
      free(text);

      num_of_searches++;

      // reallocate the memory stored for searches if we exceed the allocated
      // memory
      if (num_of_searches == memory_bound) {
        memory_bound *= 2;
        searches = realloc(searches, memory_bound * sizeof(char *));
      }
    }
  } else {
    num_of_searches = argc - NUM_OF_REQUIRED_ARGUMENTS;

    // allocate memory to store the searches
    searches = malloc(num_of_searches * sizeof(char *));
    if (searches == NULL) {
      printf("Cannot allocate memory for the searches\n");
      return 1;
    }

    // loop through the remain arguments and store them to the searches array
    for (int i = 0; i < num_of_searches; i++) {
      int search_index = i + NUM_OF_REQUIRED_ARGUMENTS;

      searches[i] =
          (char *)malloc((strlen(argv[search_index]) + 1) * sizeof(char));
      strcpy(searches[i], argv[search_index]);
    }
  }

  // get the file for writting the result in
  FILE *fp = fopen(argv[2], "w");
  if (fp == NULL) {
    printf("Cannot open %s\n", argv[3]);
    return 1;
  }

  // loop through all the strings that needed to search and search
  // one by one in the dict, then write them to the file
  for (int i = 0; i < num_of_searches; i++) {
    int is_found = 0;
    char *search = searches[i];

    Record *record = dict->first_record;
    while (record != NULL) {
      // if they're matched by their trading name then we've found the element
      if (strcmp(record->trading_name, search) == 0) {
        write_to_file(fp, dict->header, record, search);
        is_found = 1;
      }
      record = record->next;
    }

    if (!is_found) {
      write_to_file(fp, NULL, NULL, search);
    }

    free(search);
  }

  fclose(fp);

  dispose_dict(dict);
  free(searches);
}

/*
 * Free the dictionary from the memory
 */
void dispose_dict(Dictionary *dict) {
  Record *record = dict->first_record;
  while (record->next && record != NULL) {
    Record *next = record->next;
    free(record);

    record = next;
  }

  free(dict->header);
  free(dict->last_record);
  free(dict);
}