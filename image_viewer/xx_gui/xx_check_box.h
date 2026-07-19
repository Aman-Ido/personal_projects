/* 
 * this is the checkbox for xx_gui
 * */
#define XX_CHECK_BOX 10

#ifndef XX_LIST 
#include "xx_list.h"
#endif

typedef struct {
  xx_list* string_list;
  int max_number; // default - 4 (enter -1 for defaultk)
  int max_string_size; // default - 20  (enter - 1 for default)

  SDL_Rect* list_box; // for the box on each list1
  SDL_Rect* inside_box; // box for inside of the box ( will be a bit smaller )
  
  int n; // index
  int sel_num; // number of elements selected

  int* integer_list; // will store the index of the information 

  int font_size;

  int current_max_string;
  int previous_max_string;
} xx_check_box;

/* 
 * function - declarations
 * */
xx_check_box* xx_create_check_box ( bm_mgr* i_font, int i_max_number, int i_max_string_size );
void xx_check_box_set_default ( bm_mgr* i_font, xx_check_box* i_check_box );
void xx_free_check_box ( xx_check_box* i_check_box );
void xx_render_check_box ( SDL_Renderer* i_renderer, xx_check_box* i_check_box, bm_mgr* i_font, int x, int y ); // for rendering
int xx_listen_check_box ( xx_check_box* i_check_box, SDL_Event e );

// for adding and removing from check_box list
void xx_add_check_box_element ( xx_check_box* i_check_box, char* i_string );
void xx_remove_check_box_element ( xx_check_box* i_check_box );

void xx_print_check_box_info ( xx_check_box* i_check_box );

char** xx_get_string_check_box ( xx_check_box* i_check_box );
void xx_free_string_check_box ( char** i_string, int size );
int* xx_get_integer_check_box ( xx_check_box* i_check_box );
int xx_get_check_box_selected_number ( xx_check_box* i_check_box );

int xx_get_check_box_total_width ( xx_check_box* i_check_box );
int xx_get_check_box_total_height ( xx_check_box* i_check_box );


/* 
 * function - definitions
 * */

/* 
 * function - xx_check_box* xx_create_check_box ( bm_mgr* i_font, int i_max_number, int i_max_string_size );
 *  -- allocates memory and then return the pointer
 * */
xx_check_box* xx_create_check_box ( bm_mgr* i_font, int i_max_number, int i_max_string_size ) {
  // crewating xx_check_box 
  xx_check_box* r_check_box = (xx_check_box*) malloc (sizeof(xx_check_box) * 1);
  if (r_check_box == NULL) {
    printf ("\t Error! Memory allocation, failed, xx_check_box*\n");
    return NULL;
  }

  if (i_max_number < 0) {
    if (i_max_number == -1) {
      r_check_box -> max_number = 4;
    } else {
      printf ("\t Error! Cannot have max_number less than '0'\n");
    }
    
  } else {
    r_check_box -> max_number = i_max_number;
  }

  r_check_box -> font_size = i_font -> z_chang;
  r_check_box -> current_max_string = 0;
  r_check_box -> previous_max_string = 0;

  if (i_max_string_size < 0) {
    if (i_max_string_size == -1) {
      r_check_box -> max_string_size = 20;
    } else {
      printf ("\t Error! Cannot have max_string_sizes less than '0'\n");
    }
    
  } else {
    r_check_box -> max_string_size = i_max_string_size;
  }

  // creawting list 
  r_check_box -> string_list = xx_create_list ( r_check_box -> max_number, r_check_box -> max_string_size );
  if ( r_check_box -> string_list == NULL ) {
    printf ("\t Error! Memory allocation, failed, xx_check_box* -> xx_list*\n");
    return NULL;
  }

  // creawting the integer list for storing the indexes that are stores 
  r_check_box -> integer_list = (int*) malloc (sizeof(int) * r_check_box -> max_number);
  if ( r_check_box -> integer_list == NULL ) {
    printf ("\t Error! Memory allocation, failed, xx_check_box* -> int*\n");
    return NULL;
  }

  // allocating memory to the two rects 
  r_check_box -> list_box = (SDL_Rect* ) malloc (sizeof(SDL_Rect) * r_check_box -> max_number);
  if ( r_check_box -> list_box == NULL ) {
    printf ("\t Error! Memory allocation, failed, xx_check_box* -> SDL_Rect* list_box\n");
    return NULL;
  }

  r_check_box -> inside_box = ( SDL_Rect* ) malloc (sizeof(SDL_Rect) * r_check_box -> max_number);
  if ( r_check_box -> inside_box == NULL ) {
    printf ("\t Error! Memory allocation, failed, xx_check_box* -> SDL_Rect* inside_box\n");
    return NULL;
  }

  // set up check_box default
  xx_check_box_set_default ( i_font, r_check_box );

  return r_check_box;
}


