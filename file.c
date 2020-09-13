#include "file.h"
#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *multi_tok(char *input, char *delimiter) {
  static char *string;
  if (input != NULL)
    string = input;

  if (string == NULL)
    return string;

  char *end = strstr(string, delimiter);
  if (end == NULL) {
    char *temp = string;
    string = NULL;
    return temp;
  }

  char *temp = string;

  *end = '\0';
  string = end + strlen(delimiter);
  return temp;
}

/*
 * Insert a record to a dict
 */
void insert_record(Dictionary *dict, Record *record) {
  if (dict->header == NULL) {
    dict->header = record;
  } else if (dict->first_record == NULL) {
    dict->first_record = record;
    dict->last_record = record;
  } else {
    dict->last_record->next = record;
    dict->last_record = record;
  }
}

/*
 * Parse a comma seperated file then return a Dictionary
 */
Dictionary *parse_file(FILE *stream) {
  char line[BUFFER_SIZE];

  Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));

  // starting at -1 because we need to exclude the header from the count
  dict->length = -1;

  if (dict) {
    while (fgets(line, BUFFER_SIZE, stream)) {
      Record *record = parse_line(line);
      if (record == NULL) {
        continue;
      }

      insert_record(dict, record);
      dict->length++;
    }
  }

  return dict;
}

/*
 * Parse a comman seperated line then return a record
 */
Record *parse_line(char *line) {
  Record *record = (Record *)malloc(sizeof(Record));
  if (record == NULL) {
    return record;
  }

  int i;
  char *tok;
  for (tok = strtok(line, ","), i = 0; tok && *tok && i <= LOCATION_POSITION;
       tok = strtok(NULL, ",\n\r"), i++) {
    // whether or not tok needs to be freed after
    int needed_free = 0;

    // if the field is wrapped by quotes
    if (strlen(tok) && tok[0] == '"') {
      // trim quotes from the string
      tok++;

      // if the string is already ended
      if (!(tok[strlen(tok) - 1] == '"')) {
        char *complete_field = malloc(strlen(tok) + 1);
        strcpy(complete_field, tok);

        // loop until the field is ended
        while (1) {
          char *other_half = strtok(NULL, ",\n\r");

          // allocate more space for the field and copy them
          complete_field = realloc(
              complete_field,
              (strlen(complete_field) + strlen(other_half) + 2) * sizeof(char));
          strcat(complete_field, ",");
          strcat(complete_field, other_half);

          // when the field is ended
          if (other_half[strlen(other_half) - 1] == '"') {
            // trim the right quote from the string
            complete_field[strlen(complete_field) - 1] = '\0';
            tok = complete_field;

            break;
          }
        }

        needed_free = 1;
      } else {
        tok[strlen(tok) - 1] = '\0';
      }
    }

    // save to the record in accordance with the current column position
    if (i == YEAR_POSITION) {
      strcpy(record->year, tok);
    } else if (i == BLOCK_ID_POSITION) {
      strcpy(record->block_id, tok);
    } else if (i == PROPERTY_ID_POSITION) {
      strcpy(record->property_id, tok);
    } else if (i == BASE_PROPERTY_ID_POSITION) {
      strcpy(record->base_property_id, tok);
    } else if (i == CLUE_SMALL_AREA_POSITION) {
      strcpy(record->clue_small_area, tok);
    } else if (i == TRADING_NAME_POSITION) {
      strcpy(record->trading_name, tok);
    } else if (i == INDUSTRY_CODE_POSITION) {
      strcpy(record->industry_code, tok);
    } else if (i == INDUSTRY_DESCRIPTION_POSITION) {
      strcpy(record->industry_description, tok);
    } else if (i == LNG_POSITION) {
      strcpy(record->lng, tok);
    } else if (i == LAT_POSITION) {
      strcpy(record->lat, tok);
    } else if (i == LOCATION_POSITION) {
      strcpy(record->location, tok);
    }

    if (needed_free) {
      free(tok);
    }
  }

  return record;
}

/*
 * Write a record to a file in accordance with a search
 *
 * If the record is NULL then write not found
 */
void write_to_file(FILE *stream, Record *header, Record *record, char *search) {
  fprintf(stream, "%s -->", search);

  // if the record not found, write the record to the file, else write NOTFOUND
  if (record != NULL) {
    // loop through every keys and write key/value to the file
    for (int j = 0; j <= LOCATION_POSITION; j++) {
      if (j == YEAR_POSITION) {
        fprintf(stream, " %s: %s", header->year, record->year);
      } else if (j == BLOCK_ID_POSITION) {
        fprintf(stream, " %s: %s", header->block_id, record->block_id);
      } else if (j == PROPERTY_ID_POSITION) {
        fprintf(stream, " %s: %s", header->property_id, record->property_id);
      } else if (j == BASE_PROPERTY_ID_POSITION) {
        fprintf(stream, " %s: %s", header->base_property_id,
                record->base_property_id);
      } else if (j == CLUE_SMALL_AREA_POSITION) {
        fprintf(stream, " %s: %s", header->clue_small_area,
                record->clue_small_area);
      } else if (j == TRADING_NAME_POSITION) {
        continue;
      } else if (j == INDUSTRY_CODE_POSITION) {
        fprintf(stream, " %s: %s", header->industry_code,
                record->industry_code);
      } else if (j == INDUSTRY_DESCRIPTION_POSITION) {
        fprintf(stream, " %s: %s", header->industry_description,
                record->industry_description);
      } else if (j == LNG_POSITION) {
        fprintf(stream, " %s: %s", header->lng, record->lng);
      } else if (j == LAT_POSITION) {
        fprintf(stream, " %s: %s", header->lat, record->lat);
      } else if (j == LOCATION_POSITION) {
        fprintf(stream, " %s: %s", header->location, record->location);
      }
      fprintf(stream, " ||");
    }
    fprintf(stream, " ");
  } else {
    fprintf(stream, " NOTFOUND");
  }

  fprintf(stream, "\n");
}

/*
 * Trim quotes from the left side of the string
 */
char *ltrim(char *s) {
  while (*s == '"')
    s++;
  return s;
}

/*
 * Trim the right side of the string by a character
 */
char *rtrim(char *s, char pattern) {
  char *back = s + strlen(s);
  while ((*--back) == pattern)
    ;
  *(back + 1) = '\0';
  return s;
}
