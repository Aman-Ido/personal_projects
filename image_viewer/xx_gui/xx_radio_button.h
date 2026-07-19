/* 
 * it may be a radio buttion but I'm not going to make a circle (it will be a box)
 * */
#define XX_RADIO_BUTTON 0x55

#ifndef XX_LIST 
#include "xx_list.h"
#endif

#ifndef XX_STRING
#include "xx_string.h"
#endif

typedef struct {
  xx_list* string_list; // for the list // default - NULL

  int max_number; // default - 4
  int max_string_size; // default - 20

  SDL_Rect* outer_box; // default - NULL (width will be the size of the font )
  SDL_Rect* inner_box; // default - NULL (width will be the size of the font );

  int selected_value; // I don't really need this. (default - (-1)); // meaning nothing has been selected 
  int n; // cursor (default - 0;)

  int font_size;

  int current_max_string; // to store the current longest string size
  int previous_max_string; // to store the previous longest string size
} xx_radio_button;

/* 
 * function - declarations
 * */
xx_radio_button* xx_create_radio_button ( bm_mgr* i_font, int i_max_number, int i_max_string_size ); // allocates memory and return the pointer
void xx_set_default_radio_button ( bm_mgr* i_font, xx_radio_button* i_radio  ); // no need to call, just a default setter, I doon't why even call it

void xx_render_radio_button ( SDL_Renderer* i_renderer, xx_radio_button* i_radio, bm_mgr* i_font, int x, int y ); // for rendering
int xx_listen_radio_button ( xx_radio_button* i_radio, SDL_Event e ); // for listening ot the input

void xx_add_radio_button ( xx_radio_button* i_radio, char* i_string ); // this will add to the end of the string array
void xx_remove_radio_button ( xx_radio_button* i_radio ); // this will remove from the last of the radio_button (string array);

void xx_free_radio_button ( xx_radio_button* i_radio ); // will free everything (don't mistake it for xx_free_radio_button_string);


// getters
int xx_get_radio_button_int ( xx_radio_button* i_radio ); // will only return the index which is selected
char* xx_get_radio_button_string ( xx_radio_button* i_radio ); // will return a dynamically allocated char* memory
void xx_free_radio_button_string ( char* i_string ); // this must be called if /\\ is called
int xx_get_radio_button_total_width ( xx_radio_button* i_radio );
int xx_get_radio_button_total_height ( xx_radio_button* i_radio );

/* 
 * function - definitions
 * */

/* 
 * function - xx_radio_button* xx_create_radio_button ( bm_mgr* i_font, int i_max_number, int i_max_string_size )
 *  -- allocates necessary memory and then calls for default function ( to fill in the default values )
 * */
xx_radio_button* xx_create_radio_button ( bm_mgr* i_font, int i_max_number, int i_max_string_size) {
  // allocating memory to the radio button
  xx_radio_button* r_radio_button = (xx_radio_button*) malloc (sizeof(xx_radio_button) * 1);
  if ( r_radio_button == NULL ) {
    printf ("\t Error! Memory allocation, failed, xx_radio_button*\n");
    return NULL;
  }

  // setting up the default values for xx_radio_button* -> max_number and xx_radio_button* -> max_string_size
  if ( i_max_number < 0 ) {
    if ( i_max_number == -1 ) {
      r_radio_button -> max_number = 4; // default value
    } else {
      printf ("\t Error! Cannot Have max_number to be less than 0 (for default input -1)\n");
      return NULL;
    }
  } else {
    r_radio_button -> max_number = i_max_number;
  }

  r_radio_button -> font_size = i_font -> z_chang;
  r_radio_button -> current_max_string = 0;
  r_radio_button -> previous_max_string = 0;

  // setting up the default value to the string_size
  if ( i_max_string_size < 0 ) {
    if ( i_max_string_size == -1 ) {
      r_radio_button -> max_string_size = 20; // default value
    } else {
      printf ("\t Error! Cannot Have max_string_size to be less than 0 (for default input -1)\n");
      return NULL;
    }
  } else {
    r_radio_button -> max_string_size = i_max_string_size;
  }

  

  r_radio_button -> string_list = xx_create_list ( r_radio_button -> max_number, r_radio_button -> max_string_size );
  if ( r_radio_button == NULL ) {
    printf ("\t Error! Could not allocate Memory to xx_radio_button* -> xx_list*\n");
    return NULL;
  }

  r_radio_button -> outer_box = (SDL_Rect*) malloc (sizeof (SDL_Rect) * r_radio_button -> max_number);
  if (r_radio_button -> outer_box == NULL) {
    printf ("\t Error! Memory allocation, failed, xx_radio_button* -> SDL_Rect*\n");
    return NULL;
  }

  r_radio_button -> inner_box = (SDL_Rect*) malloc (sizeof (SDL_Rect) * r_radio_button -> max_number);
  if (r_radio_button -> inner_box == NULL) {
    printf ("\t Error! Memory allocation, failed, xx_radio_button* -> SDL_Rect*\n");
    return NULL;
  }

  xx_set_default_radio_button ( i_font, r_radio_button );

  return r_radio_button;
}

/* 
 * function - void xx_set_default_radio_button ( bm_mgr* i_font, int i_max_number, int i_max_string_size );
 *    -- function to set default values
 * */
void xx_set_default_radio_button ( bm_mgr* i_font, xx_radio_button* i_radio ) {

  for (int i = 0; i < i_radio -> max_number; i++) {
    i_radio -> outer_box[i].w = i_font -> z_chang;
    i_radio -> outer_box[i].h = i_font -> z_chang;

    i_radio -> inner_box[i].w = i_radio -> outer_box [i].w - 4;
    i_radio -> inner_box[i].h = i_radio -> outer_box [i].h - 4;
  }

  i_radio -> selected_value = -1;
  i_radio -> n = 0;

}

