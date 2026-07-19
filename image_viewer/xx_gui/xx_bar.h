/* 
 * this is just bar, simple ass bar, that will do nothing but will be used in I guess, xx_input, and xx_grid
 * the primary need for this bar is to scroll down or scroll horizontally to see more of the elements
 * */
#define XX_BAR 1111
/*
 * what I want from this library
 *  - I want it to handle inputs
 *  - I want it to render properly
 * */

typedef struct {

  SDL_FPoint* mouse_position;
  SDL_FRect* bar_size;
  SDL_Color* bar_color;

  int show; // 0 by default
  int is_vertical; // 0 by default - meaning _ bar
  int selected; // 0 by default
  
  int Screen_Width;
  int Screen_Height;
} xx_bar;

/* 
 * function - declarations
 * */
xx_bar* xx_create_bar (int Screen_Width, int Screen_Height);
void xx_free_bar ( xx_bar* i_bar );
int xx_listen_bar ( xx_bar* i_bar, SDL_Event i_event );
void xx_render_bar ( SDL_Renderer* i_renderer, xx_bar* i_bar );
void xx_set_bar ( xx_bar* i_bar, float x, float y );// just updates the coordinates of the bar
void xx_calculate_bar  ( xx_bar* i_bar, int diff ); // calculates the difference and then creates width and height

void xx_show_bar ( xx_bar* i_bar, int value ); // will have 0 value on negative values and 1 on positive values
void xx_set_bar_orientation ( xx_bar* i_bar, int is_set );


void xx_fill_default_value_bar ( xx_bar* i_bar, int Screen_Width, int Screen_Height );
void xx_border_checking_bar ( xx_bar* i_bar );

/* 
 * function definitions
 * */
xx_bar* xx_create_bar (int Screen_Width, int Screen_Height) {
  xx_bar* r_bar = ( xx_bar* ) malloc ( sizeof ( xx_bar ) * 1 );
  if (r_bar == NULL) {
    printf ("\t Error! Memory allocation, FAILED, xx_bar* r_bar\n");
    return NULL;
  }

  r_bar -> bar_color = (SDL_Color*) malloc (sizeof (SDL_Color) * 1);
  if ( r_bar -> bar_color == NULL ) {
    printf ("\t Error! Memory allocation, FAILED, xx_bar* r_bar -> bar_color\n");
    return NULL;
  }

  r_bar -> bar_size = (SDL_FRect*) malloc (sizeof(SDL_FRect) * 1);
  if (r_bar -> bar_size == NULL) {
    printf ("\t Error! Memory Allocation, FAILED, xx_bar* r_bar -> bar_size\n");
    return NULL;
  }

  r_bar -> mouse_position = (SDL_FPoint*) malloc (sizeof(SDL_FPoint) * 1);
  if (r_bar -> mouse_position == NULL) {

    printf ("\t Error! Memory allocation, FAILED, xx_bar* r_bar -> mouse_position\n");
    return NULL;
  }

  // fill in default values with this function
  xx_fill_default_value_bar ( r_bar, Screen_Width, Screen_Height );

  return r_bar;
}

/* 
 * function - void xx_fill_default_value_bar ( xx_bar* i_bar, int Screen_Width, int Screen_Height );
 *  -- this function will just fill in the default values for the bar
 * */
void xx_fill_default_value_bar ( xx_bar* i_bar, int Screen_Width, int Screen_Height ) {
  i_bar -> bar_size -> w = 100.0f;
  i_bar -> bar_size -> h = 10.0f;

  i_bar -> bar_color -> r = 200;
  i_bar -> bar_color -> g = 200;
  i_bar -> bar_color -> b = 200;
  i_bar -> bar_color -> a = 200;

  i_bar -> mouse_position -> x = 0.0f;
  i_bar -> mouse_position -> y = 0.0f;

  i_bar -> show = 0;
  i_bar -> selected = 0;
  i_bar -> is_vertical = 0;

  i_bar -> Screen_Width = Screen_Width;
  i_bar -> Screen_Height = Screen_Height;

}


/* 
 * function - void xx_render_bar ( SDL_Renderer* i_renderer, xx_bar* i_bar )
 *  -- this function will render the bar only if the status is on
 * */
void xx_render_bar ( SDL_Renderer* i_renderer, xx_bar* i_bar ) {
  if ( i_bar -> show ) {
    xx_border_checking_bar ( i_bar );

    SDL_SetRenderDrawColor ( i_renderer, i_bar -> bar_color -> r, i_bar -> bar_color -> g, i_bar -> bar_color -> b, i_bar -> bar_color -> a );


    SDL_RenderFillRectF ( i_renderer, i_bar -> bar_size );
  }
}

