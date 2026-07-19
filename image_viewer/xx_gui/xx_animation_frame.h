/* 
 * unlike xx_animation_frame* this file will be solely for sprite sheet animation and noothing more
 * just like bm_mgr - for font
 * */

typedef struct {
  // for the images
  SDL_Texture* image_texture;
  int image_width;
  int image_height;

  // sourc rect and destinatioon rect 
  int rect_count; // 2 - default 
  SDL_Rect** box; // 0 - source, 1 will be destination
  int* n; // 0 - number of col, 1 - number of row
  SDL_Point* cursor; // cursor to change the frame
  // these two for the smooth animation
  int speed;
  int counter;
} xx_animation_frame;

/* 
 * function - declarations
 * */
// this functoin will create the animation struct and return xx_animation_frame* 
// arguments - sdl_renderer*, image path, 
// per_w is the image's total width - set to -1, calculates automatically only if there is one row of sprite
// per_h is the image's total height - set to -1, calculates automatically only if there is one row of sprite
// n_w -> compulsory, number of sprites on a row -> 
// n_h -> compulsory, number of sprites on a colmn \|/ 
xx_animation_frame* xx_create_animation_frame ( SDL_Renderer* i_renderer, char* img_path, int per_w, int per_h, int n_w, int n_h );

// frees the animation frame
void xx_free_animation_frame ( xx_animation_frame* i_frame );

void xx_change_animation_sprite ( xx_animation_frame* i_frame, int value ); // change row of animation, from row 0 (idle sprite) to row 1 (running sprite)

/* 
 * this function will render the animation or the image (with animation)
 * arguments
 * sdl_rendere*, xx_animation_frame*
 * x, y - rendering coordinates
 * w, h - destination size, if I want 300:300 single sprite to be shown a bit small on animation use 100:100
 * */
void xx_render_animation_frame ( SDL_Renderer* i_renderer, xx_animation_frame* i_frame, int x, int y, int w, int h ); 

/* 
 * this function will have faster speed on low number and slower speed on high number. 
 * for e.g. -> if you enter 
 * xx_animation_frame* anime = xx_create_animation_frame (...);
 * xx_set_animation_frame ( anime, 2 ); // this will be faster
 * xx_set_animation_frame ( anime, 20 ); // this will be slower
 * */
void xx_set_animation_speed ( xx_animation_frame* i_frame, int value ); // changes the speed of the animation
/* 
 * function - definitions
 * */

/* 
 * function - xx_animation_frame* xx_create_animation_frame ( SDL_Renderer* i_renderer, char* img_path, int per_w, int per_h, int n_w, int n_h );
 *    -- function to allocate memory and return the pointer
 *    -- it takes in SDL_Renderer, char* to image path, and width and height for sprite (set to -1 or negative value for auto calculation <works only for a single row of sprite sheet>)
 *    -- the last two arguments are the number of frames or pictures in a row or a column (these must be entered) 
 * */
xx_animation_frame* xx_create_animation_frame ( SDL_Renderer* i_renderer, char* img_path, int per_w, int per_h, int n_w, int n_h ) {
  xx_animation_frame* r_anime = ( xx_animation_frame* ) malloc (sizeof(xx_animation_frame) * 1);
  if ( r_anime == NULL ) {
    printf ("\t Error! Memory Allocation, Failed, xx_animation_frame*_frame*\n");
    return NULL;
  }

  // loading image using IMG_LoadTexture 
  // make sure to initialize SDL_image before hand 
  r_anime -> image_texture = IMG_LoadTexture ( i_renderer, img_path );
  if ( r_anime -> image_texture == NULL ) {
    printf ("\t Error! Memory Allocation, Failed, xx_animation_frame*_frame* -> SDL_Texture* image_texture\n");
    return NULL;
  }

  SDL_QueryTexture ( r_anime -> image_texture, NULL, NULL, &r_anime -> image_width, &r_anime -> image_height );

  r_anime -> rect_count = 2;
  r_anime -> speed = 20;
  r_anime -> counter = 1; // always start with 1 cannot be 0 or less than 0

  // allocating memory to the r_anime -> box (rects)
  r_anime -> box = (SDL_Rect**) malloc (sizeof(SDL_Rect*) * r_anime -> rect_count);
  if ( r_anime -> box == NULL) {
    printf ("\t Error! Memory Allocation, Failed, xx_animation_frame*_frame* -> SDL_Rect** box\n");
    return NULL;
  }

  r_anime -> n = (int*) malloc (sizeof(int) * r_anime -> rect_count);

  for ( int i = 0; i < r_anime -> rect_count; i++ ) {
    r_anime -> box [i] = (SDL_Rect*) malloc (sizeof(SDL_Rect) * 1);
    if ( r_anime -> box [i] == NULL ) {
      printf ("\t Error! Memory Allocation, Failed, xx_animation_frame*_frame* -> SDL_Rect** box [%d]\n", i);
      return NULL;
    }
  }

  r_anime -> cursor = (SDL_Point*) malloc (sizeof(SDL_Point) * 2);
  if ( r_anime -> cursor == NULL ) {
    printf ("\t Error! Memory Allocation, Failed, xx_animation_frame*_frame* -> SDL_Point*\n");
    return NULL;
  }

  r_anime -> cursor -> x = 0;
  r_anime -> cursor -> y = 0;

  // calculating per frame width and height 
  if ( per_w < 0 ) {
    r_anime -> box [0] -> w = r_anime -> image_width / n_w;
    r_anime -> box [1] -> w = r_anime -> box [0] -> w;
  } else {
    r_anime -> box [0] -> w = per_w;
    r_anime -> box[1] -> w = per_w;
  }

  if ( per_h < 0 ) {
    r_anime -> box [0] -> h = r_anime -> image_height / n_h;
    r_anime -> box [1] -> h = r_anime -> box [0] -> h;
  } else {
    r_anime -> box [0] -> h = per_h;
    r_anime -> box [1] -> h = per_h;
  }

  r_anime -> n [0] = n_w;
  r_anime -> n [1] = n_h;

  return r_anime;
}

