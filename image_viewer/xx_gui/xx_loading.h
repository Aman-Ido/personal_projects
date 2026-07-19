/* 
 * this is loading bar for xx_gui
 * */

#define XX_LOADING  3030

/* 
 * struct to store the SDL_FRect and SDL_Color
 * */
typedef struct {

  SDL_FRect* max_size; // for the max size
  SDL_FRect* curr_size; // for current value going forward
  
  SDL_Color* border_color;
  SDL_Color* loading_color;

  float max_value; // this is the max value, if you are going to make a loading of 100 then this should be set to 100
  float change_value; // this value changes and according to that ratio we will update the graphics, while this changes 
  
  float const_change; // this is the constant change, when we do ratio calculation we will put the result here and according to this we will update 
                      // change value
  int is_full;

} xx_loading;

/* 
 * function - declarations
 * */
xx_loading* xx_create_loading_bar ();
void xx_free_loading_bar (xx_loading* i_loading);

void xx_default_loading_bar (xx_loading* i_loading);
void xx_calculate_loading_bar ( xx_loading* i_loading );

void xx_set_loading_bar_color ( xx_loading* i_loading, int r, int g, int b, int a );
void xx_set_border_bar_color ( xx_loading* i_loading, int r, int g, int b, int a );
void xx_set_loading_bar_size ( xx_loading* i_loading, float width, float height );

void xx_set_max_value ( xx_loading* i_loading, float i_value ); // call before xx_calculate_loading_bar

void xx_update_loading_bar ( xx_loading* i_loading, float i_data );

void xx_render_loading_bar ( SDL_Renderer* i_renderer, xx_loading* i_loading, float x, float y );

int xx_get_loading_total_width ( xx_loading* i_loading );
int xx_get_loading_total_height ( xx_loading* i_loading );

/* 
 * function - definitinos
 * */
xx_loading* xx_create_loading_bar () {
  xx_loading* r_loading = ( xx_loading* ) malloc (sizeof(xx_loading) * 1);
  if (r_loading == NULL) {
    printf ("\t ! Memory allocation, failed, xx_loading* r_loading\n");
    return NULL;
  }

  r_loading -> border_color = ( SDL_Color* ) malloc ( sizeof(SDL_Color) * 1);
  if ( r_loading -> border_color == NULL) {
    printf ("\t ! Memory allocation, failed, xx_loading* r_loading -> border_color \n");
    return NULL;

  }

  r_loading -> loading_color = ( SDL_Color* ) malloc ( sizeof(SDL_Color) * 1);
  if ( r_loading -> loading_color == NULL) {
    printf ("\t ! Memory allocation, failed, xx_loading* r_loading -> loading_color \n");
    return NULL;
  }



  r_loading -> max_size = ( SDL_FRect* ) malloc ( sizeof(SDL_FRect) * 1);
  if ( r_loading -> max_size == NULL) {
    printf ("\t ! Memory allocation, failed, xx_loading* r_loading -> max_size \n");
    return NULL;
  }

  r_loading -> curr_size = ( SDL_FRect* ) malloc ( sizeof(SDL_FRect) * 1);
  if ( r_loading -> curr_size == NULL) {
    printf ("\t ! Memory allocation, failed, xx_loading* r_loading -> curr_size \n");
    return NULL;
  }

  xx_default_loading_bar ( r_loading );


  return r_loading;

}


/* 
 * function - void xx_free_loading_bar (xx_loading* i_loading);
 * */
void xx_free_loading_bar ( xx_loading* i_loading ) {
  if (i_loading != NULL) {
    free (i_loading -> border_color);
    i_loading -> border_color = NULL;

    free (i_loading -> loading_color);
    i_loading -> loading_color = NULL;
    
    free (i_loading -> curr_size);
    i_loading -> curr_size = NULL;

    free (i_loading -> max_size);
    i_loading -> max_size = NULL;

    free (i_loading);
    i_loading = NULL;
  }

}

/* 
 * function - void xx_default_loading_bar (xx_loading* i_loading);
 *  -- this function will fill in the default values
 * */
void xx_default_loading_bar (xx_loading* i_loading) {
  i_loading -> max_size -> x = 0.0f;
  i_loading -> max_size -> y = 0.0f;
  i_loading -> max_size -> w = 300.0f;
  i_loading -> max_size -> h = 50.0f;

  i_loading -> curr_size -> x = i_loading -> max_size -> x;
  i_loading -> curr_size -> y = i_loading -> max_size -> y;
  i_loading -> curr_size -> h = i_loading -> max_size -> h;
  i_loading -> curr_size -> w = i_loading -> max_size -> x;

  i_loading -> loading_color -> r = 255;  
  i_loading -> loading_color -> g = 0;  
  i_loading -> loading_color -> b = 5;  
  i_loading -> loading_color -> a = 255;  

  i_loading -> border_color -> r = 0;  
  i_loading -> border_color -> g = 0;  
  i_loading -> border_color -> b = 0;  
  i_loading -> border_color -> a = 255;  

  i_loading -> max_value = 0.0f;
  i_loading -> change_value = 0.0f;
  i_loading -> const_change = 0.0f;

  i_loading -> is_full = 0;
}

