/* 
  this one is a bit map image manager
  
  this file will have
    - function to load the image (sdl_image.h)
    - function to render the text
*/
#define BM_MGR 1010


typedef struct {
  SDL_Texture* map; // texture to store the bitmap font image
  
  SDL_Rect src; // cursors - source
  SDL_Rect dest; // cursor - destination
  
  int font_size; /* stores the info about font_size*/
  
  int zoom; /* 
    to show the rendered text bigger or smaller
    kind of like setting font_size
  */
  int z_chang;
  /* 
    Stores the zoom change values
    so that I don't have to calculate it everytime (only needed when the zoom is in negative, like -82)
  */
  
  int cur_x; // cursor for destination rendering - pointers_x
  int cur_y; // cursor for destination rendering - pointers_y
  /* 
    in future there will probably be a function using which you can just type in newline text
    like if you input "my name is \nsomething haha";
    then the result will be
    
    ----------------
    my name is
    something haha
    ----------------
  */
  
  int width; // get the width of the loaded image
  int height; // get the height of the loaded image
              //
  int total_width;
  int total_height;
  
  // char file_size [30]; // string to store the the filename - currently not in use
  
} bm_mgr; // bit_map manager


// function declarations
bm_mgr* bm_create (); // it ain't mandatory to do this, but it will fill in the default values
int bm_load (SDL_Renderer* i_renderer, bm_mgr* i_bm, char* file_path); // loads image
void bm_free (bm_mgr* i_bm); // frees those image
void bm_render (SDL_Renderer* i_renderer, bm_mgr* i_bm, char* i_string, int x, int y); // renders the font
void bm_render_advanced (SDL_Renderer* i_renderer, bm_mgr* i_bm, char* i_string, int x, int y, int window_width); // renders font (advanced)
void bm_set_color (bm_mgr* i_bm, int r, int g, int b); // sets the color
void bm_set_zoom (bm_mgr* i_bm, int zoom); // sets the zoom
int bm_get_total_width ( bm_mgr* i_bm, char* string );
int bm_get_total_height ( bm_mgr* i_bm, char* string );// this function does not work with bm_render_advanced right now


/* 
  void bm_update_cursor (bm_mgr*, char, int, int, int, int)
    - just updates the src and destination rect for the text rendering
    - not needed by the user, just used by bm_render
 */
void bm_update_cursor (bm_mgr* i_bm, char i_value, int x, int y, int cur_x, int cur_y);


// function definitions
/* 
  function - bm_load (SDL_Renderer*, bm_mgr*, char*);
    / this function will
    - Load the image
    - Get the Width and Height to struct -> width, struct -> height
    - Get the Font Size (Automatically) struct -> font_size
    - set the Source Cursor width, height (Automatically) Struct -> src.w and Struct -> src.h
    - set the Destination Cursor width, height (Automatically) Struct -> dest.w and Struct -> dest.h
    - And Updates Zoom_Change (z_chang) (Automatically) Struct -> z_chang
*/
int bm_load (SDL_Renderer* i_renderer, bm_mgr* i_bm, char* file_path) {
  // bm_free (i_bm); // freeing old image
  if ( i_bm -> map != NULL ) {
    SDL_DestroyTexture ( i_bm -> map );
    i_bm -> map = NULL;
  }
  
  i_bm -> map = IMG_LoadTexture (i_renderer, file_path); // loading the image
  // error checking
  if (i_bm -> map == NULL) {
    printf ("\t ! Could not load the texture : %s\n", IMG_GetError());
    return -1;
  } else {
    printf ("\t * Texture Loaded \n");
  }
  
  // getting width - height
  SDL_QueryTexture (i_bm -> map, NULL, NULL, &i_bm -> width, &i_bm -> height);
  
  // setting up values for bm_mgr
  
  /* 
    getting font_size of the image (though not vertically)
  */
  i_bm -> font_size = i_bm -> width / 16; 
  
  // setting up width and height for source (if a font is 12 px, then the source should clip out 12x12 px)
  i_bm -> src.w = i_bm -> font_size;
  i_bm -> src.h = i_bm -> font_size;
  
  // setting up the destination width and height 
  i_bm -> dest.w = xx_abs((i_bm -> font_size) + (i_bm -> zoom)); // if this is -1 then the text will be a bit smaller
  i_bm -> dest.h = xx_abs((i_bm -> font_size) + (i_bm -> zoom)); // if this is 1 then the text will be a bit larger
  
  
  /* 
    sets the value for zoom_change, which is mostly used for negative zoom value
  */
  i_bm -> z_chang = xx_abs (i_bm -> font_size + i_bm -> zoom);
  
  
  return 1;
}

/* 
  function - void bm_free ()
      - frees the bitmap font image, that was loaded
*/
void bm_free (bm_mgr* i_bm) {
  if (i_bm -> map != NULL) {
    SDL_DestroyTexture (i_bm -> map);
    i_bm -> map = NULL;
  }

  free ( i_bm );
  i_bm = NULL;
}

/* 
  function - void bm_render ()
      - calls for bm_update_cursor () function to update the cursor
      - renders the inputted text at inputted location (coordinate)
*/
void bm_render (SDL_Renderer* i_renderer, bm_mgr* i_bm, char* i_string, int x, int y) {
  int t_ss = xx_strlen (i_string); // gets the length of the string
  
  //i_bm -> total_width = t_ss * i_bm -> z_chang;
  //i_bm -> total_height = i_bm -> z_chang;
  
  
  for (int i = 0; i < t_ss; i++) {
    bm_update_cursor (i_bm, i_string[i], x, y, i, i_bm -> cur_y); // updates the cursor, src - destination
    SDL_RenderCopy (i_renderer, i_bm -> map, &i_bm -> src, &i_bm -> dest); // renders
  }
  
  
}


