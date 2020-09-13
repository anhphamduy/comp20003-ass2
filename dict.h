#ifndef DICT_H
#define DICT_H

typedef struct Record {
  char year[12];
  char block_id[16];
  char property_id[16];
  char base_property_id[32];
  char industry_code[128];
  char industry_description[128];
  char lng[64];
  char lat[64];
  char location[96];
  char clue_small_area[128];
  char trading_name[128];
  struct Record *next;
} Record;

typedef struct Dictionary {
  Record *first_record;
  Record *last_record;
  Record *header;
  int length;
} Dictionary;

void dispose_dict(Dictionary *);

#endif