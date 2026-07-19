/* 
 * This program is where we are going to make a xx_button type header file but for inputs
 * Also we are going to use custom made bm_mgr for inputs. the version with SDL_ttf will be provided in later dates
 * */
#ifndef XX_INPUT 
#define XX_INPUT 1010
#endif

// including bm_mgr
#ifndef BM_MGR
#include "./bm_mgr.h"
#endif

#ifndef XX_COLOR
#include "./xx_color.h"
#endif

#ifndef XX_CURSOR
#include "./xx_cursor.h"
#endif

// #ifndef XX_LINE
// #include "./xx_line.h"
// #endif

#define XX_USER_INPUT_SIZE 72 // dynamic array will be 72 array for now

/* enum for border types */
typedef enum {
  BORDER, // just for border
  LINE, // just for line
} Input_Box_Type;

typedef enum {
  HOVER,
  ACTIVE,
  NONE, 
} Input_Box_Action;

typedef enum {
  TEXT,
  PASSWORD,
} Input_Type; // don't mistake this one for that 'border' input

/* 
 * struct for input - xx_input
 * */
typedef struct {
  SDL_Rect* border; // this is for the box
  int box_type; // this will be set to 'border' by default
  int input_type; // this will be the input type 'text' by default
  
  /* for cursor */
  xx_cursor* t_cursor; // text cursor

  // string to store the input from the user
  char* user_input;
  int user_input_size;
  
  char* pass_input;
  int pass_input_size;
  
  SDL_Point* mouse_position; // for mouse position
  
  /* 
    these three variables are for calculations
    so that I don't have to calculate again and again in render function
  */
  char* f_input; // for function input, default "";
  int f_input_size; // this is the size, default 'box width / font_size'
  int f_input_from; // this is from, default '0'

  SDL_Point* padding; // x is for horizontal padding and y is for vertical padding
  
  int is_active; // default '0' checks if the input is active or not
  int action_type;
  
  SDL_Color* border_color; // for border color
  SDL_Color* hover_color; // for hover color
  SDL_Color* click_color; // for click color (when active)
                       
  SDL_Color* text_color; // for text color
  
  
} xx_input;


/* 
 * function - declarations
 * */
void xx_render_input (SDL_Renderer* i_renderer, xx_input* i_input, bm_mgr* i_font, int x, int y); // to render the input
int xx_set_input_default (xx_input* i_input, bm_mgr* i_font);
int xx_listen_input (SDL_Renderer* i_renderer, xx_input* i_input, bm_mgr* i_font, SDL_Event e);
xx_input* xx_create_input (int i_size, bm_mgr* i_font);
int xx_allocate_memory_input (xx_input* i_input, bm_mgr* i_font); // allocates memory to all the necessary variables inside of the function

void xx_set_input_cursor_size (xx_input* i_input, int i_width, int i_height);
void xx_set_input_cursor_color (xx_input* i_input, int r, int g, int b, int a);

// unrelated function
/* 
  setters
*/
void xx_set_input_type (xx_input* i_input, Input_Type i_value);
void xx_set_input_width (xx_input* i_input, bm_mgr* i_value,  size_t i_width, int i_height);
void xx_set_input_padding (xx_input* i_input, size_t h_pad, size_t v_pad, bm_mgr* i_font);
void xx_set_input_border_type (xx_input* i_input, Input_Box_Type i_value);

int xx_get_input_total_width ( xx_input* i_input );
int xx_get_input_total_height ( xx_input* i_input );


// clearing
void xx_free_input (xx_input* i_input);


/*
 * function - definitions
 * */
 
 /* 
  function - int xx_set_input_default (xx_input* i_input, bm_mgr* i_font);
        - just sets the input to default
 */
