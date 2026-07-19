/* 
 * testing out xx_file_reader.h file
 * this one is very simple and does not need sdl 
 * */
#include <stdio.h>
#include <stdlib.h>

#include "../xx_string.h"
#include "../xx_file_reader/xx_file_reader.h"

int main ( int argc, char** argv ) {
  xx_file_reader* r_file = xx_create_file_reader ( (char*) "../xx_file_reader/test_text.txt" );
  if ( r_file == NULL ) {
    printf ("\t Could not create file reader \n");
    return -1;
  }

  // reading from the file
  xx_read_from_file_reader ( r_file );
  
  for (int i = 0; i < xx_get_file_reader_line_number (r_file); i++) {
    printf ("%s", xx_get_file_reader_string_n ( r_file, i ));
  }

  xx_free_file_reader ( r_file );

  return 0;
}
