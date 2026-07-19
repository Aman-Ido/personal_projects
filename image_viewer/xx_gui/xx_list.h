/* 
 * just a list nothing more than that
 * */
#define XX_LIST 0x01

#ifndef XX_STRING 
#include "xx_string.h"
#endif

typedef struct {
  int max_items;
  int max_string_size;
  int n; // for indexing - current number of items

  char** string_array;
} xx_list;

/* 
 * function - declarations
 * */
xx_list* xx_create_list ( int max_items, int max_string_size ); // creates the list
void xx_free_list ( xx_list* i_list ); // frees everything of the list

// functions to add or remove the string from the string array in the list (remember to add xx_string.h)
void xx_add_list ( xx_list* i_list, char* i_string ); // adds to the end 
void xx_remove_list ( xx_list* i_list ); // removes from the last 
void xx_add_list_n ( xx_list* i_list, char* i_string, int n ); // add on that particular list from that 
void xx_remove_list_n ( xx_list* i_list, int n ); // remove that indexed string 


// getters 
void xx_print_list_info ( xx_list* i_list );
int xx_get_list_max_items ( xx_list* i_list );
int xx_get_list_max_string_size ( xx_list* i_list );
int xx_get_list_cursor ( xx_list* i_list );
char* xx_get_list_n ( xx_list* i_list, int n );
void xx_print_list ( xx_list* i_list);

void xx_load_list_string_n ( xx_list* i_list, char* i_string, int n ); // loads the string to i_string1

/* 
 * funciton - xx_list* xx_create_list ( int max_items, int max_string_size );
 *  --- function to allocate all the necessary elements and set default values
 * */
xx_list* xx_create_list ( int max_items, int max_string_size ) {
  if (max_items < 0 || max_string_size < 0) {
    printf ("\t Error! Could not have either max_items | max_string_size -> '0'\n");
    return NULL;
  } 

  // allocating memory 
  xx_list* r_list = (xx_list*) malloc (sizeof(xx_list) * 1);
  if ( r_list == NULL ) {
    printf ("\t Error! memory allocation, failed, xx_list*\n");
    return NULL;
  } else {
    r_list -> max_string_size = max_string_size;
    r_list -> n = 0;
    r_list -> max_items = max_items;
  }

  r_list -> string_array = (char**) malloc (sizeof(char*) * r_list -> max_items);
  if ( r_list -> string_array == NULL ) {
    printf ("\t Error! Memory allocation, failed, xx_list* -> char**\n");
    return NULL;
  }

  for (int i = 0; i < r_list -> max_items; i++) {
    r_list -> string_array[i] = (char*) calloc (r_list -> max_string_size, sizeof(char));
    if (r_list -> string_array[i] == NULL) {
      printf ("\t Error! Memory allocation, failed, xx_list* -> char** -> char* [%d]\n", i);
    }
  }


  return r_list;
}

/* 
 * function - void xx_free_list ( xx_list* i_list ) {}
 *  -- function to free everything allocated
 * */
void xx_free_list ( xx_list* i_list ) {
  if ( i_list != NULL ) {
    // freeing the individual strings
    for (int i = 0; i < i_list -> max_items; i++) {
      free ( i_list -> string_array[i] );
      i_list -> string_array[i] = NULL;
    }

    free ( i_list -> string_array );
    i_list -> string_array = NULL;

    free ( i_list );
    i_list = NULL;
  }
}

/* 
 * function - void xx_print_list_info ( xx_list* i_list )
 *  -- this function will print everything of thel ist
 * */
void xx_print_list_info ( xx_list* i_list ) {
  printf ("xx_list* -> max_items :\t %d\n", i_list -> max_items);
  printf ("xx_list* -> max_string_size :\t %d\n", i_list -> max_string_size);
  printf ("xx_list* -> n (current pos) :\t %d\n", i_list -> n);
  printf ("xx_list* -> string_array (mem) :\t %p\n", i_list -> string_array);

  printf ("Size of xx_list -> %ld\n", sizeof(xx_list));
}

/* 
 * function - int xx_get_list_max_items ( xx_list* i_list );
 *    -- this function will return the max number of list items 
 * */
int xx_get_list_max_items ( xx_list* i_list ) {
  return i_list -> max_items;
}

/* 
 * function - int xx_get_list_max_string_size ( xx_list* i_list )
 *    -- function will return the max string size of this list 
 * */
int xx_get_list_max_string_size ( xx_list* i_list ) {
  return i_list -> max_string_size;
}

/* 
 * function - int xx_get_list_cursor ( xx_list* i_list ) 
 *    -- function to get the current list cursor (n) or n number of elements
 * */
int xx_get_list_cursor ( xx_list* i_list ) {
  return i_list -> n;
}

/* 
 * function - void xx_add_list ( xx_list* i_list, char* i_string );
 *    - function to add a string to the end of the string 
 * */
void xx_add_list ( xx_list* i_list, char* i_string ) {
  xx_push_back_string ( i_list -> string_array, i_string, i_list -> max_items );
  i_list -> n += 1;
}

/* 
 * function - void xx_remove_list ( xx_list* i_list );
 *    - function to remove the last string from the list 
 * */
void xx_remove_list ( xx_list* i_list ) {
  xx_pop_back_string ( i_list -> string_array, i_list -> max_items );
  i_list -> n -= 1;
}

/* 
 * function - void xx_print_list ( xx_list* i_list );
 *    - function to print everything (string) of the list 
 * */
void xx_print_list ( xx_list* i_list) {
  for (int i = 0; i < i_list -> n; i++) {
    printf ("%d -> %s\n", i, i_list -> string_array[i]);
  }
}

/* 
 * function - void xx_add_list_n ( xx_list* i_list, char* i_string, int n )
 *  -- adds to that particular index 
 * */
void xx_add_list_n ( xx_list* i_list, char* i_string, int n ) {
  if (i_list -> n + 1 > i_list -> max_items ) {
    printf ("Error! Not Enough Space\n");
    return;
  }

  int index = xx_array_length_string ( i_list -> string_array, i_list -> max_items );

  int i = n;
  for (i = index; i >= n; i-- ) {
    xx_strcpy ( i_list -> string_array [i + 1], i_list -> string_array[i] );
  }

  xx_strcpy ( i_list -> string_array [n], i_string );

  i_list -> n += 1;
}

/* 
 * function - void xx_remove_list_n ( xx_list* i_list, int n );
 *  -- removes to that particular index 
 * */
void xx_remove_list_n ( xx_list* i_list, int n ) {
  int index = xx_array_length_string ( i_list -> string_array, i_list -> max_items );

  int i;
  for (i = n; i < index - 1; i++ ) {
    xx_strcpy ( i_list -> string_array [i], i_list -> string_array [i + 1] );
  }

  i_list -> string_array [index - 1][0] = '\0';
  i_list -> n -= 1;
}

/* 
 * function to get string for a specific n number 
 * */
char* xx_get_list_n ( xx_list* i_list, int n ) {
  if (n < 0) {
    return NULL;
  }
  return i_list -> string_array[n];
}

/* 
 * function - void xx_load_list_string_n ( xx_list* i_list, char* i_string, int n ); // loads the string to i_string1
 *    -- this function will load the 'n' index string to 2nd argument
 * */
void xx_load_list_string_n ( xx_list* i_list, char* i_string, int n ) {
  xx_strcpy ( i_string, i_list -> string_array[n] );
}