int xx_set_input_default (xx_input* i_input, bm_mgr* i_font) {
  i_input -> text_color -> r = 0;
  i_input -> text_color -> g = 0;
  i_input -> text_color -> b = 0;
  i_input -> text_color -> a = 0;


  i_input -> border_color -> r = XX_NONE_R;
  i_input -> border_color -> g = XX_NONE_G;
  i_input -> border_color -> b = XX_NONE_B;
  i_input -> border_color -> a = XX_NONE_A;


  i_input -> hover_color -> r = XX_HOVER_R;
  i_input -> hover_color -> g = XX_HOVER_G;
  i_input -> hover_color -> b = XX_HOVER_B;
  i_input -> hover_color -> a = XX_HOVER_A;


  i_input -> click_color -> r = XX_CLICK_R;
  i_input -> click_color -> g = XX_CLICK_G;
  i_input -> click_color -> b = XX_CLICK_B;
  i_input -> click_color -> a = XX_CLICK_A;
  
  i_input -> padding -> x = 5;
  i_input -> padding -> y = 5;
  
  i_input -> mouse_position -> x = 0;
  i_input -> mouse_position -> y = 0;

  i_input -> border -> w = 200 + (i_input -> padding -> x * 2);
  i_input -> border -> h = i_font -> z_chang + (i_input -> padding -> y * 2);

  i_input -> box_type = BORDER;

  // i_input -> line.width = 100;
  
  i_input -> input_type = TEXT;
  
  i_input -> is_active = 0;
  
  i_input -> f_input_from = 0;
  
  i_input -> action_type = NONE;
  
  // i_input -> user_input_size = i_input -> user_input_size;
  // i_input -> pass_input_size = i_input -> user_input_size;
  i_input -> f_input_size = (i_input -> border -> w - i_input -> padding -> x) / i_font -> z_chang;
 

  if (i_input -> f_input != NULL) {
    
    free (i_input -> f_input);
    i_input -> f_input = NULL;
  }
  
  i_input -> f_input = (char*) calloc (i_input -> f_input_size + 1, sizeof(char));
  if (i_input -> f_input == NULL) {
    printf ("\t ! xx_set_input_width: Memory allocation, failed, i_input -> f_input\n");
    
  } 
  

  printf ("The program is reading here\n");
  
  return 1;
}


