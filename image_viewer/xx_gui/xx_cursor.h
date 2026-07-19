/* 
  this cursor is for xx_input.h
*/
#define XX_CURSOR 1231

typedef struct {
  SDL_Rect* cursor_box; // the cursor will be a block
  SDL_Color* cursor_color;
  int show; // determines to show or not show
} xx_cursor;

/* 
  function - declarations
*/
xx_cursor* xx_create_cursor (); // creates cursor
void xx_set_cursor (xx_cursor* i_input, int i_width, int i_height, int r, int g, int b, int a); // set by the user
void xx_set_cursor_default (xx_cursor* i_input, int height); 
void xx_render_cursor (SDL_Renderer* i_renderer, xx_cursor* i_input, int pos_x, int pos_y);
void xx_free_cursor (xx_cursor* i_input);
void xx_show_cursor (xx_cursor* i_input, int i); // i = 1 - yes , 0 - no


/* 
  function - definitions
*/

/* 
  function - void xx_create_cursor (xx_cursor* i_input)
      - creates memory and loads it to the inputted xx_cursor
 */
xx_cursor* xx_create_cursor () {
  xx_cursor* i_input = (xx_cursor*) malloc (sizeof(xx_cursor) * 1);
  if (i_input == NULL) {
    printf ("\t Error! xx_create_cursor::Memory allocation, failed, i_input\n");
    return NULL;
  } else {
    printf ("\t Succ* xx_create_cursor::Memory allocation, success, i_input\n");
  }

  i_input -> cursor_box = (SDL_Rect*) calloc (1, sizeof(SDL_Rect));
  if (i_input -> cursor_box == NULL) {
    printf ("\t ! xx_create_cursor::Memory allocation, failed, i_input -> cursor_box\n");
    return NULL;
  } else {
    printf ("\t * xx_create_cursor::Memory allocation, success, i_input -> cursor_box \n");
  }
  
  i_input -> cursor_color = (SDL_Color*) calloc (1, sizeof(SDL_Color));
  if (i_input -> cursor_color == NULL) {
    printf ("\t ! xx_create_cursor::Memory allocation, failed, i_input -> cursor_color\n");
    return NULL;
  } else {
    printf ("\t * xx_create_cursor::Memory allocation, success, i_input -> cursor_color\n");
  }

  return i_input;
  
}

/* 
  function - void xx_set_cursor (xx_cursor* i_input, int i_width, int i_height, int r, int g, int b)
        - loads the rect and color with user inputted value
 */
void xx_set_cursor (xx_cursor* i_input, int i_width, int i_height, int r, int g, int b, int a) {
  i_input -> cursor_box -> w = i_width;
  i_input -> cursor_box -> h = i_height;
  
  i_input -> cursor_color -> r = r;
  i_input -> cursor_color -> g = g;
  i_input -> cursor_color -> b = b;
  i_input -> cursor_color -> a = a;
}

/* 
  function - void xx_set_cursor_default (xx_cursor* i_input, int height);
        - fills in the values with default values
*/
void xx_set_cursor_default (xx_cursor* i_input, int height) {
  i_input -> cursor_box -> w = 5;
  i_input -> cursor_box -> h = height;
  
  i_input -> cursor_color -> r = 113;
  i_input -> cursor_color -> g = 113;
  i_input -> cursor_color -> b = 113;
  i_input -> cursor_color -> a = 255; 

  i_input -> show = 0;
} 


/* 
  function - void xx_render_cursor (SDL_Renderer* i_renderer, xx_cursor* i_input, int pos_x, int pos_y);
        - rendering the values 
*/
void xx_render_cursor (SDL_Renderer* i_renderer, xx_cursor* i_input, int pos_x, int pos_y) {
  
  if (i_input -> show) {
    
    i_input -> cursor_box -> x = pos_x;
    i_input -> cursor_box -> y = pos_y;
    
    SDL_SetRenderDrawColor (
      i_renderer,
      i_input -> cursor_color -> r,
      i_input -> cursor_color -> g,
      i_input -> cursor_color -> b,
      i_input -> cursor_color -> a
    );
    
    SDL_RenderFillRect (i_renderer, i_input -> cursor_box);
  }
}

/* 
  function - void xx_free_cursor (xx_cursor* i_input);
      - just clears the allocated memory
*/
void xx_free_cursor (xx_cursor* i_input) {
  if (i_input -> cursor_box != NULL) {
    free (i_input -> cursor_box);
    i_input -> cursor_box = NULL;
  }
  
  if (i_input -> cursor_color != NULL) {
    free (i_input -> cursor_color);
    i_input -> cursor_color = NULL;
  }

  free (i_input);
  i_input = NULL;
}

/* 
  function - void xx_show_cursor (xx_cursor* i_input, int i);
      - just switches show cursor
 */
void xx_show_cursor (xx_cursor* i_input, int i) {
  if (i >= 1) {
    
    i_input -> show = 1;
  } else {
    i_input -> show = 0;
  }
}