/* 
  function - void bm_update_cursor ()
      - updates the source points (x, y)
      - updates the destination points (x, y) - depending if the zoom is negative or positive
*/
void bm_update_cursor (bm_mgr* i_bm, char i_value, int x, int y, int cur_x, int cur_y) {
  i_bm -> src.x = (i_value % 16) * i_bm -> font_size; // sets the source x
  i_bm -> src.y = (i_value / 16) * i_bm -> font_size; // sets the source y 
  
  if (i_bm -> zoom < 0) { // if the zoom is negative
    i_bm -> dest.x = x + (cur_x * i_bm -> z_chang); // updates destination x, when negative
    i_bm -> dest.y = y + (cur_y * i_bm -> z_chang); // updates destination y, when negative
  } else {
    
    i_bm -> dest.x = x + (cur_x * i_bm -> dest.w); // you can try, i_bm -> z_chang
    i_bm -> dest.y = y + (cur_y * i_bm -> dest.h); // you can try, i_bm -> z_chang
  }
  
}


/* 
  function - to set the color of the text (mandatory)
      - sets the font colors, this one is mandatory if you have bitmap font image with white texts
*/
void bm_set_color (bm_mgr* i_bm, int r, int g, int b) {
  SDL_SetTextureColorMod (i_bm -> map, r, g, b); // this function changes color
  /* 
    there needs to be a condition where if the arguments are negative or greater than 255 then something something.
  */
}

/* 
  function - to set the zoom of the text (mandatory)
      - sets the font zoom, call it before calling bm_load () <mandatory>
*/
void bm_set_zoom (bm_mgr* i_bm, int zoom) {
  i_bm -> zoom = zoom; // just updates the bm_mgr value
  
  /* 
    there needs to be a condition where if the (zoom = font_size), then we show error, or update the size 
    to minimum, but that is for later
    
    when (zoom == font_size) {
      - it won't show error message
      - it won't show the text as well
    }
  */
}

/* 
  function - void bm_render_advanced (SDL_Renderer* i_renderer, bm_mgr* i_bm, char* i_string, int x, int y, int window_width)
        - this function will do everything that bm_render does
        - adding to that, can read \n \r and if the text is out of bounds
*/
void bm_render_advanced (SDL_Renderer* i_renderer, bm_mgr* i_bm, char* i_string, int x, int y, int window_width) {
  int string_size = xx_strlen (i_string);
  

  for (int i = 0; i < string_size; i++) {
    i_bm -> src.x = i_string[i] % 16 * i_bm -> font_size;
    i_bm -> src.y = i_string[i] / 16 * i_bm -> font_size;
    
    if ( i_string [i] == '\t' ) {
      i_bm -> cur_x += 4; // tab space
    }
    i_bm -> dest.x = x + (i_bm -> z_chang * i_bm -> cur_x);

    i_bm -> dest.y = y + (i_bm -> z_chang * i_bm -> cur_y);

    i_bm -> cur_x += 1;
    // checking if the x is out of width 
    if ( i_bm -> dest.x + i_bm -> z_chang > window_width ) {
      i_bm -> cur_y += 1;
      i_bm -> cur_x = 0;
    }

    if (i_string [i] == '\n') {
      i_bm -> total_width = i_bm -> cur_x * i_bm -> z_chang;
      i_bm -> cur_x = 0;
      i_bm -> total_height = (i_bm -> cur_y + 1) * i_bm -> z_chang;
      i_bm -> cur_y += 1;
      
    }
    if (i_string [i] == '\r') {
      i_bm -> cur_x = 0;
    }
    SDL_RenderCopy (i_renderer, i_bm -> map, &i_bm -> src, &i_bm -> dest);
    
  }
  i_bm -> cur_x = 0;
  i_bm -> cur_y = 0;
}



/* 
 * function - bm_mgr* bm_create ();
 *    - function to create a dynamic bm_mgr and then return (the ownership it to the caller)
 *    - isn't mandatory to call, but is recommended as it will fill in defaults
 * */
bm_mgr* bm_create () {
  bm_mgr* r_value = (bm_mgr*) malloc (sizeof(bm_mgr) * 1);
  if (r_value == NULL) {
    printf ("\t ! Could not allocate memory -> bm_mgr*\n");
    return NULL;
  }


  r_value -> map = NULL;

  r_value -> zoom = 0;

  r_value -> cur_x = 0;
  r_value -> cur_y = 0;

  r_value -> dest.x = 0;
  r_value -> dest.y = 0;
  r_value -> dest.w = 0;
  r_value -> dest.h = 0;
  
  r_value -> src.x = 0;
  r_value -> src.y = 0;
  r_value -> src.w = 0;
  r_value -> src.h = 0;

  r_value -> font_size = 0;
  r_value -> z_chang = 0;

  r_value -> width = 0;
  r_value -> height = 0;

  return r_value;
}

int bm_get_total_width ( bm_mgr* i_bm, char* string ) {
  i_bm -> total_width = strlen (string) * i_bm -> z_chang;
  return i_bm -> total_width;
}

// this function does not work with bm_render_advanced for now
int bm_get_total_height ( bm_mgr* i_bm, char* string ) {
  i_bm -> total_height = i_bm -> z_chang;
  return i_bm -> total_height;
}
