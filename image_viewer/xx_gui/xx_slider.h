/* 
 * this one is a slider, but there wouldn't be much of customizablility and also
 * the button won't be circle but a box 
 * */
#define XX_SLIDER 10

/* 
#ifndef XX_MATH
#include "xx_math.h"
#endif
*/

typedef struct {

  SDL_Rect** main_container; // it will be the main box also with the constant information of the box
                             // meaning main_container [0] -> will have the box (the main container)
                             // main_container[1] -> will have the information about the line from where to where kind of thing 
                             // main_container[2] -> will be the cursor
                             // main_container[3] -> padding
  int* main_value; // these will the main value 
                   // main_value [0] -> will be the total value (max_value)
                   // main_value [1] -> default_value
                   // main_value [2] -> change ( main pixel_width / max_width ) - scaled
                   // main_value [3] ->  curr position (current value)
  int main_value_count; // 4
  int main_container_count; // will be 4
  int is_pressed;

  char* string; // for graphical showcase

} xx_slider;

/* 
 * function - declarations
 * */
xx_slider* xx_create_slider ( int max_value, int default_value, int line_width, int box_x, int box_y, int box_width, int box_height );
void xx_free_slider ( xx_slider* i_slider );
void xx_set_default_value_slider ( xx_slider* i_slider,  int max_value, int default_value, int line_width, int box_x, int box_y, int box_width, int box_height );
void xx_render_slider ( SDL_Renderer* i_renderer, xx_slider* i_slider );
void xx_render_slider_text (   SDL_Renderer* i_renderer, xx_slider* i_slider, bm_mgr* i_font ); // rendering the cursor but with also a text on top
int xx_listen_slider ( xx_slider* i_slider, SDL_Event e );


void xx_print_slider_info ( xx_slider* i_slider );

int xx_get_slider_info ( xx_slider* i_slider );

int xx_get_slider_total_width ( xx_slider* i_slider );
int xx_get_slider_total_height ( xx_slider* i_slider );

/* 
 * */

/* 
 * function - xx_slider* xx_create_slider ( int max_value, int default_value, int line_width, int box_x, int box_y, int box_width, int box_height );
 *    -- allocates memory to the necessary and returns the value
 * */
xx_slider* xx_create_slider ( int max_value, int default_value, int line_width, int box_x, int box_y, int box_width, int box_height ) {

  xx_slider* r_slider = (xx_slider*) malloc (sizeof(xx_slider) * 1);
  if ( r_slider == NULL ) {
    printf ("\t Error! Memory Allocation, Failed, xx_slider*\n");
    return NULL;
  }

  r_slider -> main_container_count = 4;
  r_slider -> main_value_count = 4;

  r_slider -> main_container = (SDL_Rect**) malloc (sizeof(SDL_Rect*) * r_slider -> main_container_count);
  if ( r_slider -> main_container == NULL ) {
    printf ("\t Error! Memory Allocation, failed, xx_slider* -> SDL_Rect**\n");
    return NULL;
  }

  for (int i = 0; i < r_slider -> main_container_count; i++) {
    r_slider -> main_container [i] = (SDL_Rect*) malloc (sizeof(SDL_Rect) * 1);
    if ( r_slider -> main_container[i] == NULL ) {
      printf ("\t Error! Memory Allocation, failed, xx_slider* -> SDL_Rect** -> [%d]\n", i);
      return NULL;
    }
  }

  r_slider -> main_value = (int*) malloc (sizeof(int) * r_slider -> main_value_count);
  if ( r_slider -> main_value == NULL ) {
    printf ("\t Error! Memory Allocation, failed, xx_slider* -> int*\n");
    return NULL;
  }

  r_slider -> string = (char*) calloc (10, sizeof(char));
  if ( r_slider -> string == NULL ) {
    printf ("\t Error! Memory Allocation, failed, xx_slider* -> char*\n");
    return NULL;
  }

  xx_set_default_value_slider ( r_slider, max_value, default_value, line_width, box_x, box_y, box_width, box_height );
  


  return r_slider;
}

/* 
 * function - void xx_free_slider ( xx_slider* i_slider );
 *    -- frees the memory allocated by the slider
 * */
void xx_free_slider ( xx_slider* i_slider ) {
  if ( i_slider != NULL ) {
    for (int i = 0; i < i_slider -> main_container_count; i++) {
      free ( i_slider -> main_container [i] );
      i_slider -> main_container [i] = NULL;
    }

    free ( i_slider -> main_container );
    i_slider -> main_container = NULL;

    free ( i_slider -> main_value );
    i_slider -> main_value = NULL;

    free ( i_slider -> string );
    i_slider -> string = NULL;

   free ( i_slider );
    i_slider = NULL; 
  }
}

/* 
 * function - void xx_set_default_value_slider ( xx_slider* i_slider,  int max_value, int default_value, int line_width, int box_x, int box_y, int box_width, int box_height )
 *    -- this function will set the default value for the slider 
 * */