/* 
  function - void xx_render_input (SDL_Renderer*, xx_input*, bm_mgr*, int x, int y);
      - renders text as per xx_input -> user_input.
      - renders, borders, their behaviors,
*/
void xx_render_input (SDL_Renderer* i_renderer, xx_input* i_input, bm_mgr* i_font, int x, int y) {
  
  if (xx_strlen (i_input -> user_input) > i_input -> f_input_size) {
    // then we start to trim down the string
    
    // calculating
    i_input -> f_input_from = xx_strlen (i_input -> user_input) - i_input -> f_input_size;
    
    if (i_input -> input_type == PASSWORD) {
      xx_trim_n (i_input -> pass_input, i_input -> f_input, i_input -> f_input_from, i_input -> f_input_size);
    } else {
      
      // trimming the string (i_input -> user_input <source> -> i_input -> f_input <destination>)
      xx_trim_n (i_input -> user_input, i_input -> f_input, i_input -> f_input_from, i_input -> f_input_size);
    }
    
  } else {
    
    if (i_input -> input_type == PASSWORD) {
       xx_trim_n (i_input -> pass_input, i_input -> f_input, i_input -> f_input_from, i_input -> f_input_size);
    } else {
      
      xx_trim_n (i_input -> user_input, i_input -> f_input, i_input -> f_input_from, i_input -> f_input_size);
    }
  }
  
  
  if (i_input -> action_type == HOVER) {
    // changing color
    SDL_SetTextureColorMod (
      i_font -> map,
      i_input -> hover_color -> r,
      i_input -> hover_color -> g,
      i_input -> hover_color -> b
    );
    
    
    if (i_input -> input_type == PASSWORD) {
      bm_render (
        i_renderer, // renderer
        i_font, // bm_mgr
        i_input -> f_input, // char* message
        x + i_input -> padding -> x, // pos_x
        y + i_input -> padding -> y
      );
    } else {
      
      bm_render (
        i_renderer, // renderer
        i_font, // bm_mgr
        i_input -> f_input, // char* message
        x + i_input -> padding -> x, // pos_x
        y + i_input -> padding -> y
      );
    }
    
    SDL_SetRenderDrawColor (
      i_renderer,
      i_input -> hover_color -> r,
      i_input -> hover_color -> g,
      i_input -> hover_color -> b,
      i_input -> hover_color -> a
    );
    
    i_input -> border -> x = x;
    i_input -> border -> y = y;
    
    if (i_input -> box_type == LINE) {
      SDL_RenderDrawLine (
        i_renderer, 
        i_input -> border -> x,
        i_input -> border -> y + i_input -> border -> h,
        i_input -> border -> x + i_input -> border -> w,
        i_input -> border -> y + i_input -> border -> h
      );
    } else {
      
      SDL_RenderDrawRect (i_renderer, i_input -> border);
    }
    
  } else if (i_input -> action_type == ACTIVE) {
    // changing color
    SDL_SetTextureColorMod (
      i_font -> map,
      i_input -> click_color -> r,
      i_input -> click_color -> g,
      i_input -> click_color -> b
    );
    
    if (i_input -> input_type == PASSWORD) {
      bm_render (
        i_renderer, // renderer
        i_font, // bm_mgr
        i_input -> f_input, // char* message
        x + i_input -> padding -> x, // pos_x
        y + i_input -> padding -> y
      );
    } else {
      
      bm_render (
        i_renderer, // renderer
        i_font, // bm_mgr
        i_input -> f_input, // char* message
        x + i_input -> padding -> x, // pos_x
        y + i_input -> padding -> y
      );
    }
    
    SDL_SetRenderDrawColor (
      i_renderer,
      i_input -> click_color -> r,
      i_input -> click_color -> g,
      i_input -> click_color -> b,
      i_input -> click_color -> a
    );
    
    i_input -> border -> x = x;
    i_input -> border -> y = y;
    
    if (i_input -> box_type == LINE) {
      SDL_RenderDrawLine (
        i_renderer, 
        i_input -> border -> x,
        i_input -> border -> y + i_input -> border -> h,
        i_input -> border -> x + i_input -> border -> w,
        i_input -> border -> y + i_input -> border -> h
      );
    } else {
      
      SDL_RenderDrawRect (i_renderer, i_input -> border);
    }
    
  } else {
    // changing color
    SDL_SetTextureColorMod (
      i_font -> map,
      i_input -> border_color -> r,
      i_input -> border_color -> g,
      i_input -> border_color -> b
    );
    
    if (i_input -> input_type == PASSWORD) {
      bm_render (
        i_renderer, // renderer
        i_font, // bm_mgr
        i_input -> f_input, // char* message
        x + i_input -> padding -> x, // pos_x
        y + i_input -> padding -> y
      );
    } else {
      
      bm_render (
        i_renderer, // renderer
        i_font, // bm_mgr
        i_input -> f_input, // char* message
        x + i_input -> padding -> x, // pos_x
        y + i_input -> padding -> y
      );
    }
    
    SDL_SetRenderDrawColor (
      i_renderer,
      i_input -> border_color -> r,
      i_input -> border_color -> g,
      i_input -> border_color -> b,
      i_input -> border_color -> a
    );
    
    i_input -> border -> x = x;
    i_input -> border -> y = y;
    
    if (i_input -> box_type == LINE) {
      SDL_RenderDrawLine (
        i_renderer, 
        i_input -> border -> x,
        i_input -> border -> y + i_input -> border -> h,
        i_input -> border -> x + i_input -> border -> w,
        i_input -> border -> y + i_input -> border -> h
      );
    } else {
      
      SDL_RenderDrawRect (i_renderer, i_input -> border);
    }
    
  }

  if (i_input -> t_cursor -> show == 1) {
    
    xx_render_cursor (
      i_renderer, 
      i_input -> t_cursor, 
      (i_input -> border -> x + i_input -> padding -> x) + (xx_strlen (i_input -> f_input) * i_font -> z_chang) + 1,
      i_input -> border -> y + i_input -> padding -> y
    );  
  }
  
  
  
}