/* 
 * function - void xx_free_animation_frame ( xx_animation_frame* i_frame );
 *    -- to free memory
 * */
void xx_free_animation_frame ( xx_animation_frame* i_frame ) {
  if ( i_frame != NULL ) {
    free ( i_frame -> n );
    i_frame -> n = NULL;

    for ( int i = 0; i < i_frame -> rect_count; i++ ) {
      free ( i_frame -> box[i] );
      i_frame -> box [i] = NULL;
    }

    free ( i_frame -> box );
    i_frame -> box = NULL;

    free ( i_frame -> n );
    i_frame -> n = NULL;

    free ( i_frame -> cursor );
    i_frame -> cursor = NULL;

    SDL_DestroyTexture ( i_frame -> image_texture );
    i_frame -> image_texture = NULL;

    free ( i_frame );
    i_frame = NULL;

  }
}

/* 
 * function - void xx_render_animation_frame ( SDL_Renderer* i_renderer, xx_animation_frame* i_frame, int x, int y, int w, int h );
 *    -- this function will render the texture (animated one) to that particular x and y coordination 
 *    -- update, these width and height or w / h at the end are for destination scaling
 * */
void xx_render_animation_frame ( SDL_Renderer* i_renderer, xx_animation_frame* i_frame, int x, int y, int w, int h ) {
  i_frame -> box [1] -> x = x;
  i_frame -> box [1] -> y = y;

  i_frame -> box [1] -> w = w;
  i_frame -> box [1] -> h = h;


  i_frame -> box [0] -> x = i_frame -> box [0] -> w * i_frame -> cursor -> x; 
  i_frame -> box [0] -> y = i_frame -> box [0] -> h * i_frame -> cursor -> y;


  // rendering 
  SDL_RenderCopy ( i_renderer, i_frame -> image_texture, i_frame -> box [0], i_frame -> box [1] );

  i_frame -> counter += 1;

  if ( i_frame -> counter % i_frame -> speed == 0) {
    i_frame -> counter = 1;
    i_frame -> cursor -> x += 1;
  }
  if ( i_frame -> cursor -> x  >= i_frame -> n[0]) {
    i_frame -> cursor -> x = 0;
  }
}

/* 
 * function - void xx_change_animation_sprite ( xx_animation_frame* i_frame, int value );
 *  -- this function will change the y axis so that another kind of frame will be rendered
 *  -- Meaning, If you have
 *  [2, 3, 4]
 *  [a, b, c]
 *  I am rendering 2, 3, 4, then I use this function like ( xx_change_animation_sprite ( anime1, 1 ) ) - moving the cursor to 1 row from 0
 *  then it will render a, b, c
 * */
void xx_change_animation_sprite ( xx_animation_frame* i_frame, int value ) {
  i_frame -> cursor -> y = value;
}

/* 
 * function - void xx_set_animation_speed ( xx_animation_frame* i_frame, int value ); // changes the speed of the animation
 *    -- function to change the speed of the animation (using module operator)
 * */
void xx_set_animation_speed ( xx_animation_frame* i_frame, int value ) {
  if ( value > 0 ) {
    i_frame -> speed = value;
  }
}