/* 
 * function - void xx_check_box_default ( bm_mgr* i_font, xx_check_box* i_check_box );
 *  -- will fill default values to each of the boxes
 * */
void xx_check_box_set_default ( bm_mgr* i_font, xx_check_box* i_check_box ) {
  for (int i = 0; i < i_check_box -> max_number; i++) {
    i_check_box -> list_box [i].w = i_font -> z_chang;
    i_check_box -> list_box [i].h = i_font -> z_chang;

    i_check_box -> inside_box [i].w = i_check_box -> list_box [i].w - 4;
    i_check_box -> inside_box [i].h = i_check_box -> list_box [i].h - 4;

    i_check_box -> integer_list [i] = -1; // meaning non exists
  }

  i_check_box -> n = 0;
  i_check_box -> sel_num = 0;
}

/* 
 * function - void xx_free_check_box ( xx_check_box* i_check_box )
 *  -- freeing everything 
 * */
void xx_free_check_box ( xx_check_box* i_check_box ) {
  if ( i_check_box != NULL ) {
    free ( i_check_box -> list_box );
    i_check_box -> list_box = NULL;

    free ( i_check_box -> inside_box );
    i_check_box -> inside_box = NULL;

    free ( i_check_box -> integer_list );
    i_check_box -> integer_list = NULL;

    xx_free_list ( i_check_box -> string_list );
    i_check_box -> string_list = NULL;
  }
}

/* 
 * function - void xx_render_check_box ( SDL_Renderer* i_renderer, xx_check_box* i_check_box, bm_mgr* i_font, int x, int y );
 *    -- function to render checkbox 
 * */
void xx_render_check_box ( SDL_Renderer* i_renderer, xx_check_box* i_check_box, bm_mgr* i_font, int x, int y ) {
  bm_set_color ( i_font, 0, 0, 0 );

  // set border color here
  for (int i = 0; i < i_check_box -> n; i++) {
    i_check_box -> list_box [i].x = x;
    i_check_box -> list_box [i].y = y + (i * (i_font -> z_chang + 2)); // that '2' is padding

    i_check_box -> inside_box [i].x = i_check_box -> list_box[i].x + 2;
    i_check_box -> inside_box [i].y = i_check_box -> list_box [i].y + 2;

    SDL_SetRenderDrawColor ( i_renderer, 100, 100, 100, 100 );
    SDL_RenderDrawRect ( i_renderer, &i_check_box -> list_box[i] );

    if ( i_check_box -> integer_list [i] == 1 ) {
      SDL_SetRenderDrawColor ( i_renderer, 200, 200, 200, 255 );
      SDL_RenderFillRect ( i_renderer, &i_check_box -> inside_box[i] );
    }

    // rendering the text not 
    bm_render ( i_renderer, i_font, i_check_box -> string_list -> string_array [i], 
        (i_check_box -> list_box [i].x + i_check_box -> list_box [i].w) + 2, i_check_box -> list_box [i].y );
  }
}


/* 
 * function - void xx_add_check_box_element ( xx_check_box* i_check_box, (char*) i_string )
 *    -- function to add thing to the end of the list 
 * */
void xx_add_check_box_element ( xx_check_box* i_check_box, char* i_string ) {
  xx_add_list ( i_check_box -> string_list, i_string );
  // i_check_box -> list_box [ i_check_box -> n ] = 
  i_check_box -> integer_list [ i_check_box -> n ] = 0; // meaning false - not chosen
  
  int l_string_size = xx_strlen ( i_string );
  if ( l_string_size > i_check_box -> current_max_string ) {
    i_check_box -> previous_max_string = i_check_box -> current_max_string;
    i_check_box -> current_max_string = l_string_size;
  }
  i_check_box -> n += 1;
}

/* 
 * function - void xx_remove_check_box_element ( xx_check_box* i_check_box )
 *    -- removes the last elements from the check box 
 * */
void xx_remove_check_box_element ( xx_check_box* i_check_box ) {
  xx_remove_list ( i_check_box -> string_list );

  i_check_box -> current_max_string = i_check_box -> previous_max_string;

  i_check_box -> n -= 1;
  i_check_box -> integer_list [ i_check_box -> n] = -1; // meaning it is not initialized
}