void xx_set_default_value_slider ( xx_slider* i_slider,  int max_value, int default_value, int line_width, int box_x, int box_y, int box_width, int box_height ) {
  // filling in the main box ( which will have defaults )
  if ( box_width < 0 || box_height < 0 ) {
    
    if ( box_width < 0 ) {
      i_slider -> main_container [0] -> w = 200;
    } else if ( box_height < 0 ) {
      i_slider -> main_container [0] -> h = 43;
    }
  } else {
    i_slider -> main_container [0] -> w = box_width;
    i_slider -> main_container [0] -> h = box_height;
  }

  i_slider -> main_container [0] -> x = box_x;
  i_slider -> main_container [0] -> y = box_y;

  // padding
  i_slider -> main_container [3] -> x = 20; // left
  i_slider -> main_container [3] -> y = 20; // right
  i_slider -> main_container [3] -> w = 20; // top
  i_slider -> main_container [3] -> h = 20; // bottom

                                      

  // line
  // if the line_width is less than 0 then we will fill it with default value
  if ( line_width < 0 ) {
    i_slider -> main_container [1] -> w = i_slider -> main_container [0] -> w - ( i_slider -> main_container [3] -> x + i_slider -> main_container [3] -> y );
  } else {
    // if the user inputs line_width more than 0, and has line_width greater than that of difference of (box_width and 2 * padding); 
    int x_width = ( i_slider -> main_container [0] -> w - ( i_slider -> main_container [3] -> x + i_slider -> main_container [3] -> y ) ) < line_width;
    if ( x_width == 1 ) {
      printf ("\t Warning! Entered, line_width is greater than (box_width - horizontal_padding * 2 (which is 20)), Conerting to the suitable width\n");
      i_slider -> main_container [1] -> w = i_slider -> main_container [0] -> x + i_slider -> main_container [0] -> w - ( i_slider -> main_container [3] -> y );
    } else if ( x_width == 0 ) {
      // if evertyhing is fine we will put in user inputted value
      i_slider -> main_container [1] -> w = line_width;
    }
  }
  i_slider -> main_container [1] -> h = i_slider -> main_container [0] -> y + ( i_slider -> main_container [0] -> h / 2 ); 
  i_slider -> main_container [1] -> x = i_slider -> main_container [0] -> x + i_slider -> main_container [3] -> x;
  // i_slider -> main_container [1] -> y = i_slider -> main_container [0] -> y + i_slider -> main_container [3] -> w;
  i_slider -> main_container [1] -> y = i_slider -> main_container [1] -> h;
  // i_slider -> main_container [1] -> w = i_slider -> main_container [0] -> x + i_slider -> main_container [0] -> w - i_slider -> main_container [3] -> y;

  i_slider -> is_pressed = 0;
  // cursor
  i_slider -> main_value [0] = max_value;
  if ( default_value < 0 ) {
    default_value = 0;
  }
  i_slider -> main_value [1] = default_value;
  i_slider -> main_value [2] = ( i_slider -> main_container [1] -> w / i_slider -> main_value [0] );
  i_slider -> main_value [3] = i_slider -> main_value [1] * i_slider -> main_value [2];

  i_slider -> main_container [2] -> w = 20;
  i_slider -> main_container [2] -> h = 20;
  i_slider -> main_container [2] -> x = i_slider -> main_value [3] * i_slider -> main_value [2];
  i_slider -> main_container [2] -> y = i_slider -> main_container [1] -> y - ( i_slider -> main_container [2] -> h / 2 ); 

}


/*
 * function - void xx_print_slider_info ( xx_slider* );
 *    -- this function will only print out stuff from the i_slider 
 * */
void xx_print_slider_info ( xx_slider* i_slider ) {
  printf ("\n\n\t xx_slider* i_slider -> information \n");

  char container_information [4][20] = {
    "Box Container", "Line Container", "Cursor Size", "Padding"
  };

  char value_information [4][20] = {
    "Max Value", "Default Value", "Changing Value", "Curr Pos"
  };

  for (int i = 0; i < i_slider -> main_container_count; i++) {
    printf ("\t\t %s -> {%d, %d, %d, %d}\n", 
        container_information [i],
        i_slider -> main_container [i] -> x,
        i_slider -> main_container [i] -> y,
        i_slider -> main_container [i] -> w,
        i_slider -> main_container [i] -> h
        );
  }

  printf ("\n\n");

  for (int i = 0; i < i_slider -> main_value_count; i++) {
    printf ("\t\t %s -> %d\n", value_information [i], i_slider -> main_value [i]);
  }

  printf ("\n\n");
  printf ("\t Max Number of Box -> %d\n\t Max Number of Value -> %d\n", i_slider -> main_container_count, i_slider -> main_value_count);
  printf ("\t Memory -> %p, Size of string -> 20\n", i_slider -> string );
  printf ("\t Size of xx_slider -> %ld\n", sizeof(xx_slider));
}

/* 
 * function - void xx_render_slider ( SDL_Renderer*, xx_slider* );
 *    -- function to render on the screen
 * */
