/* 
  this file will only have the window manager stuff like
    - a struct for window manager (SDL_Window*, SDL_Renderer*, int width, int height, char window_title[20]);
    - functions like
      - create_window_renderer ();
      - destroy_window_renderer ();
*/


typedef struct {
  SDL_Renderer* renderer;
  SDL_Window* window;
  
  int width;
  int height;
  
  char window_title [STRING_SIZE] ;
} xx_window_manager;

// function - declarations
xx_window_manager* xx_create_window_renderer (int width, int height, char* window_title);
void xx_destroy_window_renderer (xx_window_manager* i_window);



// function - definitions

/* 
  function - create_window_renderer
    - to create window and renderer (basic)
*/
xx_window_manager* xx_create_window_renderer (int width, int height, char* window_title) {
  
  xx_window_manager* i_window = (xx_window_manager*) malloc (sizeof(xx_window_manager) * 1);
  if (i_window == NULL) {
    printf ("\t ! Memory allocation, failed, xx_window_manager* i_window\n");
    return NULL;
  }

  i_window -> width = 0;
  i_window -> height = 0;

  if (window_title[0] == '\0') {
    printf ("\t ! Please, fill in window_title, setting it 'default' \n");
    char lt [] = "de_fault";
    xx_strcpy (i_window -> window_title, lt);
  } else {
    xx_strcpy (i_window -> window_title, window_title);
  }
  
  if ( width <= 0 ||  height <= 0) {
    printf ("\t ! Please, set the values of width and height\nsetting it to default 640x480\n");
    i_window -> width = 640;
    i_window -> height = 480;
  } else {
    i_window -> width = width;
    i_window -> height = height;
  }
  
  // creating window and renderer
  i_window -> window = SDL_CreateWindow (i_window -> window_title, 0,
      0, i_window -> width, i_window -> height, SDL_WINDOW_SHOWN);
  if (i_window -> window == NULL) {
    printf ("\t ! SDL_CreateWindow () -> Error, %s\n", SDL_GetError());
    return NULL;
  } else {
    printf ("\t * SDL_CreateWindow () -> Success\n");
  }
  
  // creating renderer
  i_window -> renderer = SDL_CreateRenderer (i_window -> window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (i_window -> renderer == NULL) {
    printf ("\t ! SDL_CreateRenderer () -> Error, %s\n", SDL_GetError());
    return NULL;
  } else {
    printf ("\t * SDL_CreateRenderer () -> Success\n");
  }
  
  return i_window;
};

/* 
  function - xx_destroy_window_renderer
    - frees the window and renderer
*/
void xx_destroy_window_renderer (xx_window_manager* i_window) {
  if (i_window -> renderer != NULL) {
    SDL_DestroyRenderer (i_window -> renderer);
    i_window -> renderer = NULL;
  }
  
  if (i_window -> window != NULL) {
    SDL_DestroyWindow (i_window -> window);
    i_window -> window = NULL;
  }

  free (i_window);
  i_window = NULL;
}
