/* 
  this file will only have the information for image rendering and image loading
  
  using SDL_image
*/
#define XX_IMAGE_MANAGER 1010


typedef struct {
  SDL_Texture* texture;
  int width;
  int height;
} xx_image_manager;

/* 
  function - declarations
*/
void xx_free_image (xx_image_manager* i_img); // just Destroys the image texture
int xx_load_image (SDL_Renderer* i_renderer, xx_image_manager* i_img, char* path); // returns 1 - success -- loads the image using SDL_image (make sure to initialize it)
void xx_render_image (SDL_Renderer* i_renderer, xx_image_manager* i_img, int x, int y); // renders the image as it is
xx_image_manager* xx_create_image_manager ( SDL_Renderer* i_renderer, char* path ); // creates the xx_image_manager struct with pointer and returns the pointer to the caller (sets the default values)
                                                                                    // also calls for xx_load_image with it

void xx_free_image_manager ( xx_image_manager* i_image); // frees xx_image_manager* itself along with Destroying img_texture

/* function - definitions */

/* 
  function to destroy and null point the loaded image
*/
void xx_free_image (xx_image_manager* i_img) {
  if (i_img != NULL) {
    SDL_DestroyTexture (i_img -> texture);
    i_img -> texture = NULL;
    i_img -> width = 0;
    i_img -> height = 0;

  }
}

/* 
 * function - void xx_free_image_manager ( xx_image_manager* i_image )
 *  -- this one will call xx_free_image (xx_image_manager*) and also free the actual 
 * */
void xx_free_image_manager ( xx_image_manager* i_image ) {
  xx_free_image ( i_image );

  if ( i_image != NULL ) {
    free ( i_image );
    i_image = NULL;
  }

}

/* 
  function - to load the image
    - will autotmatically load the image 
    - and set the width and height
*/
int xx_load_image (SDL_Renderer* i_renderer, xx_image_manager* i_img, char* path) {
  // xx_free_image (i_img); // freeing the previously loaded image

  if (i_img -> texture != NULL) {
    SDL_DestroyTexture (i_img -> texture);
    i_img -> texture = NULL;
    i_img -> width = 0;
    i_img -> height = 0;
  }
  
  // loading the image using SDL_image
  i_img -> texture = IMG_LoadTexture (i_renderer, path);
  if (i_img -> texture == NULL) {
    printf ("\t ! IMG_LoadTexture - %s\n ", IMG_GetError ());
    return 0;
  } else {
    printf ("\t * IMG_LoadTexture - Success, %s\n", path);
  }
  
  // querying the texture to get width and height
  SDL_QueryTexture (i_img -> texture, NULL, NULL, &i_img -> width, &i_img -> height);
  
  return 1;
}

/* 
  function - render the image
*/
void xx_render_image (SDL_Renderer* i_renderer, xx_image_manager* i_img, int x, int y) {
  SDL_Rect l_rect = {x, y, i_img -> width, i_img -> height};
  SDL_RenderCopy (i_renderer, i_img -> texture, NULL, &l_rect);
}

/* 
 * function - 
xx_image_manager* xx_create_image_manager ( SDL_Renderer* i_renderer, char* path );
 *  -- function to allocate memory to xx_image_manager and also return the pointer
 *  -- this function automatically calls xx_load_image 
 * */
xx_image_manager* xx_create_image_manager ( SDL_Renderer* i_renderer, char* path ) {
  // creating xx_image_manager, r -> return
  xx_image_manager* r_image = (xx_image_manager*) malloc (sizeof(xx_image_manager) * 1);
  if (r_image == NULL) {
    printf ("\t Error! Memory allocation, failed, xx_create_image_manager::r_image \n");
    return NULL;
  }

  r_image -> texture = NULL;
  r_image -> width = 0;
  r_image -> height = 0;

  // loading the image
  if (!xx_load_image ( i_renderer, r_image, path )) {
    printf ("\t Error! Loading Image -> %s\n", path);
    return NULL;
  }

  // returning the pointer
  return r_image;
}