/* 
  function - int xx_listen_input (SDL_Renderer* i_input, xx_input* i_input, bm_mgr* i_font, SDL_Event e)
        - this function will take input and change the internal system as per the inputs
*/
int xx_listen_input (SDL_Renderer* i_renderer, xx_input* i_input, bm_mgr* i_font, SDL_Event e) {
  if (e.type == SDL_TEXTINPUT) {
    if (i_input -> is_active) {
      
      xx_strcat_ch (i_input -> user_input, (char*) e.text.text);
      xx_strcat_ch (i_input -> pass_input, (char*) "*"); // for password
    }
  } 
  if (e.type == SDL_MOUSEMOTION) {
   SDL_GetMouseState (&i_input -> mouse_position -> x, &i_input -> mouse_position -> y);
    
    if (SDL_PointInRect (i_input -> mouse_position, i_input -> border)) {
      i_input -> action_type = HOVER;
    } else {
      i_input -> action_type = NONE;
    }
  }
  if (e.type == SDL_MOUSEBUTTONDOWN) {
    if (SDL_PointInRect (i_input -> mouse_position, i_input -> border)) {
      i_input -> action_type = ACTIVE;
      if (!i_input -> is_active) {
        
        i_input -> is_active = 1;
        xx_show_cursor (i_input -> t_cursor, 1);
      } 
      // else {
        // i_input -> is_active = 0;
      // }
      // return 1;
    } else {
      i_input -> is_active = 0;
      xx_show_cursor (i_input -> t_cursor, 0);
      i_input -> action_type = NONE;
    }
  }
  
  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
      case SDLK_BACKSPACE:
        if ( i_input -> is_active ) {
          if (xx_strlen (i_input -> user_input) > 0) {
            
            xx_pop_back (i_input -> user_input);
            xx_pop_back (i_input -> pass_input);
          }

        }
        break;
      case SDLK_RETURN:
        return 1;
        break;
    }
  }
  
  
  return 0;
}

/* 
  function - void xx_set_input_type (xx_input*, Input_Type); // Input_Type - TEXT, PASSWORD
      - this function will set the TEXT / PASSWORD
*/
void xx_set_input_type (xx_input* i_input, Input_Type i_value) {
  i_input -> input_type = i_value;
}


/* 
  function - void xx_set_input_width (xx_input*, bm_mgr* i_value, size_t, int);
      - this function will set the width and update other stuff that is dependent about that
*/
void xx_set_input_width (xx_input* i_input, bm_mgr* i_value, size_t i_width, int i_height) {
  

  i_input -> border -> w = i_width + (i_input -> padding -> x * 2);

  if ( i_height < 0 ) {
    printf ("The value is %d\n", i_value -> z_chang);
    i_input -> border -> h = ( i_value -> z_chang ) + ( i_input -> padding -> y * 2 );
  } else {
    i_input -> border-> h = i_height + (i_input -> padding -> y * 2);

  }
  
  i_input -> f_input_size = (i_input -> border -> w - i_input -> padding -> x) / i_value -> z_chang;
 

  if (i_input -> f_input != NULL) {
    
    free (i_input -> f_input);
    i_input -> f_input = NULL;
  }
  
  i_input -> f_input = (char*) calloc (i_input -> f_input_size + 1, sizeof(char));
  if (i_input -> f_input == NULL) {
    printf ("\t ! xx_set_input_width: Memory allocation, failed, i_input -> f_input\n");
    
  } else {
    printf ("\t * xx_set_input_width: Memory allocation, success, i_input -> f_input\n");
    printf ("f_input_size -> %d\n", i_input -> f_input_size);
  }


}

/* 
  function - void xx_set_input_padding (xx_input*, size_t h_pad, size_t v_pad, bm_mgr* i_font)
      - this function will set the padding.x and padding.y
*/
void xx_set_input_padding (xx_input* i_input, size_t h_pad, size_t v_pad, bm_mgr* i_font) {
  i_input -> padding -> x = h_pad;
  i_input -> padding -> y = v_pad;
  
    // setting up the maximum size to be shown on the input panel
  i_input -> f_input_size = (i_input -> border -> w - i_input -> padding -> x) / i_font -> z_chang;
  
  i_input -> f_input = (char*) calloc (i_input -> f_input_size + 1, sizeof(char));
  if (i_input -> f_input == NULL) {
    printf ("\t ! xx_input::xx_create_input () - Memory allocation, failed, i_input -> f_input\n");
  } else {
    printf ("\t * xx_input::xx_create_input () - Memory allocation, success, i_input -> f_input\n");
  }
 
}

