#include <pebble.h>
#include "rtl_faker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_WORDS_PER_LINE 5

int words_in_string=0;
int total_lines=0;
int chars_in_str=0;

char* strdup(	const char *str){
  char *copy;
  copy = malloc(strlen(str)+1);
  strncpy(copy,str,strlen(str)+1 );
  return copy;
}

char* str_split_at_first_delim(char* a_str)
{
  char *r = a_str;
  while(*r != ' ') {
    r++;
  }
  *r++ = '\0';
  // left part is l, right part is r
  return r;
}

char** str_split(char* src){
  char** ret_string;
  int word_count = 0;
  char* temp = src;


  //calculate words in source string based on spaces
  while(*temp != '\0'){
    if(*temp == ' ')
      word_count++;
    temp++;
  }
  //TODO: check if space as terminal charecter
  word_count++;

  words_in_string = word_count;

  ret_string=malloc(word_count * sizeof(char*));

  if(ret_string){
    for(int i=0;i<word_count;i++){
      ret_string[i]=src;
      src = str_split_at_first_delim(src);
    }
  }

  return ret_string;
}

void swap_word_section(char** strs, int words_per_line){
  char* temp[words_per_line];
  for(int i =0; i< words_per_line;i++){
    temp[i] = strs[words_per_line - i-1];
  }
  for(int j = 0; j<MAX_WORDS_PER_LINE; j++){
    strs[j] = temp[j];
  }
}

void swap_words(char** strs){

  for(int i = 0; i<words_in_string;i+=MAX_WORDS_PER_LINE){
    int words_per_line = MAX_WORDS_PER_LINE;
    while(i+words_per_line > words_in_string){
      words_per_line--;
    }

    char* swik[words_per_line];
    for(int j = 0 ; j<words_per_line;j++){
      swik[j]=strs[i+j];
    }
    swap_word_section(swik, words_per_line);

    for(int h=0;h<words_per_line;h++){
      strs[i+h]=swik[h];
    }
    total_lines++;
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "total Lines: %d", total_lines);
}

char* recombine_str(char** str){
  int recombine_len = chars_in_str + total_lines;
  char* rec_str = malloc(recombine_len+1);
  if(rec_str){
    for(int i = 0; i<words_in_string;i++){
      if(i%MAX_WORDS_PER_LINE==0 && i!=0){
        strncat(rec_str, "\r", 2);
      }
      strncat(rec_str, str[i], strlen(str[i]));
      strncat(rec_str, " ", 1);
    }
  }
  return rec_str;
}


// char* fake_rtl(char* src){
//   char** split_pnts = str_split(src);
//   char** swaped_strs = str_swap(split_pnts);
//   char* recombined_str = concat(swaped_strs);
//   return reverse_str(recombined_str);
// }

void test_split(){
  char months[] = "JAN FEB MAR APR MAY JUN JUL AUG SEP OCT NOV DEC";
  chars_in_str = strlen(months);
  char** tokens;
  tokens = str_split(months);
  swap_words(tokens);
  char* ret_string = recombine_str(tokens);
  free(tokens);
  APP_LOG(APP_LOG_LEVEL_DEBUG, ret_string);
  free(ret_string);
}