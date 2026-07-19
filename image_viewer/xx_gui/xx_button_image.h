/* 
  this file will hold the functions for button - in SDL2/C
*/

#ifndef XX_IMAGE_MANAGER
#include "xx_image_manager.h"
#endif

#define XX_BUTTON 1030

#ifndef XX_BUTTON_PROPERTIES
#include "./xx_button_properties.h"
#endif


typedef struct {
  SDL_Rect* btn_size;
  
  /* 
    I also want my buttons to have images - strings
  */
  
  xx_image_manager* image;
  
  SDL_Point mouse_pos;
  
  // colors
  SDL_Color* border_color;
  SDL_Color* hover_color;
  SDL_Color* click_color;
  
  SDL_Point padding; // x - horizontal and y - vertical
  
  enum Button_Actions btn_state;
  
  SDL_Point btn_active_translate; 
  /* 
    when button is pressed, add 5 pixels to x and 5 pixels to y 
  */
  
  int return_value;
} xx_button_image;

// functions - declarations
void xx_set_default_button_image ( xx_button_image* i_btn ); // sets the default of the button (no need to call if you are calling xx_create_button)
int xx_set_button_image (SDL_Renderer* i_renderer, xx_button_image* btn, char* img_path); // set the image to be shown on the button
void xx_render_button_image (SDL_Renderer* i_renderer, xx_button_image* btn, int x, int y); // render the button with image
void xx_free_button_image (xx_button_image* i_btn); // free the button destoys everything of the inputtd arguments xx_button but with image 



                                                                      // and also sets the default values 
xx_button_image* xx_create_button_image ( SDL_Renderer* i_renderer, char* image_path ); // allocates all the necessary memory and also 
                                                                                  // sets up the image and returns the pointer

int xx_button_image_listen ( xx_button_image* btn, SDL_Event e); // function to listen for button events (put it in SDL_PollEvent while loop)
                                                                              //
int xx_get_button_image_total_width ( xx_button_image* i_button );
int xx_get_button_image_total_height ( xx_button_image* i_button );
/* 
 * function - to change the colors of the button in different action
 * */
void xx_set_button_image_hover_color ( xx_button_image* i_button, SDL_Color* i_color );
void xx_set_button_image_click_color ( xx_button_image* i_button, SDL_Color* i_color );
void xx_set_button_image_border_color ( xx_button_image* i_button, SDL_Color* i_color );



// functions - definitions
/* 
  void xx_render_button_image (SDL_Renderer* i_renderer, xx_button_image* btn);
    - this function will render the button but with an image
*/
void xx_render_button_image (SDL_Renderer* i_renderer, xx_button_image* btn, int x, int y) {
  // btn -> btn_size.x = x + btn -> padding.x;
  // btn -> btn_size.y = y + btn -> padding.y;
  
  btn -> btn_size -> x = x;
  btn -> btn_size -> y = y;
  
  
  // change button border color here
  if (btn -> btn_state == Hovered) {
   // change border color
    SDL_SetRenderDrawColor (i_renderer, btn -> hover_color -> r, btn -> hover_color -> g, btn -> hover_color -> b,
      btn -> hover_color -> a);
  } else if (btn -> btn_state == Pressed) {
    // updating the translation
    btn -> btn_active_translate.x = btn -> btn_size -> x + 5; // this '5' value may set to variable in the future
    btn -> btn_active_translate.y = btn -> btn_size -> y + 5; 
    
    // updating the btn -> size.x and btn -> size.y
    btn -> btn_size -> x = btn -> btn_active_translate.x;
    btn -> btn_size -> y = btn -> btn_active_translate.y;
    
    // change border color
    SDL_SetRenderDrawColor (i_renderer, btn -> click_color -> r, btn -> click_color -> g, btn -> click_color -> b,
      btn -> click_color -> a);
  } else {
    // change border color
    SDL_SetRenderDrawColor (i_renderer, btn -> border_color -> r, btn -> border_color -> g, btn -> border_color -> b,
      btn -> border_color -> a);
  }
  
  // rendering the image first
  xx_render_image (i_renderer, btn -> image, btn -> btn_size -> x + btn -> padding.x, btn -> btn_size -> y + btn -> padding.y);
  
  // rendering the button outline
  SDL_RenderDrawRect (i_renderer, btn -> btn_size);
}