/*
 * function - void xx_print_check_box_info ( xx_check_box* i_check_box ) {
 *  -- prints out all the information about the i_check_box
 * }
 * */
void xx_print_check_box_info ( xx_check_box* i_check_box ) {
  printf ("*** xx_check_box* ***\n");
  printf ("\txx_check_box -> xx_list* :  %p\n", i_check_box -> string_list);
  printf ("\txx_check_box -> max_number : %d\n", i_check_box -> max_number);
  printf ("\txx_check_box -> max_string_size : %d\n", i_check_box -> max_string_size);
  printf ("\txx_check_box -> list_box : %p\n", i_check_box -> list_box);
  printf ("\txx_check_box -> inside_box : %p\n", i_check_box -> inside_box);
  printf ("\txx_check_box -> n (curr position) : %d\n", i_check_box -> n);
}

/* 
 * function - int xx_listen_check_box ( xx_check_box* i_check_box, SDL_Event* e );
 *    -- funcction to listen for inputs and then control the data value 
 * */
int xx_listen_check_box ( xx_check_box* i_check_box, SDL_Event e ) {
  SDL_Point m_mouse = {0, 0};

  if (e.type == SDL_MOUSEBUTTONDOWN ){
    m_mouse.x = e.button.x;
    m_mouse.y = e.button.y;
    for (int i = 0; i < i_check_box -> n; i++) {
      if ( SDL_PointInRect (&m_mouse, &i_check_box -> list_box[i]) ) {
        if ( i_check_box -> integer_list [i] == 0 ) {
          i_check_box -> integer_list[i] = 1;
          i_check_box -> sel_num += 1;
        } else {
          i_check_box -> integer_list[i] = 0;
          i_check_box -> sel_num -= 1;
        }
      }
    }
  }

  return 1;
}

/* 
 * function - char** xx_get_string_check_box ( xx_check_box* i_check_box )
 *    -- function to return the main string array 
 * */
char** xx_get_string_check_box ( xx_check_box* i_check_box ) {
  /* xx_list* i_list = xx_create_list ( i_check_box -> max_number, i_check_box -> max_string_size );
  for (int i = 0; i < i_check_box -> n; i++) {
    if (i_check_box -> integer_list [i] == 1) {
      xx_add_list ( i_list, i_check_box -> string_list [i] );
    }

  } */
  printf ("The problem is here \n");
  char** r_str = (char**) malloc (sizeof (char*) * i_check_box -> sel_num);

  for (int i = 0; i < i_check_box -> sel_num; i++) {
    r_str [i] = (char*) calloc (i_check_box -> max_string_size, sizeof(char));
  }
  // printf ("%s\n", i_check_box -> string_list -> string_array[0]);
  int n = 0;

  for (int i = 0; i < i_check_box -> max_number; i++) {
    if ( i_check_box -> integer_list[i] == 1 ) {
      xx_strcpy ( r_str [n], i_check_box -> string_list -> string_array [i] );
      n++;
    }
  }
  // xx_strcpy ( r_str )
  
  return r_str;
}

/* just frees the memory allocated by /\\ function, here (int size) just but check1 -> sel_num */
void xx_free_string_check_box ( char** i_string, int size ) {
  if ( i_string != NULL ) {
    for (int i = 0; i < size; i++) {
      free ( i_string [i] );
      i_string [i] = NULL;
    }
    free (i_string);
    i_string = NULL;
  }
}


/* 
 * function - int xx_get_integer_check_box ( xx_check_box* i_check_box )
 *  -- returns the integer array of (everything like ) from {1, 0, 1, 0}
 * */
int* xx_get_integer_check_box ( xx_check_box* i_check_box  ) {
  return i_check_box -> integer_list;
}

/* 
 * function - int xx_get_check_box_selected_number ( xx_check_box* i_check_box )
 *  -- returns the number of selected items
 * */
int xx_get_check_box_selected_number ( xx_check_box* i_check_box ) {
  return i_check_box -> sel_num;
}


int xx_get_check_box_total_width ( xx_check_box* i_check_box ) {
  return i_check_box -> list_box[0].w + (i_check_box -> current_max_string * i_check_box -> font_size) + 4; // (2 + 2) // padding
}

int xx_get_check_box_total_height ( xx_check_box* i_check_box ) {
  return (i_check_box -> list_box[0].h * i_check_box -> n) + 4; // (2 + 2) - padding
}
