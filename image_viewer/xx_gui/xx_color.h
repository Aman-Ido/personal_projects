/* 
  this file holds the color defitions for
    - click color (activate)
    - hover color (hover)
    - default color (nothing / none)
*/

#define XX_COLOR 1010

typedef enum {
  // FE5D26
  XX_HOVER_R = 254,
  XX_HOVER_G = 93,
  XX_HOVER_B = 26,
  XX_HOVER_A = 99,
  
  // F9DB6D
  XX_CLICK_R = 249,
  XX_CLICK_G = 219,
  XX_CLICK_B = 109,
  XX_CLICK_A = 99,
  
  
  // 85877C
  XX_NONE_R = 85,
  XX_NONE_G = 87,
  XX_NONE_B = 124,
  XX_NONE_A = 99,
} XX_Color_Defaults;

typedef struct {
  SDL_Color* hover;
  SDL_Color* none;
  SDL_Color* click;
} xx_color;

/* 
  function - declarations
*/
xx_color* xx_create_color (); // to allocate the color and set it to the default values
void xx_free_color (xx_color* c1); // to free / free the dynamic allocated color

/* 
  function - definitions
*/

/* 
  function - xx_color* xx_create_color ();
      - allocates three colors type (hover, click (active), none (default))
      - sets the value as per the enum <XX_Color_Defaults>
      - returns the malloc array of the struct
*/
xx_color* xx_create_color () {
  xx_color* c1 = (xx_color*) malloc (sizeof(xx_color) * 1);
  if (c1 == NULL) {
    printf ("\t ! xx_create_color () - memory allocation, failed, main struct <xx_color>\n");
    return NULL;
  } else {
    printf ("\t * xx_create_color () - memory allocation, success, main struct <xx_color>\n");
  }
  
  c1 -> hover = (SDL_Color*) malloc (sizeof(SDL_Color) * 1);
  if (c1 -> hover == NULL) {
    printf ("\t ! xx_create_color () - memory allocation, failed, hover\n");
    return NULL;
  } else {
    printf ("\t * xx_create_color () - memory allocation, success, hover\n");
  }
  
  c1 -> none = (SDL_Color*) malloc (sizeof(SDL_Color) * 1);
  if (c1 -> none == NULL) {
    printf ("\t ! xx_create_color () - memory allocation, failed, none\n");
    return NULL;
  } else {
    printf ("\t * xx_create_color () - memory allocation, success, none\n");
  }
  
  
  c1 -> click = (SDL_Color*) malloc (sizeof(SDL_Color) * 1);
  if (c1 -> click == NULL) {
    printf ("\t ! xx_create_color () - memory allocation, failed, click\n");
    return NULL;
  } else {
    printf ("\t * xx_create_color () - memory allocation, success, click\n");
  }
  
  c1 -> hover -> r = XX_HOVER_R;
  c1 -> hover -> g = XX_HOVER_G;
  c1 -> hover -> b = XX_HOVER_B;
  c1 -> hover -> a = XX_HOVER_A;
  
  c1 -> click -> r = XX_CLICK_R;
  c1 -> click -> g = XX_CLICK_G;
  c1 -> click -> b = XX_CLICK_B;
  c1 -> click -> a = XX_CLICK_A;
  
  c1 -> none -> r = XX_NONE_R;
  c1 -> none -> g = XX_NONE_G;
  c1 -> none -> b = XX_NONE_B;
  c1 -> none -> a = XX_NONE_A;
  
  return c1;
}

/* 
  function - xx_free_color (xx_color* c1);
      - frees the colors
      - frees the xx_color*
*/
void xx_free_color (xx_color* c1) {
  if (c1 -> hover != NULL) {
    free (c1 -> hover);
    c1 -> hover = NULL;
  }
  
  if (c1 -> none != NULL) {
    free (c1 -> none);
    c1 -> none = NULL;
  }
  
  if (c1 -> click != NULL) {
    free (c1 -> click);
    c1 -> click = NULL;
  }
  
  free (c1);
}
