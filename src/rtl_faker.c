#include <pebble.h>
#include "rtl_faker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_WORDS_PER_LINE 3

int words_in_string=0;
int total_lines=0;
int chars_in_str=0;



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

}

char* recombine_str(char** str){
  int recombine_len = chars_in_str + total_lines;
  char* rec_str = malloc(recombine_len+1);
  if(rec_str){
    for(int i = 0; i<words_in_string;i++){
      if(i%MAX_WORDS_PER_LINE==0 && i!=0){
        strncat(rec_str, "\n", 2);
      }
      strncat(rec_str, str[i], strlen(str[i]));
      strncat(rec_str, " ", 1);
    }
  }
  return rec_str;
}

void reverse_str(char* str){
  int string_len = strlen(str);
  char temp;
  int   i = 0;
  int j = string_len - 1;

  while (i < j) {
    temp = str[i];
    str[i] = str[j];
    str[j] = temp;
    i++;
    j--;
  }
}

void reverse_tokens(char** tokens){
  char* temp;
  int i = 0;
  int j = words_in_string -1;
  while(i<j){
    temp = tokens[i];
    tokens[i]=tokens[j];
    tokens[j]=temp;
    i++;
    j--;
  }
}

void reverse( char *start, char *end )
{
  while( start < end )
  {
    char c = *start;
    *start++ = *end;
    *end-- = c;
  }
}

char *reverse_char( char *start )
{
  char *end = start;
  while( (end[1] & 0xC0) == 0x80 ) end++;
  reverse( start, end );
  return( end+1 );
}

void reverse_string( char *string )
{
  char *end = string;
  while( *end ) end = reverse_char( end );
  reverse( string, end-1 );
}


char* fake_rtl(char* src){
  reverse_string(src);
  chars_in_str = strlen(src);
  char** tokens;
  tokens = str_split(src);
  swap_words(tokens);
  reverse_tokens(tokens);
  //reverse each token
  char* ret_string = recombine_str(tokens);
  APP_LOG(APP_LOG_LEVEL_DEBUG, ret_string);
  free(tokens);
  return ret_string;
}