/* 
  function - void xx_set_input_border_type (xx_input* i_input, Input_Box_Type)
      - this one will set the border type
*/
void xx_set_input_border_type (xx_input* i_input, Input_Box_Type i_value) {
  i_input -> box_type = i_value;
}

/* 
  function - void xx_set_input_cursor_size (xx_input* i_input, int i_width, int i_height);
*/
void xx_set_input_cursor_size (xx_input* i_input, int i_width, int i_height) {
  i_input -> t_cursor -> cursor_box -> w = i_width;
  i_input -> t_cursor -> cursor_box -> h = i_height;
}

/* 
  function - void xx_set_input_cursor_color (xx_input* i_input, int r, int g, int b, int a);
*/
void xx_set_input_cursor_color (xx_input* i_input, int r, int g, int b, int a) {
  i_input -> t_cursor -> cursor_color -> r = r;
  i_input -> t_cursor -> cursor_color -> g = g;
  i_input -> t_cursor -> cursor_color -> b = b;
  i_input -> t_cursor -> cursor_color -> a = a;
}

/* 
  function - void xx_free_input (xx_input* i_input)
      - this function is for freeing allocated shit
*/
void xx_free_input (xx_input* i_input) {
  if (i_input -> pass_input != NULL) {
    
    free (i_input -> pass_input);
    i_input -> pass_input = NULL;
  }
  
  if (i_input -> user_input != NULL) {
    
    free (i_input -> user_input);
    i_input -> user_input = NULL;
  }
  

  if (i_input -> f_input != NULL) {
    free (i_input -> f_input);
    i_input -> f_input = NULL;
  }
  
  if (i_input -> t_cursor != NULL) {
    xx_free_cursor (i_input -> t_cursor);
    
  }

  if (i_input -> border != NULL) {
    free (i_input -> border);
    i_input -> border = NULL;
  }

  if (i_input -> text_color != NULL) {
    free (i_input -> text_color);
    i_input -> text_color = NULL;
  }
  

  if (i_input -> hover_color != NULL) {
    free (i_input -> hover_color);
    i_input -> hover_color = NULL;
  }

  if (i_input -> click_color != NULL) {
    free (i_input -> click_color);
    i_input -> click_color = NULL;
  }

  if (i_input -> border_color != NULL) {
    free (i_input -> border_color);
    i_input -> border_color = NULL;
  }

  if (i_input -> padding != NULL) {
    free (i_input -> padding);
    i_input -> padding = NULL;
  }

  if (i_input -> mouse_position != NULL) {
    free (i_input -> mouse_position);
    i_input -> mouse_position = NULL;
  }
  
  free (i_input);
}

/* 
  function - xx_input*  xx_create_input (int i_size, bm_mgr* i_font);
      -- function to allocate memory to necessary items
      -- may upgrade to everything dynamic in future
 *    -- returns the allocated memory address of xx_input to the variable on success
 *      -- returns NULL on error
*/
xx_input* xx_create_input (int i_size, bm_mgr* i_font) {
  // checking if the memory allocation size is negative or not
  if (i_size < 1) {
    printf ("\t ! xx_create_input() -> inputted integer should be greater than 0\n");
    return NULL;
  }
  
  // allocating memory to the very main struct variable
  xx_input* i_input = (xx_input*) calloc (1, sizeof(xx_input));
  if (i_input == NULL) {
    printf ("\t ! xx_input::xx_create_input () - Memory allocation, failed, i_input\n");
    return NULL;
  }


  // putting in the size of the user_input_size and pass_input_size
    i_input -> user_input_size = i_size;
    i_input -> pass_input_size = i_size;
    
    // allocating everything important
  xx_allocate_memory_input (i_input, i_font);

  return i_input;
}
 
