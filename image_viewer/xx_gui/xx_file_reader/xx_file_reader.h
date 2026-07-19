/* 
 * it's nothing like FILE* which does the core part of the file loading to memory and then accessing it.
 * This header file is solely for a simple text editor
 * */
#ifndef XX_STRING
#include "../xx_string.h"
#endif

#ifndef XX_LIST
#include "../xx_list.h"
#endif

// #ifndef BM_MGR
// #include "../"

typedef struct {
  FILE* reading_file;
  char* file_path;
  long int file_size;

  int file_line_n; // number of lines in the file 
  int max_string_size_per_line; // default will be 150
  xx_list* my_list;

  char* m_string; // main string that can be modified anywhere by any function in this program
} xx_file_reader;

/* 
 * function - declarations
 * */
// some functions to work with files
long xx_get_file_size ( char* file_path );
int xx_get_no_of_file_line ( char* file_path );

xx_file_reader* xx_create_file_reader ( char* i_path );
void xx_free_file_reader ( xx_file_reader* i_file );
int xx_read_from_file_reader ( xx_file_reader* i_file );

/* getters */
long xx_get_file_reader_size ( xx_file_reader* i_file );
int xx_get_file_reader_line_number ( xx_file_reader* i_file );
char* xx_get_file_reader_string_n ( xx_file_reader* i_file, int n ); // return the string at that n index

/* 
 * function - definitions
 * */
xx_file_reader* xx_create_file_reader ( char* i_path ) {
  xx_file_reader* r_file = (xx_file_reader*) malloc (sizeof(xx_file_reader) * 1);
  if ( r_file == NULL ) {
    printf ("\t Error! Memory Allocation, Failed, xx_file_reader* r_file\n");
    return NULL;
  }

  r_file -> file_path = (char*) malloc (sizeof(char) * 25);
  if ( r_file -> file_path == NULL ) {
    printf ("\t Error! Memory Allocation, Failed, xx_file_reader* r_file -> char* file_path\n");
    return NULL;
  }

  // copying the name to file path 
  xx_strcpy ( r_file -> file_path, i_path );

  // getting the size of file 
  r_file -> file_size = xx_get_file_size ( r_file -> file_path );
  if ( r_file -> file_size == -1 ) {
    return NULL;
  }

  r_file -> file_line_n = xx_get_no_of_file_line ( r_file -> file_path );
  r_file -> max_string_size_per_line = 256;

  // allocating memory to m_string
  r_file -> m_string = (char*) malloc (sizeof(char) * r_file -> max_string_size_per_line);
  if ( r_file -> m_string == NULL ) {
    printf ("\t Error! Memory Allocation, Failed, xx_file_reader* -> char* m_string\n");
    return NULL;
  }

  // allocating memory to list 
  r_file -> my_list = xx_create_list ( r_file -> file_line_n, r_file -> max_string_size_per_line );
  if ( r_file -> my_list == NULL ) {
    printf ("\t Error! r_file -> xx_list* (up)\n");
    return NULL;
  }

  // opening the file and settting mode as reading and writing 
  r_file -> reading_file = fopen ( r_file -> file_path, "rw" );
  if ( r_file -> reading_file == NULL ) {
    printf ("\t Error! Could not open file for reading \n");
    return NULL;
  }
  
  return r_file;
}

/* 
 * function - long xx_get_file_size ( char* file_path );
 *    -- this function will return the size of the file in bytes I guess
 * */
long xx_get_file_size ( char* file_path ) {
  long r_value = 0; // return value

  FILE* l_file = fopen ( file_path, "r" );
  if ( l_file == NULL ) {
    printf ("\t\t Error! Could not open file with '%s'\n", file_path);
    r_value = -1;
    return r_value;
  }

  // reading the file 
  fseek ( l_file, 0, SEEK_END );

  r_value = ftell ( l_file );

  fclose ( l_file );
  return r_value;
}

/* 
 * function - int xx_get_no_of_file_line ( char* file_path )
 *    -- this function will return the number of lines in the file 
 * */
int xx_get_no_of_file_line ( char* file_path ) {
  int r_value = 0; // return value;

  FILE* reading = fopen ( file_path, "r" );
  char l_line [256];

  while (fgets ( l_line, 256, reading ) != NULL) {
    r_value += 1;
  }

  fclose (reading);

  return r_value;
}


/* 
 * function - void xx_free_file_reader ( xx_file_reader* i_file );
 *    -- this function will free the file reader 
 * */
void xx_free_file_reader ( xx_file_reader* i_file ) {
  if ( i_file != NULL ) {
    fclose ( i_file -> reading_file );
    i_file -> reading_file = NULL;

    free ( i_file -> file_path );
    i_file -> file_path = NULL;

    xx_free_list ( i_file -> my_list );
    i_file -> my_list = NULL;

    free ( i_file -> m_string );
    i_file -> m_string = NULL;

    free ( i_file );
    i_file = NULL;
  }
}

/* 
 * function - int xx_read_from_file_reader ( xx_file_reader* i_file );
 *    -- function to read from file reader
 *    -- meaning it will only fille i_file -> my_list value and nothing more
 *    -- return 1 on success and -1 on failure
 * */
int xx_read_from_file_reader ( xx_file_reader* i_file ) {
  int r_value = 1;

  char t_string [i_file -> max_string_size_per_line];

  while ( fgets ( t_string, i_file -> max_string_size_per_line, i_file -> reading_file ) != NULL ) {
    xx_add_list ( i_file -> my_list, t_string );
  }

  return r_value;
}

/* 
 * function - long xx_get_file_reader_size ( xx_file_reader* i_file )
 *    -- this function will return the size of the file (that is stored in i_file)
 * */
long xx_get_file_reader_size ( xx_file_reader* i_file ) {
  return i_file -> file_size;
}

/* 
 * function - int xx_get_file_reader_line_number ( xx_file_reader* i_file )
 *    -- returns the number of lines of a file that is stored in i_file
 * */
int xx_get_file_reader_line_number ( xx_file_reader* i_file ) {
  return i_file -> file_line_n;
}


/* 
 * function - char* xx_get_file_reader_string_n ( xx_file_reader* i_file, int n );  
 *    -- this function will return the string
 * */
char* xx_get_file_reader_string_n ( xx_file_reader* i_file, int n ) {
  xx_load_list_string_n ( i_file -> my_list, i_file -> m_string, n );
  return i_file -> m_string;
}
