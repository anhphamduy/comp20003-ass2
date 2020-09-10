#include "dict.h"
#include <stdio.h>

#ifndef FILE_H
#define FILE_H

#define BUFFER_SIZE 512

#define YEAR_POSITION 0
#define BLOCK_ID_POSITION 1
#define PROPERTY_ID_POSITION 2
#define BASE_PROPERTY_ID_POSITION 3
#define CLUE_SMALL_AREA_POSITION 4
#define TRADING_NAME_POSITION 5
#define INDUSTRY_CODE_POSITION 6
#define INDUSTRY_DESCRIPTION_POSITION 7
#define LNG_POSITION 8
#define LAT_POSITION 9
#define LOCATION_POSITION 10

Dictionary *parse_file(FILE *stream);
Record *parse_line(char *line);
void write_to_file(FILE *stream, Record *header, Record *record, char *search);

char *ltrim(char *s);
char *rtrim(char *s, char pattern);

#endif