/* 
 * function - void xx_calculate_loading_bar ( xx_loading* i_loading );
 *  -- this function will calculate and update the max_value, const_change value , just update max_value
 * */
void xx_calculate_loading_bar ( xx_loading* i_loading ) {
  float result = i_loading -> max_size -> w / i_loading -> max_value;
  if ( result == 0.0f ) {
    result = i_loading -> max_size -> w;
  }

  i_loading -> const_change = result;
}

/* 
 * function - void xx_set_max_value ( xx_loading* i_loading, float i_value );
 *  -- this function will update the max size, call before xx_calculate_loading_bar
 * */
void xx_set_max_value ( xx_loading* i_loading, float i_value ) {
  if (i_value == 0.0f) {
    printf ("\t\t Warning! xx_set_max_value::Entered i_value is less than 0.0f\n");
  }
  i_loading -> max_value = i_value;
}

/* 
 * function - void xx_render_loading_bar ( SDL_Renderer* i_renderer, xx_loading* i_loading, float x, float y )
 *  -- just renders the loading bar with the properties of i_loading
 * */
void xx_render_loading_bar ( SDL_Renderer* i_renderer, xx_loading* i_loading, float x, float y ) {

  i_loading -> max_size -> x = x;
  i_loading -> max_size -> y = y;
  /* changing the color of the border */
  SDL_SetRenderDrawColor ( i_renderer,
     i_loading -> border_color -> r, i_loading -> border_color -> g,
    i_loading -> border_color -> b, i_loading -> border_color -> a );

  SDL_RenderDrawRectF ( i_renderer, i_loading -> max_size );

  i_loading -> curr_size -> x = i_loading -> max_size -> x;
  i_loading -> curr_size -> y = i_loading -> max_size -> y;

  /* changing the color of the loading bar */
  SDL_SetRenderDrawColor ( i_renderer,
     i_loading -> loading_color -> r, i_loading -> loading_color -> g,
    i_loading -> loading_color -> b, i_loading -> loading_color -> a );

  SDL_RenderFillRectF ( i_renderer, i_loading -> curr_size );

}

/* 
 * function - void xx_set_border_bar_color ( xx_loading* i_loading, int r, int g, int b, int a );
 *   -- just changes the color of the border
 * */
void xx_set_border_bar_color ( xx_loading* i_loading, int r, int g, int b, int a ) {
  i_loading -> border_color -> r = r;
  i_loading -> border_color -> g = g;
  i_loading -> border_color -> b = b;
  i_loading -> border_color -> a = a;
}


/* 
 * function - void xx_set_loading_bar_color ( xx_loading* i_loading, int r, int g, int b, int a );
 *   -- just changes the color of the loading bar
 * */
void xx_set_loading_bar_color ( xx_loading* i_loading, int r, int g, int b, int a ) {
  i_loading -> loading_color -> r = r;
  i_loading -> loading_color -> g = g;
  i_loading -> loading_color -> b = b;
  i_loading -> loading_color -> a = a;
}

/* 
 * function - void xx_set_loading_bar_size ( xx_loading* i_loading, float width, float height );
 *     -- changes the size of the whole things (i_loading -> max_size -> width and height)
 * */
void xx_set_loading_bar_size ( xx_loading* i_loading, float width, float height ) {
  i_loading -> max_size -> w = width;
  i_loading -> max_size -> h = height;

  i_loading -> curr_size -> h = i_loading -> max_size -> h;
}

/* 
 * function - void xx_update_loading_bar ( xx_loading* i_loading, float i_data )
 *    -- function to update the loading bar 
 * */
void xx_update_loading_bar ( xx_loading* i_loading, float i_data ) {
  if ( !i_loading -> is_full ) {
    i_loading -> change_value = i_data;
    i_loading -> curr_size -> w = i_loading -> change_value * i_loading -> const_change;
  }

  if ( i_loading -> max_value < i_loading -> change_value ) {
    i_loading -> is_full = 1;
  }
}

/*
 * function - int xx_get_loading_total_width ( xx_loading* i_loading );
 *    -- returns the total width of the loading bar
 */
int xx_get_loading_total_width ( xx_loading* i_loading ) {
  return (int) i_loading -> max_size -> w;
}

/*
 * function - int xx_get_loading_total_height ( xx_loading* i_loading );
 *    -- returns the total height of the loading bar
 */

int xx_get_loading_total_height ( xx_loading* i_loading ) {
  return (int) i_loading -> max_size -> h;
}