/* 
 * function - void xx_render_radio_button ( SDL_Renderer* i_renderer, xx_radio_button* i_radio, bm_mgr* i_font, int x, int y );
 *    -- function to render the radio button 
 * */
void xx_render_radio_button ( SDL_Renderer* i_renderer, xx_radio_button* i_radio, bm_mgr* i_font, int x, int y ) {

    bm_set_color ( i_font, 0, 0, 0 );
  for ( int i = 0; i < i_radio -> n; i++ ) {
    // i_radio -> outer_box [i].x = x + ( i * (i_font -> z_chang + 3 ));
    i_radio -> outer_box [i].x = x;
    i_radio -> outer_box [i].y = y + ( i * (i_font -> z_chang + 3) );

    i_radio -> inner_box [i].x = i_radio -> outer_box [i].x + 2;
    i_radio -> inner_box [i].y = i_radio -> outer_box [i].y + 2;

    SDL_SetRenderDrawColor ( i_renderer, 100, 100, 100, 100 );
    SDL_RenderDrawRect ( i_renderer, &i_radio -> outer_box[i] );

    if ( i_radio -> selected_value == i ) {
      SDL_SetRenderDrawColor ( i_renderer, 200, 100, 50, 234 );
      SDL_RenderFillRect ( i_renderer, &i_radio -> inner_box [i] );
    }

    bm_render ( i_renderer, i_font, i_radio -> string_list -> string_array [i], (i_radio -> outer_box[i].x + i_radio -> outer_box [i].w) + 3, i_radio -> outer_box [i].y );
  }

}

/* 
 * function - void xx_add_radio_button ( xx_radio_button* i_radio, char* i_string );
 *    -- this function will add a string to the radio button
 * */
void xx_add_radio_button ( xx_radio_button* i_radio, char* i_string ) {
  if ( i_radio -> n >= i_radio -> max_number ) {
    printf ("\t Error! xx_add_radio_button-> limit has been reached\n");
    return;
  }

  xx_add_list ( i_radio -> string_list, i_string );

  if ( xx_strlen (i_string) > i_radio -> current_max_string) {
    i_radio -> previous_max_string = i_radio -> current_max_string;
    i_radio -> current_max_string = xx_strlen (i_string);
  }

  i_radio -> n += 1;

}

/* 
 * function - void xx_remove_radio_button ( xx_radio_button* i_radio );
 *    -- this function will remove the last string from the radio button
 * */
void xx_remove_radio_button ( xx_radio_button* i_radio ) {

  xx_remove_list ( i_radio -> string_list );

  if ( i_radio -> n == i_radio -> selected_value ) {
    i_radio -> selected_value = -1;
  }

  i_radio -> current_max_string = i_radio -> previous_max_string;

  i_radio -> n -= 1;
}

/* 
 * function - void xx_free_radio_button ( xx_radio_button* i_radio )
 *    -- this function will clear the radio button
 * */
void xx_free_radio_button ( xx_radio_button* i_radio ) {
  if ( i_radio != NULL ) {
    xx_free_list ( i_radio -> string_list );
    i_radio -> string_list = NULL;

    free ( i_radio -> outer_box );
    i_radio -> outer_box = NULL;

    free ( i_radio -> inner_box );
    i_radio -> inner_box = NULL;

    free ( i_radio );
    i_radio = NULL;
  }
}

/* 
 * function - int xx_listen_radio_button ( xx_radio_button* i_radio, SDL_Event e )
 *    -- this function will listen to the events and update the things
 * */
int xx_listen_radio_button ( xx_radio_button* i_radio, SDL_Event e ) {
  SDL_Point m_mouse_pos = {0};
  if (e.type == SDL_MOUSEBUTTONDOWN) {
    m_mouse_pos.x = e.button.x;
    m_mouse_pos.y = e.button.y;

    for (int i = 0; i < i_radio -> n; i++) {
      if ( SDL_PointInRect ( &m_mouse_pos, &i_radio -> outer_box [i] ) ) {
        if ( i_radio -> selected_value != i ) {
          i_radio -> selected_value = i;
        }
      }


    }
  }

  return 1;
}

/* 
 * function - int xx_get_radio_button_int ( xx_radio_button* i_radio );
 *    -- will return the integer value of the selected item
 * */
int xx_get_radio_button_int ( xx_radio_button* i_radio ) {
  return i_radio -> selected_value;
}

/* 
 * function - char* xx_get_radio_button_string ( xx_radio_button* i_radio )
 *    -- will return the string value of the selected item ( or will return selected item )
 * */
char* xx_get_radio_button_string ( xx_radio_button* i_radio ) {
  char* r_string = (char*) malloc (sizeof (char) * i_radio -> max_string_size);

  xx_strcpy ( r_string, i_radio -> string_list -> string_array [i_radio -> selected_value] );

  return r_string;
}

/* 
 * function - void xx_free_radio_button_string  ( char* );
 *  -- will free the dynamically allocated memory
 * */
void xx_free_radio_button_string ( char* i_string ) {
  if (i_string != NULL) {
    free ( i_string );
    i_string = NULL;
  }
}

/* 
 * function - getters
 * */
int xx_get_radio_button_total_width ( xx_radio_button* i_radio ) {
  return i_radio -> outer_box -> w + ( i_radio -> font_size * i_radio -> current_max_string );
}

int xx_get_radio_button_total_height ( xx_radio_button* i_radio ) {
  return i_radio -> outer_box -> h + ( i_radio -> font_size * i_radio -> n );
}