/* 
 * function - free everything of a button
 * */
void xx_free_button_image ( xx_button_image* i_button ) {
  if ( i_button != NULL ) {

    if ( i_button -> image != NULL ) {
      xx_free_image_manager ( i_button -> image );
      i_button -> image = NULL;
    }

    free ( i_button -> btn_size );
    i_button -> btn_size = NULL;

    free ( i_button -> border_color );
    i_button -> border_color = NULL;

    free ( i_button -> hover_color );
    i_button -> hover_color = NULL;

    free ( i_button -> click_color );
    i_button -> click_color = NULL;

    free ( i_button );
    i_button = NULL;
  }


}

/* 
  function - to set up the button with an image
    - it will allocate xx_image_manager
    - set the width and height
*/
int xx_set_button_image (SDL_Renderer* i_renderer, xx_button_image* btn, char* img_path) {
  // freeing the previous image manager or image data 
  //xx_free_image ( btn -> image ); // only destroys image texture data
  xx_free_image_manager ( btn -> image);
  
  // creating and loading the image (using xx_create_image_manager);
  btn -> image = xx_create_image_manager  ( i_renderer, img_path );
  if ( btn -> image == NULL ) {
    printf ("\t Error! xx_set_button_image::could not load image \n");
    return 0;
  }

  btn -> btn_size -> w = btn -> image -> width + (btn -> padding.x * 2);
  btn -> btn_size -> h = btn -> image -> height + (btn -> padding.y * 2);
  
  return 1;
}

/* 
  function - to listen for inputs
    - it will take SDL_Event as input as well 
*/
int xx_button_image_listen ( xx_button_image* btn, SDL_Event e) {

  //btn -> mouse_pos.x = e.button.x;
  //btn -> mouse_pos.y = e.button.y;

  int is_inside = 0;
  //int is_inside = SDL_PointInRect (&btn -> mouse_pos, btn -> btn_size);

  if (e.type == SDL_MOUSEMOTION) {
    SDL_GetMouseState (&btn -> mouse_pos.x, &btn -> mouse_pos.y); // getting the mouse state
    
    if (SDL_PointInRect(&btn -> mouse_pos, btn -> btn_size)) {
      btn -> btn_state = Hovered;
    } else {
      btn -> btn_state = None;
    }
  }
  
  // if a button is pressed
  if (e.type == SDL_MOUSEBUTTONDOWN) {
    is_inside = SDL_PointInRect ( &btn -> mouse_pos, btn -> btn_size );
    // and if the mouse point is inside of the button (while being pressed)
    if (is_inside) {
      btn -> btn_state = Pressed;
      btn -> return_value = 1;
      return 1;
    }
  }
  
  if (e.type == SDL_MOUSEBUTTONUP) {
    if (is_inside) {

      btn -> btn_state = None;
      btn -> return_value = -1;
      return -1;
    }
  }
  
  return 0;
}

/* 
 * function - void xx_set_default_button ( xx_button_image* i_btn );
 *  -- function to set the default values for the button
 * */