void xx_render_slider ( SDL_Renderer* i_renderer, xx_slider* i_slider ) {

  SDL_SetRenderDrawColor ( i_renderer, 150, 150, 185, 200 );
  SDL_RenderDrawRect ( i_renderer, i_slider -> main_container [0] );

  SDL_SetRenderDrawColor ( i_renderer, 50, 75, 90, 200 );
  SDL_RenderDrawLine ( 
      i_renderer,
      i_slider -> main_container [1] -> x,
      i_slider -> main_container [1] -> y,
      i_slider -> main_container [1] -> w,
      i_slider -> main_container [1] -> h
      );

  SDL_SetRenderDrawColor ( i_renderer, 255, 0, 50, 255 );
  SDL_RenderFillRect ( i_renderer, i_slider -> main_container [2] );
}


/* 
 * Function - int xx_listen_slider ( xx_slider* i_slider, SDL_Event e );
 *    -- function to listen to the inputs from the user
 * */
int xx_listen_slider ( xx_slider* i_slider, SDL_Event e ) {
  SDL_Point mouse_position = {0};
  SDL_GetMouseState ( &mouse_position.x, &mouse_position.y );
  if (e.type == SDL_MOUSEBUTTONDOWN) {
    mouse_position.x = e.button.x;
    mouse_position.y = e.button.y;

    if ( SDL_PointInRect (&mouse_position, i_slider -> main_container [2]) ) {
      i_slider -> is_pressed = 1;
    }
  }
  if (e.type == SDL_MOUSEBUTTONUP) {
    if (i_slider -> is_pressed == 1) {
      i_slider -> is_pressed = 0;
      return 1;
    }
  }

  if (e.type == SDL_MOUSEMOTION) {
    // SDL_GetMouseState ( &mouse_position.x, &mouse_position.y );
    if ( i_slider -> is_pressed ) {
      //i_slider -> main_container [2] -> x = i_slider -> main_container [2] -> x + mouse_position.x;
      // i_slider -> main_value [3] = i_slider -> main_value [3] + mouse_position.x;
      i_slider -> main_container [2] -> x = mouse_position.x;
      i_slider -> main_value [3] = ((i_slider -> main_container[2] -> x + i_slider -> main_container [2] -> w ) / 2 ) * i_slider -> main_value [2];


      // values
      if ( i_slider -> main_value [3] > i_slider -> main_value [0] ) {
        i_slider -> main_value [3] = i_slider -> main_value [0];
      } else if ( i_slider -> main_value [3] < i_slider -> main_container [1] -> x ) {
        i_slider -> main_value [3] = 0;
      }
      xx_itoa ( i_slider -> string, i_slider -> main_value [3] );
      // container
      if ( (i_slider -> main_container [2] -> x + i_slider -> main_container [2] -> w ) > i_slider -> main_container [1] -> w ) {
        i_slider -> main_container [2] -> x = i_slider -> main_container [1] -> w - (i_slider -> main_container [2] -> w / 2);
      } else if ( i_slider -> main_container [2] -> x < i_slider -> main_container [1] -> x ) {
        i_slider -> main_container [2] -> x = i_slider -> main_container [1] -> x - (i_slider -> main_container [2] -> w / 2);
      }

    }
  }

  return 0;
}

/* 
 * function - int xx_get_slider_info ( xx_slider* i_slider );
 *    -- function return the slider value (data)
 * */
int xx_get_slider_info ( xx_slider* i_slider ) {
  return i_slider -> main_value [3];
}

/* 
 * function - void xx_render_slider_text ( SDL_Renderer* i_renderer, xx_slider* i_slider, bm_mgr* i_font );
 * */
void xx_render_slider_text (   SDL_Renderer* i_renderer, xx_slider* i_slider, bm_mgr* i_font ) {
  

  SDL_SetRenderDrawColor ( i_renderer, 150, 150, 185, 200 );
  SDL_RenderDrawRect ( i_renderer, i_slider -> main_container [0] );

  SDL_SetRenderDrawColor ( i_renderer, 50, 75, 90, 200 );
  SDL_RenderDrawLine ( 
      i_renderer,
      i_slider -> main_container [1] -> x,
      i_slider -> main_container [1] -> y,
      i_slider -> main_container [1] -> w,
      i_slider -> main_container [1] -> h
      );

  SDL_SetRenderDrawColor ( i_renderer, 255, 0, 50, 255 );
  SDL_RenderFillRect ( i_renderer, i_slider -> main_container [2] );

  bm_set_color ( i_font, 175, 175, 180 );
  bm_render ( i_renderer, i_font, i_slider -> string, i_slider -> main_container [2] -> x, i_slider -> main_container [3] -> y - (i_font -> z_chang - 3) );
}

/* 
 * function - getters
 * */
int xx_get_slider_total_width ( xx_slider* i_slider ) {
  return i_slider -> main_container [0] -> w;
}

int xx_get_slider_total_height ( xx_slider* i_slider ) {
  return i_slider -> main_container [0] -> h;
}
