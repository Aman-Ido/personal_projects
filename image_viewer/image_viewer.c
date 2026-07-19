/* 
 * Image Viewer - This will be a very simple image viewer using SDL2 and SDL2_image, with C
 * for gui I'm going to use my own xx_gui
 * */
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>


#define WINDOW_WIDTH 100
#define WINDOW_HEIGHT 100
#define STRING_SIZE 512

#include "xx_gui/xx_string.h"
#include "xx_gui/xx_window_manager.h"
#include "xx_gui/bm_mgr.h"
#include "xx_gui/xx_list.h"



struct Bundle {
  bm_mgr* font;
  xx_window_manager* window;
  int window_width;
  int window_height;

  xx_list* path_record;
  SDL_Texture* m_texture; // this is the main texture
  int image_width;
  int image_height;

  // for the directories
  DIR* directory;
  char* home_path;
  char* full_path;
  int full_path_size;
  struct dirent* entry; // for entry -> d_name
  
  // for controlling the pictures / index 
  int image_index;
  int max_image_index;
  SDL_Rect rendering_rect;
  
  // for checking if it is a directory or something else
  struct stat file_stat;
  // the main switch
  int quit;
};

#include "functions.c"

int main ( int argc, char** argv ) {

  if ( iv_init_lib () != 1 )  {
    fprintf (stderr, "iv_init_lib() - failed\n");return 0;
  }

  struct Bundle* bun = iv_init_bundle ();
  if ( bun == NULL ) {
    fprintf ( stderr, "iv_init_bundle() - failed\n" );
    return 0;
  }

  if ( iv_event (bun) != 1 ) {
    fprintf (stderr, "iv_event () - failed\n");
    return 0;
  }


  iv_free_bundle ( bun );
  iv_free_lib ();

  return 0;
}
