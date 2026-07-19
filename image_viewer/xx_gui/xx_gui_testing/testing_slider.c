/* 
 * testing out xx_slider
 * compiled using:
 *  Window -> gcc testing_slider.c -o testing_slider -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
 *  Linux -> gcc testing_slider.c -o testing_slider -lSDL2main -lSDL2 -lSDL2_image
 * */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../xx_string.h"
#include "../bm_mgr.h"
#include "../xx_slider.h"
#include "../xx_window_manager.h"


int main ( int argc, char** argv ) {
  SDL_Init ( SDL_INIT_VIDEO );
  IMG_Init ( IMG_INIT_PNG );

  xx_window_manager* window1 = xx_create_window_renderer (640, 480, (char*) "Testing xx_slider");
  bm_mgr* font1 = bm_create ();
  bm_set_zoom ( font1, -10 );
  bm_load ( window1 -> renderer, font1, (char*) "../font1.png" );

  xx_slider* slider1 = xx_create_slider ( 200, 50, 300, 10, 10, 340, 50  );

  xx_print_slider_info ( slider1 );

  SDL_Event e;
  int quit = 0;

  while (!quit) {
    while (SDL_PollEvent (&e)) {
      if (e.type == SDL_QUIT ) {
        quit = 1;
      }
      if (  xx_listen_slider ( slider1, e ) ) {
        printf ("%d\n", xx_get_slider_info ( slider1 ));
      }
    }

    SDL_SetRenderDrawColor ( window1 -> renderer, 255, 255, 255, 255 );
    SDL_RenderClear ( window1 -> renderer  );

    //xx_render_slider ( window1 -> renderer, slider1 );
    xx_render_slider_text ( window1 -> renderer, slider1, font1 );

    SDL_RenderPresent ( window1 -> renderer );

  }

  xx_free_slider ( slider1 );
  bm_free ( font1 );
  xx_destroy_window_renderer ( window1 );
  IMG_Quit ();
  SDL_Quit ();
  return 0;
}