/* 
 * function - void xx_show_bar ( xx_bar* i_bar, int value )
 *  -- just toggles if we should be should the bar or not, default value is 0
 * */
void xx_show_bar ( xx_bar* i_bar, int value ) {
  int s = 0;

  if (value < 0) {
    s = 0;
  } else if (value > 0) {
    s = 1;
  }


  i_bar -> show = s;
}

/* 
 * function - int xx_listen_bar ( xx_bar* i_bar, SDL_Event i_event )
 *  -- will listen to the inputs and then do something
 *
 * */
int xx_listen_bar ( xx_bar* i_bar, SDL_Event i_event ) {
  if ( i_bar -> show ) {

    if (i_event.type == SDL_MOUSEMOTION) {
      i_bar -> mouse_position -> x = i_event.motion.x;
      i_bar -> mouse_position -> y = i_event.motion.y;

      if ( i_bar -> selected ) {
        if ( i_bar -> is_vertical ) { // if it is set to vertical
          i_bar -> bar_size -> y = i_bar -> mouse_position -> y;
        } else if ( !i_bar -> is_vertical ) {
          i_bar -> bar_size -> x = i_bar -> mouse_position -> x;
        }

      }
    }

    if (i_event.type == SDL_MOUSEBUTTONDOWN ){
      if (SDL_PointInFRect (i_bar -> mouse_position, i_bar -> bar_size)) {
        i_bar -> selected = 1;

        // xx_set_bar ( i_bar, i_bar -> mouse_position -> x, i_bar -> mouse_position -> y);
      }
    } else if (i_event.type == SDL_MOUSEBUTTONUP) {
      i_bar -> selected = 0;
    }
  }

  return 1;
}

/* 
 * function - void xx_free_bar ( xx_bar* i_bar ) {
 *  -- just frees the allocated resources
 * */
void xx_free_bar ( xx_bar* i_bar ) {
  if (i_bar != NULL) {
    free ( i_bar -> bar_color );
    i_bar -> bar_color = NULL;

    free ( i_bar -> bar_size );
    i_bar -> bar_size = NULL;

    free ( i_bar -> mouse_position );
    i_bar -> mouse_position = NULL;

    free ( i_bar );
    i_bar = NULL;
  }
}

/* 
 * function - void xx_set_bar ( xx_bar* i_bar, float x, float y );
 *  -- just updates the coordinates x and y
 * */
void xx_set_bar ( xx_bar* i_bar, float x, float y ) {
  i_bar -> bar_size -> x = x;
  i_bar -> bar_size -> y = y;
}

/* 
 * function - void xx_calculate_bar ( xx_bar* i_bar, int diff )
 *  -- calculates the width or height (according to i_bar -> is_vertical)
 * */
void xx_calculate_bar ( xx_bar* i_bar, int diff ) {
  //int r = diff - max;
  if ( i_bar -> is_vertical ) { // if the setting is vertical
    i_bar -> bar_size -> w = 5.0f;
    //i_bar -> bar_size -> h = (float) (max - diff);
    i_bar -> bar_size -> h = (float) (i_bar -> Screen_Height - diff);
  } else {
    //i_bar -> bar_size -> w = (float) (max - diff);
    i_bar -> bar_size -> w = (float) (i_bar -> Screen_Width - diff);
    i_bar -> bar_size -> h = 5.0f;
  }
}

/* 
 * function - void xx_set_bar_orientation ( xx_bar* i_bar, int is_set ) 
 *  -- updates if is_vertical
 * */
void xx_set_bar_orientation ( xx_bar* i_bar, int is_set ) {
  if ( is_set < 0 ) {
    i_bar -> is_vertical = 0;
  } else if ( is_set > 0 ) {
    i_bar -> is_vertical = 1;
  }
}

/* 
 *  function - void xx_border_checking_bar ( xx_bar* i_bar )
 *    -- checks for the border
 * */
void xx_border_checking_bar ( xx_bar* i_bar ) {

  if (i_bar -> is_vertical) {

    if (i_bar -> bar_size -> y < 0) {
      i_bar -> bar_size -> y = 0;

    } else if ( (i_bar -> bar_size -> y + i_bar -> bar_size -> h) > i_bar -> Screen_Height ) {
      i_bar -> bar_size -> y =  i_bar -> Screen_Height - i_bar -> bar_size -> h;
    }
  } else {
    if ( i_bar -> bar_size -> x < 0 ) {
      i_bar -> bar_size -> x = 0;
    } else if ( (i_bar -> bar_size -> x + i_bar -> bar_size -> w) > i_bar -> Screen_Width ) {
      i_bar -> bar_size -> x = i_bar -> Screen_Width - i_bar -> bar_size -> w;
    }
  }
}