void xx_set_default_button_image ( xx_button_image* i_btn ) {
  
  i_btn -> btn_size -> x = 0;
  i_btn -> btn_size -> y = 0;
  i_btn -> btn_size -> w = 0;
  i_btn -> btn_size -> h = 0;

  i_btn -> image = NULL;

  i_btn -> mouse_pos.x = 0;
  i_btn -> mouse_pos.y = 0;

  i_btn -> border_color -> r = 0;
  i_btn -> border_color -> g = 0;
  i_btn -> border_color -> b = 0;
  i_btn -> border_color -> a = 0;


  i_btn -> hover_color -> r = 255;
  i_btn -> hover_color -> g = 0;
  i_btn -> hover_color -> b = 0;
  i_btn -> hover_color -> a = 0;

  i_btn -> click_color -> r = 0;
  i_btn -> click_color -> g = 0;
  i_btn -> click_color -> b = 255;
  i_btn -> click_color -> a = 0;


  i_btn -> padding.x = 3;
  i_btn -> padding.y = 3;

  i_btn -> btn_state = None;
  i_btn -> btn_active_translate.x = 5;
  i_btn -> btn_active_translate.y = 5;

  i_btn -> return_value = 0;
}

/* 
 * xx_button_image* xx_create_button_image ( SDL_Renderer*, char* image_path )
 * */
xx_button_image* xx_create_button_image ( SDL_Renderer* i_renderer, char* image_path ) {

  // creating xx_button 
  xx_button_image* r_button = (xx_button_image*) malloc (sizeof(xx_button_image) * 1);
  if ( r_button == NULL ) {
    printf ("\t Error! Memory allocation, failed, xx_create_button::r_button \n");
    return NULL;
  }

  r_button -> border_color = (SDL_Color*) malloc (sizeof(SDL_Color) * 1);
  if ( r_button -> border_color == NULL ) {
    printf ("\t Error! Memory allocation, failed, xx_create_button::r_button -> border_color\n");
    return NULL;
  }

  r_button -> hover_color = (SDL_Color*) malloc (sizeof(SDL_Color) * 1);
  if ( r_button -> hover_color == NULL ) {
    printf ("\t Error! Memory allocation, failed, xx_create_button::r_button -> hover_color\n");
    return NULL;
  }

  r_button -> click_color = (SDL_Color*) malloc (sizeof(SDL_Color) * 1);
  if ( r_button -> click_color == NULL ) {
    printf ("\t Error! Memory allocation, failed, xx_create_button::r_button -> click_color\n");
    return NULL;
  }

  r_button -> btn_size = (SDL_Rect*) malloc (sizeof(SDL_Rect) * 1);
  if (r_button -> btn_size == NULL) {
    printf ("\t Error! Memory allocation, failed, xx_create_button::r_button -> btn_size\n");
    return NULL;
  }

  xx_set_default_button_image ( r_button );

  if (!xx_set_button_image ( i_renderer, r_button, image_path )) {
    return NULL;
  }

  return r_button;
}

/* 
 * function - to change the color of the button (setters)
 * */
// for hover 
void xx_set_button_image_hover_color ( xx_button_image* i_button, SDL_Color* i_color ) {
  i_button -> hover_color -> r = i_color -> r;
  i_button -> hover_color -> g = i_color -> g;
  i_button -> hover_color -> b = i_color -> b;
  i_button -> hover_color -> a = i_color -> a;
}

// for click 
void xx_set_button_image_click_color ( xx_button_image* i_button, SDL_Color* i_color ) {
  i_button -> click_color -> r = i_color -> r;
  i_button -> click_color -> g = i_color -> g;
  i_button -> click_color -> b = i_color -> b;
  i_button -> click_color -> a = i_color -> a;
}

// for nothing 
void xx_set_button_image_border_color ( xx_button_image* i_button, SDL_Color* i_color ) {
  i_button -> border_color -> r = i_color -> r;
  i_button -> border_color -> g = i_color -> g;
  i_button -> border_color -> b = i_color -> b;
  i_button -> border_color -> a = i_color -> a;
}

/* 
 * function - getters to get the total width and total height of the button
 * */
int xx_get_button_image_total_width ( xx_button_image* i_button ) {
  return i_button -> btn_size -> w;
}

int xx_get_button_image_total_height ( xx_button_image* i_button ) {
  return i_button -> btn_size -> h;
}