/*
 * function - int xx_allocate_memory_input (xx_input* i_input, bm_mgr* i_font);
 *    -- this function will only allocate memory to all the necesary things
 * */
int xx_allocate_memory_input (xx_input* i_input, bm_mgr* i_font) {
  // allocating memory to the border
  i_input -> border = (SDL_Rect*) calloc (1, sizeof(SDL_Rect));
  if (i_input -> border == NULL) {
    printf ("\t ! xx_input::xx_allocate_memory_input () - Memory allocation, failed, i_input -> border\n");
    return 0;
  }

  i_input -> mouse_position = (SDL_Point*) calloc (1, sizeof(SDL_Point));
  if (i_input -> mouse_position == NULL) {
    printf ("\t ! xx_input::xx_allocate_memory_input () - Memory allocation, failed, i_input -> mouse_position\n");
    return 0;
  }


  // allocating memory to all the texts
  i_input -> text_color = (SDL_Color*) calloc (1, sizeof(SDL_Color));
  if (i_input -> text_color == NULL) {
    printf ("\t ! xx_input::xx_allocate_memory_input () -> memory allocation, failed, i_input -> text_color\n");
    return 0;
  }
 
  i_input -> border_color = (SDL_Color*) calloc (1, sizeof(SDL_Color));
  if (i_input -> border_color == NULL) {
    printf ("\t ! xx_input::xx_allocate_memory_input () -> memory allocation, failed, i_input -> border_color\n");
    return 0;
  }

   i_input -> hover_color = (SDL_Color*) calloc (1, sizeof(SDL_Color));
  if (i_input -> hover_color == NULL) {
    printf ("\t ! xx_input::xx_allocate_memory_input () -> memory allocation, failed, i_input -> hover_color\n");
    return 0;
  }

   i_input -> click_color = (SDL_Color*) calloc (1, sizeof(SDL_Color));
  if (i_input -> click_color == NULL) {
    printf ("\t ! xx_input::xx_allocate_memory_input () -> memory allocation, failed, i_input -> click_color\n");
    return 0;
  }
 
  // allocating memory to padding
  i_input -> padding = (SDL_Point*) calloc (1, sizeof(SDL_Point));
  if (i_input -> padding == NULL) {
    printf ("\t ! xx_input::xx_allocate_memory_input () -> Memory allocation, failed, i_input -> padding\n");
    return 0;
  }


    // allocating memory to the user_input
  i_input -> user_input = (char*) calloc (i_input -> user_input_size + 1, sizeof(char));
  if (i_input -> user_input == NULL) {
    printf ("\t ! xx_input::xx_create_input () - Memory allocation, failed, i_input -> user_input\n");
    return 0;
  } else {
    printf ("\t * xx_input::xx_set_create_input () - Memory allocation, success, i_input -> user_input\n");
  }
  
 
  i_input -> pass_input = (char*) calloc (i_input -> pass_input_size + 1, sizeof(char));
  if (i_input -> pass_input == NULL) {
    printf ("\t ! xx_input::xx_create_input () - Memory allocation, failed, i_input -> pass_input\n");
    return 0;
  } else {
    printf ("\t * xx_input::xx_create_input () - Memory allocation, success, i_input -> pass_input\n");
  }
  
  i_input -> t_cursor = xx_create_cursor ();
  if ( i_input -> t_cursor == NULL ) {
    printf ("\t Error! xx_input::xx_create_input(), i_input -> t_cursor\n");
    return 0;
  }
  
  xx_set_cursor_default (i_input -> t_cursor, i_font -> z_chang);

  if (!xx_set_input_default (i_input, i_font)) {
    printf ("\t Error! xx_input::xx_set_input_default () -> Could not set default\n");
    return 0;
  }
  
 
  return 1;
}

/* 
 * function - getters
 * */
int xx_get_input_total_width ( xx_input* i_input ) {
  return i_input -> border -> w;
}

int xx_get_input_total_height ( xx_input* i_input ) {
  return i_input -> border -> h;
}
