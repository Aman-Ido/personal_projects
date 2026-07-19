/* 
 * xx_label.h
 *  -- this file is just like input however, it does not interact with the user it's just for showing data
 *  -- make sure to include bm_mgr, xx_window_manager before including this file
 * */

#ifndef XX_STRING
#include "./xx_string.h"
#endif

typedef struct {
  SDL_Rect rect;

  size_t string_size;
  char* message;

  SDL_Color* text_color;
  SDL_Color* border_color;
} xx_label;

xx_label* xx_create_label ( bm_mgr* font, int width, int str_size, char* message ); // allocates the memory and returns the xx_label* pointer
void xx_free_label ( xx_label* lbl ); // frees and sets the variables to NULL
void xx_render_label ( SDL_Renderer* i_renderer, xx_label* lbl, bm_mgr* font, int x, int y ); // renders the label box and also the the messages that is set
void xx_set_label_message ( xx_label* lbl, int message_size, char* message ); // sets the message
int xx_allocate_label_message ( xx_label* lbl, int size); // allocates memory for storing the messages (used for reallocation)
void xx_set_label_width ( xx_label* lbl, bm_mgr* font, int width  ); // used to set the total width (enter negative value for automatic fill)

/* 
 * getters
 * */
int xx_get_label_total_width ( xx_label* lbl ); // returns total width
int xx_get_label_total_height ( xx_label* lbl ); // returns total height

xx_label* xx_create_label ( bm_mgr* font, int width, int str_size, char* message ) {
  xx_label* lbl = (xx_label*) calloc (sizeof(xx_label), 1 );
  if ( lbl == NULL ) {
    fprintf (stderr, "Could not allocate memory - xx_label*\n");
    return NULL;
  }

  lbl -> rect.x = 0;
  lbl -> rect.y = 0;
  if ( width > 0 ) {
    lbl -> rect.w = width;
  } else if ( width < 0 ) {
    lbl -> rect.w = 200; // this is the default value
  }
  lbl -> rect.h = font -> font_size + 4; // 4 is the padding

  // allocating memory to the colors
  lbl -> text_color = (SDL_Color*) calloc (sizeof(SDL_Color), 1);
  lbl -> border_color = (SDL_Color*) calloc (sizeof(SDL_Color), 1);


  lbl -> text_color -> r = 0;
  lbl -> text_color -> g = 0;
  lbl -> text_color -> b = 0;
  lbl -> text_color -> a = 255;

  lbl -> border_color -> r = 0;
  lbl -> border_color -> g = 0;
  lbl -> border_color -> b = 0;
  lbl -> border_color -> a = 255;

  // setting in the string size
  if ( str_size > 0 ) {
    lbl -> string_size = str_size;
  } else if ( str_size < 0 ) {
    lbl -> string_size = 30; // default value
  }

  if ( xx_strlen (message) > lbl -> string_size ) {
    fprintf (stdout, "Warning! Inputted string_size is less than that of the string_length of message\nIf you have inputted negative size then the default value is 30\n ");
    return NULL;
  }

  // allocating memory the messagej
  fprintf (stdout, "String size -> %d\n", lbl -> string_size);
  lbl -> message = (char*) calloc (sizeof(char), lbl -> string_size);
  if ( lbl -> message == NULL ) {
    fprintf (stderr, "Error! Could not allocate memory - xx_label* lbl -> char* message\n");
    return NULL;
  }
  snprintf ( lbl -> message, lbl -> string_size, "%s", message );
  //lbl -> rect.w = font -> font_size * xx_strlen (lbl -> message);

  return lbl;
}

void xx_free_label ( xx_label* lbl ) {
  if ( lbl -> message != NULL ) {
    free ( lbl -> message );
    lbl -> message = NULL;
  }

  if ( lbl -> text_color != NULL ) {
    free ( lbl -> text_color );
    lbl -> text_color = NULL;
  }

  if ( lbl -> border_color != NULL ) {
    free ( lbl -> border_color );
    lbl -> border_color = NULL;
  }

  if ( lbl != NULL ) {
    free ( lbl );
    lbl = NULL;
  }
}

void xx_render_label ( SDL_Renderer* i_renderer, xx_label* lbl, bm_mgr* font, int x, int y ) {
  lbl -> rect.x = x;
  lbl -> rect.y = y;

  // changing colors
  SDL_SetRenderDrawColor ( i_renderer, lbl -> border_color -> r, lbl -> border_color -> g, lbl -> border_color -> b, lbl -> border_color -> a );
  // rendering a rect
  SDL_RenderDrawRect ( i_renderer, &lbl -> rect );

  // changing font color
  bm_set_color ( font, lbl -> text_color -> r, lbl -> text_color -> g, lbl -> text_color -> b );
  // rendering the font 
  bm_render ( i_renderer, font, lbl -> message ,lbl -> rect.x + 2, lbl -> rect.y + 2 ); // these '2' are the padding
}

/* 
 * int xx_allocate_label_message ( xx_label* lbl, int size )
 *    -- returns 1 on succ and 0 on fail
 *    -- allocate this function if you have a string which is greater in size than previously allocated string size
 *    -- if the string size is less than the previously allocate size, then no need to call this fuunction
 *    -- call this function if you want to change the message / the string data of the label
 * e.g. ----> current size (5), 
 *          # you want to display (shit), no need to use this function 
 *          # you want to display (it's raining), you should call this function first
 * */
int xx_allocate_label_message ( xx_label* lbl, int size ) {
  // freeing the memory
  if ( lbl -> message != NULL ) {
    free ( lbl -> message );
    lbl -> message = NULL;
  }

  lbl -> message = (char*) calloc (sizeof(char), size);
  if ( lbl -> message == NULL) {
    fprintf (stderr, "Error! Could not re-allocate xx_allocate_label_message memory \n");
    return 0;
  }
  return 1;
}

/* 
 * void xx_set_label_message ( xx_label*, int, char* )
 *    -- all this function does it change the data inside the label's message variable
 *    -- use this function to change the string on the label
 * */
void xx_set_label_message ( xx_label* lbl, int message_size, char* message ) {
  if ( message_size > lbl -> string_size ) {
    fprintf (stderr, "Error! Message Size is greater than allocate size, please call xx_allocate_label_message first (xx_label*, size_t)\n");
    return;
  }
  snprintf ( lbl -> message, lbl -> string_size, "%s", message );
}

/* 
 * void xx_set_label_width ( xx_label*, bm_mgr* font, int width );
 *  -- sets the width for the label, enter negative value for automatic
 * */
void xx_set_label_width ( xx_label* lbl, bm_mgr* font, int width  ) {
  if ( width < 0 ) {
    lbl -> rect.w = font -> font_size * xx_strlen( lbl -> message);
  } else {
    lbl -> rect.w = width;
  }
}

/* 
 * int xx_get_label_total_width ( xx_label* )
 *  -- returns the total width of the label
 * */
int xx_get_label_total_width ( xx_label* lbl ) {
  return lbl -> rect.w;
}

/* 
 * int xx_get_label_total_height ( xx_label* )
 *  -- returns the total height of the label
 * */
int xx_get_label_total_height ( xx_label* lbl ) {
  return lbl -> rect.h;
}
